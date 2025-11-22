# Initial process execs /init.
# This code runs in user space.

#include <syscall.h>
#include <traps.h>


# exec(init, argv)

.globl start
start:

	# open /dev/console
	movl	$SYS_open, %eax
	movl	$console, %ebx
	movl	$0x002, %ecx
	int	$T_SYSCALL

	# mknod /dev/console
	movl	$SYS_mknod, %eax
	movl	$console, %ebx
	movl	$0x1, %ecx
	movl	$0x1, %edx
	int	$T_SYSCALL

	# open /dev/console again
	movl	$SYS_open, %eax
	movl	$console, %ebx
	movl	$0x002, %ecx
	int	$T_SYSCALL

	# dup 1
	movl	$SYS_dup, %eax
	movl	$0x0, %ebx
	int	$T_SYSCALL

	# dup 2
	movl	$SYS_dup, %eax
	movl	$0x0, %ebx
	int	$T_SYSCALL

	# exec("/sbin/init", argv)
	movl	$SYS_exec, %eax
	movl	$init, %ebx		# arg0 = path
	movl	$argv, %ecx		# arg1 = argv
	int	$T_SYSCALL

exit:
	movl	$SYS_exit, %eax
	movl	$0, %ebx
	int	$T_SYSCALL
	jmp	exit

init:
	.string "/sbin/init\0"
console:
	.string "/dev/console\0"

.p2align 2
argv:
	.long init
	.long 0

