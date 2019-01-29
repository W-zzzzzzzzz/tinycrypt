	.file	"sm4.c"
	.intel_syntax noprefix
	.text
	.globl	affine
	.type	affine, @function
affine:
.LFB2:
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
.LFE2:
	.size	affine, .-affine
	.globl	SM4
	.type	SM4, @function
SM4:
.LFB3:
	.cfi_startproc
	movzx	edi, dil
	call	affine
	xor	edi, edi
	test	al, al
	mov	r8b, al
	je	.L9
	xor	esi, esi
	mov	dil, 1
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
	jmp	affine
	.cfi_endproc
.LFE3:
	.size	SM4, .-SM4
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
