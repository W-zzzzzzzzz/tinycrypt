	.file	"s4.c"
	.intel_syntax noprefix
	.text
	.globl	A
	.type	A, @function
A:
.LFB0:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	xor	eax, eax
	mov	dl, -89
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	esi
	push	ebx
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	mov	bl, BYTE PTR 8[ebp]
.L4:
	mov	cl, dl
	and	ecx, ebx
.L2:
	test	cl, cl
	je	.L8
	mov	esi, ecx
	shr	cl
	and	esi, 1
	xor	eax, esi
	jmp	.L2
.L8:
	rol	dl
	ror	al
	cmp	dl, -89
	jne	.L4
	pop	ebx
	.cfi_restore 3
	xor	eax, -45
	pop	esi
	.cfi_restore 6
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	A, .-A
	.globl	S
	.type	S, @function
S:
.LFB1:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	edi
	push	esi
	movzx	eax, BYTE PTR 8[ebp]
	push	ebx
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	push	eax
	call	A
	pop	edx
	xor	edx, edx
	test	al, al
	je	.L10
	mov	dl, 1
	xor	esi, esi
	xor	ecx, ecx
.L11:
	dec	cl
	je	.L10
	mov	ebx, esi
	test	bl, bl
	jne	.L12
	cmp	dl, al
	jne	.L12
	mov	dl, 1
	mov	esi, 1
.L12:
	mov	bl, dl
	sar	bl, 7
	mov	edi, ebx
	lea	ebx, [edx+edx]
	and	edi, -11
	xor	ebx, edi
	xor	edx, ebx
	jmp	.L11
.L10:
	movzx	edx, dl
	mov	DWORD PTR 8[ebp], edx
	lea	esp, -12[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	edi
	.cfi_restore 7
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	jmp	A
	.cfi_endproc
.LFE1:
	.size	S, .-S
	.section	.rodata
	.align 4
.LC0:
	.long	-1548633402
	.long	1453994832
	.long	1736282519
	.long	-1301273892
	.text
	.globl	sm4
	.type	sm4, @function
sm4:
.LFB2:
	.cfi_startproc
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	call	__x86.get_pc_thunk.cx
	add	ecx, OFFSET FLAT:_GLOBAL_OFFSET_TABLE_
	mov	ebp, esp
	.cfi_def_cfa_register 5
	push	edi
	push	esi
	push	ebx
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	lea	esi, .LC0@GOTOFF[ecx]
	lea	edi, -60[ebp]
	mov	ecx, 4
	sub	esp, 76
	mov	eax, DWORD PTR 8[ebp]
	rep movsd
	lea	edi, -60[ebp]
.L24:
	mov	ebx, DWORD PTR [eax+ecx*4]
	lea	esi, -44[ebp]
	bswap	ebx
	xor	ebx, DWORD PTR [edi+ecx*4]
	mov	DWORD PTR -28[ebp+ecx*4], ebx
	mov	ebx, DWORD PTR 12[ebp]
	mov	ebx, DWORD PTR [ebx+ecx*4]
	bswap	ebx
	mov	DWORD PTR [esi+ecx*4], ebx
	inc	ecx
	cmp	ecx, 4
	jne	.L24
	xor	eax, eax
	mov	DWORD PTR -88[ebp], esi
.L31:
	lea	ebx, 0[0+eax*4]
	xor	ecx, ecx
.L26:
	mov	esi, edx
	lea	edx, [ebx+ecx]
	inc	ecx
	sal	esi, 8
	imul	edx, edx, 7
	movzx	edx, dl
	or	edx, esi
	cmp	ecx, 4
	jne	.L26
	lea	edi, 1[eax]
	mov	DWORD PTR -64[ebp], 1
	mov	ecx, edi
	mov	DWORD PTR -72[ebp], edi
	and	ecx, 3
	lea	edi, 0[0+ecx*4]
	lea	ecx, 8[0+eax*4]
	and	ecx, 12
	mov	DWORD PTR -76[ebp], edi
	mov	edi, DWORD PTR -88[ebp]
	mov	DWORD PTR -80[ebp], ecx
	lea	ecx, 12[0+eax*4]
	and	eax, 3
	sal	eax, 2
	and	ecx, 12
	mov	DWORD PTR -68[ebp], eax
	add	edi, eax
	mov	DWORD PTR -84[ebp], ecx
.L30:
	mov	eax, edi
	sub	eax, DWORD PTR -68[ebp]
	mov	ebx, DWORD PTR -76[ebp]
	mov	esi, 4
	mov	ecx, DWORD PTR 16[eax+ebx]
	mov	ebx, DWORD PTR -80[ebp]
	xor	ecx, DWORD PTR 16[eax+ebx]
	xor	ecx, edx
	mov	edx, DWORD PTR -84[ebp]
	xor	ecx, DWORD PTR 16[eax+edx]
.L27:
	mov	ebx, ecx
	movzx	ecx, cl
	push	ecx
	xor	bl, bl
	call	S
	movzx	eax, al
	or	eax, ebx
	mov	ecx, eax
	ror	ecx, 8
	dec	esi
	pop	edx
	jne	.L27
	cmp	DWORD PTR -64[ebp], 0
	mov	edx, DWORD PTR 16[edi]
	mov	esi, eax
	je	.L28
	rol	esi, 5
	rol	eax, 15
	jmp	.L42
.L28:
	mov	ebx, eax
	ror	esi, 6
	rol	ebx, 2
	xor	ebx, esi
	mov	esi, eax
	rol	eax, 16
	rol	esi, 10
	xor	esi, ebx
.L42:
	xor	eax, esi
	xor	edx, ecx
	dec	DWORD PTR -64[ebp]
	xor	edx, eax
	sub	edi, 16
	mov	DWORD PTR 32[edi], edx
	cmp	DWORD PTR -64[ebp], -1
	jne	.L30
	mov	eax, DWORD PTR -72[ebp]
	cmp	eax, 32
	jne	.L31
	mov	eax, DWORD PTR -44[ebp]
	mov	edx, DWORD PTR -32[ebp]
	mov	DWORD PTR -32[ebp], eax
	mov	DWORD PTR -44[ebp], edx
	mov	eax, DWORD PTR -40[ebp]
	mov	edx, DWORD PTR -36[ebp]
	mov	DWORD PTR -36[ebp], eax
	mov	DWORD PTR -40[ebp], edx
	xor	eax, eax
.L33:
	mov	edx, DWORD PTR -44[ebp+eax*4]
	mov	edi, DWORD PTR 12[ebp]
	bswap	edx
	mov	DWORD PTR [edi+eax*4], edx
	inc	eax
	cmp	eax, 4
	jne	.L33
	lea	esp, -12[ebp]
	pop	ebx
	.cfi_restore 3
	pop	esi
	.cfi_restore 6
	pop	edi
	.cfi_restore 7
	pop	ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	sm4, .-sm4
	.section	.text.__x86.get_pc_thunk.cx,"axG",@progbits,__x86.get_pc_thunk.cx,comdat
	.globl	__x86.get_pc_thunk.cx
	.hidden	__x86.get_pc_thunk.cx
	.type	__x86.get_pc_thunk.cx, @function
__x86.get_pc_thunk.cx:
.LFB3:
	.cfi_startproc
	mov	ecx, DWORD PTR [esp]
	ret
	.cfi_endproc
.LFE3:
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
