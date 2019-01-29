	.file	"s4.c"
	.intel_syntax noprefix
	.text
	.globl	AX
	.type	AX, @function
AX:
.LFB0:
	.cfi_startproc
	xor	eax, eax
	mov	dl, -89
.L4:
	mov	cl, dl
	and	ecx, edi
.L2:
	test	cl, cl
	je	.L7
	mov	sil, cl
	shr	cl
	and	esi, 1
	xor	eax, esi
	jmp	.L2
.L7:
	rol	dl
	ror	al
	cmp	dl, -89
	jne	.L4
	xor	eax, -45
	ret
	.cfi_endproc
.LFE0:
	.size	AX, .-AX
	.globl	S
	.type	S, @function
S:
.LFB1:
	.cfi_startproc
	movzx	edi, dil
	call	AX
	xor	edi, edi
	test	al, al
	mov	r8b, al
	je	.L9
	mov	dil, 1
	xor	esi, esi
	xor	edx, edx
	mov	r9b, -11
.L10:
	dec	dl
	je	.L9
	test	sil, sil
	jne	.L11
	cmp	dil, r8b
	jne	.L11
	mov	dil, 1
	mov	sil, 1
.L11:
	mov	al, dil
	lea	ecx, [rdi+rdi]
	shr	al, 7
	imul	eax, r9d
	xor	eax, ecx
	xor	edi, eax
	jmp	.L10
.L9:
	movzx	edi, dil
	jmp	AX
	.cfi_endproc
.LFE1:
	.size	S, .-S
	.globl	sm4
	.type	sm4, @function
sm4:
.LFB2:
	.cfi_startproc
	push	r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	push	r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	xor	ecx, ecx
	push	r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	push	r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	push	rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	push	rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	sub	rsp, 72
	.cfi_def_cfa_offset 128
	lea	rdx, 24[rsp]
	lea	r9, 56[rsp]
	lea	r13, 40[rsp]
	mov	DWORD PTR 56[rsp], -1548633402
	mov	DWORD PTR 60[rsp], 1453994832
	mov	DWORD PTR 64[rsp], 1736282519
	mov	DWORD PTR 68[rsp], -1301273892
.L21:
	mov	r8d, DWORD PTR [rdi+rcx]
	bswap	r8d
	xor	r8d, DWORD PTR [rcx+r9]
	mov	DWORD PTR [rcx+rdx], r8d
	mov	r8d, DWORD PTR [rsi+rcx]
	add	rcx, 4
	bswap	r8d
	mov	DWORD PTR -4[rcx+r13], r8d
	cmp	rcx, 16
	jne	.L21
	xor	r10d, r10d
.L28:
	lea	r9d, 0[0+r10*4]
	xor	r8d, r8d
.L23:
	lea	ecx, [r8+r9]
	sal	eax, 8
	inc	r8d
	imul	ecx, ecx, 7
	movzx	ecx, cl
	or	eax, ecx
	cmp	r8d, 4
	jne	.L23
	lea	ecx, 1[r10]
	lea	ebx, 2[r10]
	lea	ebp, 3[r10]
	and	r10d, 3
	xor	r12d, r12d
	mov	r11, rdx
	mov	r14d, ecx
	and	ebx, 3
	and	ebp, 3
	and	r14d, 3
	sal	rbx, 2
	sal	rbp, 2
	sal	r14, 2
	sal	r10, 2
.L27:
	mov	edi, DWORD PTR [r11+r14]
	xor	edi, DWORD PTR [r11+rbx]
	mov	r15d, 4
	xor	eax, edi
	xor	eax, DWORD PTR [r11+rbp]
	mov	edi, eax
.L24:
	mov	eax, edi
	movzx	edi, dil
	mov	QWORD PTR 16[rsp], rdx
	xor	al, al
	mov	QWORD PTR 8[rsp], rsi
	mov	DWORD PTR 4[rsp], ecx
	mov	DWORD PTR [rsp], eax
	call	S
	movzx	eax, al
	or	eax, DWORD PTR [rsp]
	mov	ecx, DWORD PTR 4[rsp]
	mov	rsi, QWORD PTR 8[rsp]
	mov	rdx, QWORD PTR 16[rsp]
	mov	edi, eax
	ror	edi, 8
	dec	r15d
	jne	.L24
	add	r11, r10
	test	r12d, r12d
	mov	r8d, eax
	mov	r9d, DWORD PTR [r11]
	je	.L25
	mov	r15d, eax
	ror	r8d, 6
	rol	r15d, 2
	xor	r8d, r15d
	mov	r15d, eax
	rol	eax, 16
	rol	r15d, 10
	xor	r8d, r15d
	jmp	.L39
.L25:
	rol	r8d, 5
	rol	eax, 15
.L39:
	xor	eax, r8d
	xor	edi, r9d
	inc	r12d
	xor	eax, edi
	cmp	r12d, 2
	mov	DWORD PTR [r11], eax
	mov	r11, r13
	jne	.L27
	cmp	ecx, 32
	mov	r10d, ecx
	jne	.L28
	mov	eax, DWORD PTR 40[rsp]
	mov	edx, DWORD PTR 52[rsp]
	mov	DWORD PTR 52[rsp], eax
	mov	DWORD PTR 40[rsp], edx
	mov	eax, DWORD PTR 44[rsp]
	mov	edx, DWORD PTR 48[rsp]
	mov	DWORD PTR 48[rsp], eax
	mov	DWORD PTR 44[rsp], edx
	xor	eax, eax
.L30:
	mov	edx, DWORD PTR 0[r13+rax]
	bswap	edx
	mov	DWORD PTR [rsi+rax], edx
	add	rax, 4
	cmp	rax, 16
	jne	.L30
	add	rsp, 72
	.cfi_def_cfa_offset 56
	pop	rbx
	.cfi_def_cfa_offset 48
	pop	rbp
	.cfi_def_cfa_offset 40
	pop	r12
	.cfi_def_cfa_offset 32
	pop	r13
	.cfi_def_cfa_offset 24
	pop	r14
	.cfi_def_cfa_offset 16
	pop	r15
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2:
	.size	sm4, .-sm4
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
