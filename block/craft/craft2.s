	.file	"craft2.c"
	.intel_syntax noprefix
	.text
	.globl	craft
	.type	craft, @function
craft:
.LFB2:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	push	rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	lea	r10, Q[rip]
	xor	eax, eax
	sub	rsp, 200
	.cfi_def_cfa_offset 224
.L2:
	mov	r8b, BYTE PTR [rdi+rax]
	mov	r9b, BYTE PTR [rsi+rax]
	mov	cl, r8b
	xor	ecx, r9d
	movzx	ecx, cl
	mov	DWORD PTR -56[rsp+rax*4], ecx
	mov	cl, BYTE PTR 16[rdi+rax]
	xor	r9d, ecx
	movzx	r9d, r9b
	mov	DWORD PTR 8[rsp+rax*4], r9d
	movsx	r9, DWORD PTR [r10+rax*4]
	mov	r9b, BYTE PTR [rsi+r9]
	xor	r8d, r9d
	xor	ecx, r9d
	movzx	r8d, r8b
	movzx	ecx, cl
	mov	DWORD PTR 72[rsp+rax*4], r8d
	mov	DWORD PTR 136[rsp+rax*4], ecx
	inc	rax
	cmp	rax, 16
	jne	.L2
	lea	r8, 4[rdx]
	lea	r9, RC4[rip]
	lea	r10, RC3[rip]
	lea	r11, P[rip]
	lea	rbx, S[rip]
	xor	ecx, ecx
.L7:
	mov	esi, ecx
	mov	rax, rdx
.L4:
	mov	dil, BYTE PTR [rax]
	xor	dil, BYTE PTR 8[rax]
	mov	bpl, BYTE PTR 12[rax]
	xor	BYTE PTR 4[rax], bpl
	inc	rax
	xor	edi, ebp
	mov	BYTE PTR -1[rax], dil
	cmp	r8, rax
	jne	.L4
	and	esi, 3
	mov	eax, DWORD PTR [r9+rcx*4]
	xor	BYTE PTR 4[rdx], al
	sal	rsi, 4
	mov	eax, DWORD PTR [r10+rcx*4]
	xor	BYTE PTR 5[rdx], al
	xor	eax, eax
.L5:
	movsx	rdi, eax
	add	rdi, rsi
	mov	edi, DWORD PTR -56[rsp+rdi*4]
	xor	BYTE PTR [rdx+rax], dil
	inc	rax
	cmp	rax, 16
	jne	.L5
	xor	eax, eax
	cmp	rcx, 31
	jne	.L12
.L11:
	inc	rcx
	cmp	rcx, 32
	jne	.L7
	add	rsp, 200
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	pop	rbx
	.cfi_def_cfa_offset 16
	pop	rbp
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	movsx	rsi, DWORD PTR [r11+rax*4]
	movzx	edi, BYTE PTR [rdx+rax]
	inc	rax
	cmp	rax, 16
	mov	DWORD PTR -120[rsp+rsi*4], edi
	jne	.L12
	xor	eax, eax
.L10:
	movsx	rsi, DWORD PTR -120[rsp+rax*4]
	mov	esi, DWORD PTR [rbx+rsi*4]
	mov	BYTE PTR [rdx+rax], sil
	inc	rax
	cmp	rax, 16
	jne	.L10
	jmp	.L11
	.cfi_endproc
.LFE2:
	.size	craft, .-craft
	.globl	RC4
	.section	.rodata
	.align 32
	.type	RC4, @object
	.size	RC4, 128
RC4:
	.long	1
	.long	8
	.long	4
	.long	2
	.long	9
	.long	12
	.long	6
	.long	11
	.long	5
	.long	10
	.long	13
	.long	14
	.long	15
	.long	7
	.long	3
	.long	1
	.long	8
	.long	4
	.long	2
	.long	9
	.long	12
	.long	6
	.long	11
	.long	5
	.long	10
	.long	13
	.long	14
	.long	15
	.long	7
	.long	3
	.long	1
	.long	8
	.globl	RC3
	.align 32
	.type	RC3, @object
	.size	RC3, 128
RC3:
	.long	1
	.long	4
	.long	2
	.long	5
	.long	6
	.long	7
	.long	3
	.long	1
	.long	4
	.long	2
	.long	5
	.long	6
	.long	7
	.long	3
	.long	1
	.long	4
	.long	2
	.long	5
	.long	6
	.long	7
	.long	3
	.long	1
	.long	4
	.long	2
	.long	5
	.long	6
	.long	7
	.long	3
	.long	1
	.long	4
	.long	2
	.long	5
	.globl	Q
	.align 32
	.type	Q, @object
	.size	Q, 64
Q:
	.long	12
	.long	10
	.long	15
	.long	5
	.long	14
	.long	8
	.long	9
	.long	2
	.long	11
	.long	3
	.long	7
	.long	4
	.long	6
	.long	0
	.long	1
	.long	13
	.globl	P
	.align 32
	.type	P, @object
	.size	P, 64
P:
	.long	15
	.long	12
	.long	13
	.long	14
	.long	10
	.long	9
	.long	8
	.long	11
	.long	6
	.long	5
	.long	4
	.long	7
	.long	1
	.long	2
	.long	3
	.long	0
	.globl	S
	.align 32
	.type	S, @object
	.size	S, 64
S:
	.long	12
	.long	10
	.long	13
	.long	3
	.long	14
	.long	11
	.long	15
	.long	7
	.long	8
	.long	9
	.long	1
	.long	5
	.long	0
	.long	2
	.long	4
	.long	6
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
