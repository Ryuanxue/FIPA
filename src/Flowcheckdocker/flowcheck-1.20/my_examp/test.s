	.file	"test.c"
	.text
	.type	VALGRIND_PRINTF, @function
VALGRIND_PRINTF:
.LFB0:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$72, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	leal	12(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	$5123, -36(%ebp)
	movl	-60(%ebp), %eax
	movl	%eax, -32(%ebp)
	leal	-48(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -16(%ebp)
	leal	-36(%ebp), %eax
	movl	$0, %edx
#APP
# 6779 "../include/valgrind/valgrind.h" 1
	roll $3,  %edi ; roll $13, %edi
	roll $29, %edi ; roll $19, %edi
	xchgl %ebx,%ebx
# 0 "" 2
#NO_APP
	movl	%edx, -44(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -40(%ebp)
	movl	-40(%ebp), %eax
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L3
	call	__stack_chk_fail_local
.L3:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	VALGRIND_PRINTF, .-VALGRIND_PRINTF
	.type	VALGRIND_PRINTF_BACKTRACE, @function
VALGRIND_PRINTF_BACKTRACE:
.LFB1:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$72, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	%eax, -60(%ebp)
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	leal	12(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	$5124, -36(%ebp)
	movl	-60(%ebp), %eax
	movl	%eax, -32(%ebp)
	leal	-48(%ebp), %eax
	movl	%eax, -28(%ebp)
	movl	$0, -24(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -16(%ebp)
	leal	-36(%ebp), %eax
	movl	$0, %edx
#APP
# 6818 "../include/valgrind/valgrind.h" 1
	roll $3,  %edi ; roll $13, %edi
	roll $29, %edi ; roll $19, %edi
	xchgl %ebx,%ebx
# 0 "" 2
#NO_APP
	movl	%edx, -44(%ebp)
	movl	-44(%ebp), %eax
	movl	%eax, -40(%ebp)
	movl	-40(%ebp), %eax
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L6
	call	__stack_chk_fail_local
.L6:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	VALGRIND_PRINTF_BACKTRACE, .-VALGRIND_PRINTF_BACKTRACE
	.globl	gval
	.data
	.align 4
	.type	gval, @object
	.size	gval, 4
gval:
	.long	3
	.comm	buf,16,4
	.text
	.globl	fun
	.type	fun, @function
fun:
.LFB2:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	8(%ebp), %eax
	movl	(%eax), %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	fun, .-fun
	.globl	fun1
	.type	fun1, @function
fun1:
.LFB3:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	12(%ebp), %eax
	addl	$1, %eax
	movl	%eax, 8(%ebp)
	movl	8(%ebp), %eax
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	fun1, .-fun1
	.globl	fun2
	.type	fun2, @function
fun2:
.LFB4:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$0, -8(%ebp)
	movl	$0, -4(%ebp)
	jmp	.L12
.L13:
	movl	-4(%ebp), %eax
	leal	0(,%eax,4), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	(%eax), %eax
	addl	%eax, -8(%ebp)
	addl	$1, -4(%ebp)
.L12:
	movl	-4(%ebp), %eax
	cmpl	12(%ebp), %eax
	jl	.L13
	movl	$1, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	fun2, .-fun2
	.globl	fun4
	.type	fun4, @function
fun4:
.LFB5:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.ax
	addl	$_GLOBAL_OFFSET_TABLE_, %eax
	movl	$2, -4(%ebp)
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	fun4, .-fun4
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	leal	4(%esp), %ecx
	.cfi_def_cfa 1, 0
	andl	$-16, %esp
	pushl	-4(%ecx)
	pushl	%ebp
	.cfi_escape 0x10,0x5,0x2,0x75,0
	movl	%esp, %ebp
	pushl	%ebx
	pushl	%ecx
	.cfi_escape 0xf,0x3,0x75,0x78,0x6
	.cfi_escape 0x10,0x3,0x2,0x75,0x7c
	subl	$80, %esp
	call	__x86.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx
	movl	%gs:20, %eax
	movl	%eax, -12(%ebp)
	xorl	%eax, %eax
	movl	$3, -72(%ebp)
	movl	$0, -64(%ebp)
	movl	$1178796048, -36(%ebp)
	leal	-72(%ebp), %eax
	movl	%eax, -32(%ebp)
	movl	$0, -28(%ebp)
	movl	$0, -24(%ebp)
	movl	$0, -20(%ebp)
	movl	$0, -16(%ebp)
	leal	-36(%ebp), %eax
	movl	$0, %edx
#APP
# 32 "test.c" 1
	roll $3,  %edi ; roll $13, %edi
	roll $29, %edi ; roll $19, %edi
	xchgl %ebx,%ebx
# 0 "" 2
#NO_APP
	movl	%edx, -68(%ebp)
	movl	-68(%ebp), %eax
	movl	%eax, -64(%ebp)
	movl	-72(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -76(%ebp)
	movl	-76(%ebp), %eax
	subl	$1, %eax
	movl	%eax, -76(%ebp)
	movl	-76(%ebp), %edx
	movl	-76(%ebp), %eax
	imull	%edx, %eax
	movl	%eax, -76(%ebp)
	movl	$3, -76(%ebp)
	subl	$12, %esp
	pushl	$8
	call	malloc@PLT
	addl	$16, %esp
	movl	%eax, -60(%ebp)
	movl	-76(%ebp), %edx
	movl	-60(%ebp), %eax
	movl	%edx, (%eax)
	movl	-60(%ebp), %eax
	leal	4(%eax), %edx
	movl	-72(%ebp), %eax
	movl	%eax, (%edx)
	movl	-76(%ebp), %eax
	testl	%eax, %eax
	je	.L17
	movl	-76(%ebp), %eax
	addl	$2, %eax
	movl	%eax, -76(%ebp)
	movl	-72(%ebp), %eax
	addl	$3, %eax
	movl	%eax, gval@GOTOFF(%ebx)
	movl	-76(%ebp), %edx
	movl	buf@GOT(%ebx), %eax
	movl	%edx, (%eax)
	movl	-72(%ebp), %edx
	movl	buf@GOT(%ebx), %eax
	movl	%edx, 4(%eax)
	movl	buf@GOT(%ebx), %eax
	movl	$0, 8(%eax)
	movl	gval@GOTOFF(%ebx), %edx
	movl	buf@GOT(%ebx), %eax
	movl	%edx, 12(%eax)
	movl	-76(%ebp), %eax
	movl	%eax, -52(%ebp)
	movl	-72(%ebp), %eax
	movl	%eax, -48(%ebp)
	movl	-76(%ebp), %eax
	addl	$3, %eax
	movl	%eax, -76(%ebp)
	movl	-76(%ebp), %eax
	movl	%eax, -56(%ebp)
	movl	gval@GOTOFF(%ebx), %eax
	addl	%eax, -56(%ebp)
.L17:
	call	fun4
	subl	$12, %esp
	leal	-72(%ebp), %eax
	pushl	%eax
	call	fun
	addl	$16, %esp
	movl	%eax, -76(%ebp)
	movl	-72(%ebp), %edx
	movl	-76(%ebp), %eax
	subl	$8, %esp
	pushl	%edx
	pushl	%eax
	call	fun1
	addl	$16, %esp
	movl	%eax, -76(%ebp)
	subl	$8, %esp
	pushl	$4
	movl	buf@GOT(%ebx), %eax
	pushl	%eax
	call	fun2
	addl	$16, %esp
	movl	%eax, -76(%ebp)
	subl	$8, %esp
	pushl	$2
	pushl	-60(%ebp)
	call	fun2
	addl	$16, %esp
	movl	%eax, -76(%ebp)
	subl	$12, %esp
	leal	-76(%ebp), %eax
	pushl	%eax
	call	fun
	addl	$16, %esp
	movl	%eax, -72(%ebp)
	movl	$0, %eax
	movl	-12(%ebp), %ecx
	xorl	%gs:20, %ecx
	je	.L19
	call	__stack_chk_fail_local
.L19:
	leal	-8(%ebp), %esp
	popl	%ecx
	.cfi_restore 1
	.cfi_def_cfa 1, 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	leal	-4(%ecx), %esp
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.text.__x86.get_pc_thunk.ax,"axG",@progbits,__x86.get_pc_thunk.ax,comdat
	.globl	__x86.get_pc_thunk.ax
	.hidden	__x86.get_pc_thunk.ax
	.type	__x86.get_pc_thunk.ax, @function
__x86.get_pc_thunk.ax:
.LFB7:
	.cfi_startproc
	movl	(%esp), %eax
	ret
	.cfi_endproc
.LFE7:
	.section	.text.__x86.get_pc_thunk.bx,"axG",@progbits,__x86.get_pc_thunk.bx,comdat
	.globl	__x86.get_pc_thunk.bx
	.hidden	__x86.get_pc_thunk.bx
	.type	__x86.get_pc_thunk.bx, @function
__x86.get_pc_thunk.bx:
.LFB8:
	.cfi_startproc
	movl	(%esp), %ebx
	ret
	.cfi_endproc
.LFE8:
	.hidden	__stack_chk_fail_local
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
