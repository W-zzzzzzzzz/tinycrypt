	.file	"tz.c"
	.intel_syntax noprefix
	.text
	.globl	threefish
	.type	threefish, @function
threefish:
.LFB0:
	.cfi_startproc
	push	rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movabs	rax, 2322219242857378830
	mov	QWORD PTR -80[rsp], rax
	movabs	rax, 2312048795198175504
	mov	QWORD PTR -72[rsp], rax
	movabs	rax, 2004413935125273122
	mov	QWORD PTR -32[rsp], rax
	xor	eax, eax
.L3:
	mov	rdx, QWORD PTR [rdi+rax*8]
	mov	QWORD PTR -64[rsp+rax*8], rdx
	xor	QWORD PTR -32[rsp], rdx
	cmp	rax, 1
	ja	.L2
	mov	rdx, QWORD PTR 32[rdi+rax*8]
	mov	QWORD PTR -24[rsp+rax*8], rdx
.L2:
	inc	rax
	cmp	rax, 4
	jne	.L3
  
	mov	rax, QWORD PTR -24[rsp]
	xor	rax, QWORD PTR -16[rsp]
	xor	edi, edi
	mov	r9d, 5
	mov	r10d, 3
	mov	QWORD PTR -8[rsp], rax
.L10:
	mov	r11, rdi
	and	r11d, 3
	je	.L4
.L9:
	cmp	rdi, 72
	je	.L17
  
	mov	rdx, rdi
	mov	rax, QWORD PTR 8[rsi]
	inc	rdi
	and	edx, 7
	movzx	ecx, BYTE PTR -72[rsp+rdx]
	movzx	ebx, BYTE PTR -80[rsp+rdx]
	mov	rdx, QWORD PTR 24[rsi]
	mov	r8, rax
	add	r8, QWORD PTR [rsi]
	mov	r11, rdx
	add	r11, QWORD PTR 16[rsi]
	rol	rdx, cl
	mov	cl, bl
	mov	QWORD PTR [rsi], r8
	rol	rax, cl
	xor	rax, r8
	xor	rdx, r11
	mov	QWORD PTR 16[rsi], r11
	mov	QWORD PTR 24[rsi], rax
	mov	QWORD PTR 8[rsi], rdx
	jmp	.L10
.L4:
	mov	rbx, rdi
	xor	ecx, ecx
	shr	rbx, 2
.L8:
	lea	r8, [rbx+rcx]
	xor	edx, edx
	mov	rax, r8
	div	r9
	mov	rax, QWORD PTR [rsi+rcx*8]
	add	rax, QWORD PTR -64[rsp+rdx*8]
	add	rax, r11
	cmp	rcx, 1
	mov	r11, rbx
	mov	QWORD PTR [rsi+rcx*8], rax
	ja	.L7
  
	mov	rax, r8
	xor	edx, edx
	div	r10
	mov	r11, QWORD PTR -24[rsp+rdx*8]
.L7:
	inc	rcx
	cmp	rcx, 4
	jne	.L8
  
	jmp	.L9
.L17:
	pop	rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	threefish, .-threefish
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
