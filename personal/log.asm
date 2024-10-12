	.file	"main.cpp"
	.text
	.p2align 4,,15
	.globl	_Z6runLogv
	.type	_Z6runLogv, @function
_Z6runLogv:
.LFB8296:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	xorl	%ebp, %ebp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	xorl	%ebx, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L2:
	pxor	%xmm0, %xmm0
	cvtsi2sd	%ebp, %xmm0
	addl	$1, %ebp
	call	log
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rbx, %xmm1
	addsd	%xmm0, %xmm1
	cvttsd2siq	%xmm1, %rbx
	cmpl	$100000000, %ebp
	jne	.L2
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movq	%rbx, %rax
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE8296:
	.size	_Z6runLogv, .-_Z6runLogv
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB8297:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	_Z6runLogv
	movl	$_ZSt4cout, %edi
	movq	%rax, %rsi
	call	_ZNSo9_M_insertIxEERSoT_
	movq	%rax, %rdi
	call	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE8297:
	.size	main, .-main
	.p2align 4,,15
	.type	_GLOBAL__sub_I__Z6runLogv, @function
_GLOBAL__sub_I__Z6runLogv:
.LFB9565:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE9565:
	.size	_GLOBAL__sub_I__Z6runLogv, .-_GLOBAL__sub_I__Z6runLogv
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z6runLogv
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.ident	"GCC: (GNU) 8.4.0"
	.section	.note.GNU-stack,"",@progbits
