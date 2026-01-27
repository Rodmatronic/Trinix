/*
 * main.c - kernel entrypoint. This is jumped to after entry.S and is where the magic happens. Bootstrap
 * the system and eventually setup userspace with INITCODE.
 */

#include <types.h>
#include <defs.h>
#include <param.h>
#include <memlayout.h>
#include <mmu.h>
#include <proc.h>
#include <x86.h>
#include <config.h>
#include <time.h>
#include <version.h>
#include <traps.h>

void cmosphys(void);
extern pde_t *kpgdir;
extern char end[]; // first address after kernel loaded from ELF file
char * banner = sys_version "\n";

// Bootstrap processor starts running C code here.
// Allocate a real stack and switch to it, first
// doing some setup required for memory allocator to work.
// Doing int function returns so GCC will not complain
int kmain(unsigned int addr){
	uartinit();	// serial port for debugging
	printk(banner);
	mbootinit(addr);	// multiboot
	kinit1(end, P2V(4*1024*1024)); // phys page allocator
	kvmalloc();	// kernel page table
	timeinit();	// set up unix date&time
	tscinit();	// TSC/PIT granular time
	mpinit();	// detect other processors
	lapicinit();	// interrupt controller
	seginit();	// segment descriptors
	ioapicinit();	// another interrupt controller
	ioapicenable(IRQ_COM1, 0);
	consoleinit();	// console hardware
	pinit();	// process table
	tvinit();	// trap vectors
	binit();	// buffer cache
	fileinit();	// file table
	ideinit();	// disk 
	kinit2(P2V(4*1024*1024), P2V(PHYSTOP)); // must come after startothers()
	userinit();	// first user process
	idtinit();	// load idt register
	scheduler();	// start running processes
	return -1; /* NOT REACHED */
}

pde_t entrypgdir[];	// For entry.S

// The boot page table used in entry.S and entryother.S.
// Page directories (and page tables) must start on page boundaries,
// hence the __aligned__ attribute.
// PTE_PS in a page directory entry enables 4Mbyte pages.

__attribute__((__aligned__(PGSIZE)))
pde_t entrypgdir[NPDENTRIES] = {
	// Map VA's [0, 4MB) to PA's [0, 4MB)
	[0] = (0) | PTE_P | PTE_W | PTE_PS,
	// Map VA's [KERNBASE, KERNBASE+4MB) to PA's [0, 4MB)
	[KERNBASE>>PDXSHIFT] = (0) | PTE_P | PTE_W | PTE_PS,
};

