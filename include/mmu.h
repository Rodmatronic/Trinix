#ifndef MMU_H
#define MMU_H

// This file contains definitions for the
// x86 memory management unit (MMU).

// Eflags register
#define FL_IF           0x00000200      // Interrupt Enable

// Control Register flags
#define CR0_PE          0x00000001      // Protection Enable
#define CR0_WP          0x00010000      // Write Protect
#define CR0_PG          0x80000000      // Paging

#define CR4_PSE         0x00000010      // Page size extension

// various segment selectors.
#define SEG_KCODE 2  // kernel code
#define SEG_KDATA 3  // kernel data+stack
#define SEG_UCODE 4  // user code
#define SEG_UDATA 5  // user data+stack
#define SEG_TSS   6  // this process's task state

// cpu->gdt[NSEGS] holds the above segments.
#define NSEGS     7

#ifndef __ASSEMBLER__
// Segment Descriptor
struct segdesc {
  unsigned int lim_15_0 : 16;  // Low bits of segment limit
  unsigned int base_15_0 : 16; // Low bits of segment base address
  unsigned int base_23_16 : 8; // Middle bits of segment base address
  unsigned int type : 4;       // Segment type (see STS_ constants)
  unsigned int s : 1;          // 0 = system, 1 = application
  unsigned int dpl : 2;        // Descriptor Privilege Level
  unsigned int p : 1;          // Present
  unsigned int lim_19_16 : 4;  // High bits of segment limit
  unsigned int avl : 1;        // Unused (available for software use)
  unsigned int rsv1 : 1;       // Reserved
  unsigned int db : 1;         // 0 = 16-bit segment, 1 = 32-bit segment
  unsigned int g : 1;          // Granularity: limit scaled by 4K when set
  unsigned int base_31_24 : 8; // High bits of segment base address
};

// Normal segment
#define SEG(type, base, lim, dpl) (struct segdesc)    \
{ ((lim) >> 12) & 0xffff, (unsigned int)(base) & 0xffff,      \
  ((unsigned int)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (unsigned int)(lim) >> 28, 0, 0, 1, 1, (unsigned int)(base) >> 24 }
#define SEG16(type, base, lim, dpl) (struct segdesc)  \
{ (lim) & 0xffff, (unsigned int)(base) & 0xffff,              \
  ((unsigned int)(base) >> 16) & 0xff, type, 1, dpl, 1,       \
  (unsigned int)(lim) >> 16, 0, 0, 1, 0, (unsigned int)(base) >> 24 }
#endif

#define DPL_USER    0x3     // User DPL

// Application segment type bits
#define STA_X       0x8     // Executable segment
#define STA_W       0x2     // Writeable (non-executable segments)
#define STA_R       0x2     // Readable (executable segments)

// System segment type bits
#define STS_T32A    0x9     // Available 32-bit TSS
#define STS_IG32    0xE     // 32-bit Interrupt Gate
#define STS_TG32    0xF     // 32-bit Trap Gate

// A virtual address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va)         (((unsigned int)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va)         (((unsigned int)(va) >> PTXSHIFT) & 0x3FF)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((unsigned int)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Page directory and page table constants.
#define NPDENTRIES      1024    // # directory entries per page directory
#define NPTENTRIES      1024    // # PTEs per page table
#define PGSIZE          4096    // bytes mapped by a page

#define PTXSHIFT        12      // offset of PTX in a linear address
#define PDXSHIFT        22      // offset of PDX in a linear address

#define PGROUNDUP(sz)  (((sz)+PGSIZE-1) & ~(PGSIZE-1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE-1))

// Page table/directory entry flags.
#define PTE_P           0x001   // Present
#define PTE_W           0x002   // Writeable
#define PTE_U           0x004   // User
#define PTE_PS          0x080   // Page Size

// Address in page table or page directory entry
#define PTE_ADDR(pte)   ((unsigned int)(pte) & ~0xFFF)
#define PTE_FLAGS(pte)  ((unsigned int)(pte) &  0xFFF)

#ifndef __ASSEMBLER__
typedef unsigned int pte_t;

// Task state segment format
struct taskstate {
  unsigned int link;         // Old ts selector
  unsigned int esp0;         // Stack pointers and segment selectors
  ushort ss0;        //   after an increase in privilege level
  ushort padding1;
  unsigned int *esp1;
  ushort ss1;
  ushort padding2;
  unsigned int *esp2;
  ushort ss2;
  ushort padding3;
  void *cr3;         // Page directory base
  unsigned int *eip;         // Saved state from last task switch
  unsigned int eflags;
  unsigned int eax;          // More saved state (registers)
  unsigned int ecx;
  unsigned int edx;
  unsigned int ebx;
  unsigned int *esp;
  unsigned int *ebp;
  unsigned int esi;
  unsigned int edi;
  ushort es;         // Even more saved state (segment selectors)
  ushort padding4;
  ushort cs;
  ushort padding5;
  ushort ss;
  ushort padding6;
  ushort ds;
  ushort padding7;
  ushort fs;
  ushort padding8;
  ushort gs;
  ushort padding9;
  ushort ldt;
  ushort padding10;
  ushort t;          // Trap on task switch
  ushort iomb;       // I/O map base address
};

// Gate descriptors for interrupts and traps
struct gatedesc {
  unsigned int off_15_0 : 16;   // low 16 bits of offset in segment
  unsigned int cs : 16;         // code segment selector
  unsigned int args : 5;        // # args, 0 for interrupt/trap gates
  unsigned int rsv1 : 3;        // reserved(should be zero I guess)
  unsigned int type : 4;        // type(STS_{IG32,TG32})
  unsigned int s : 1;           // must be 0 (system)
  unsigned int dpl : 2;         // descriptor(meaning new) privilege level
  unsigned int p : 1;           // Present
  unsigned int off_31_16 : 16;  // high bits of offset in segment
};

// Set up a normal interrupt/trap gate descriptor.
// - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
//   interrupt gate clears FL_IF, trap gate leaves FL_IF alone
// - sel: Code segment selector for interrupt/trap handler
// - off: Offset in code segment for interrupt/trap handler
// - dpl: Descriptor Privilege Level -
//        the privilege level required for software to invoke
//        this interrupt/trap gate explicitly using an int instruction.
#define SETGATE(gate, istrap, sel, off, d)                \
{                                                         \
  (gate).off_15_0 = (unsigned int)(off) & 0xffff;                \
  (gate).cs = (sel);                                      \
  (gate).args = 0;                                        \
  (gate).rsv1 = 0;                                        \
  (gate).type = (istrap) ? STS_TG32 : STS_IG32;           \
  (gate).s = 0;                                           \
  (gate).dpl = (d);                                       \
  (gate).p = 1;                                           \
  (gate).off_31_16 = (unsigned int)(off) >> 16;                  \
}

#endif
#endif
