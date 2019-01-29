	.file	"kuz.c"
	.intel_syntax noprefix
	.text
	.globl	M
	.type	M, @function
M:
.LFB0:
	.cfi_startproc
	xor	eax, eax
.L2:
	test	sil, sil
	je	.L8
	mov	dl, al
	xor	edx, edi
	test	sil, 1
	cmovne	eax, edx
	mov	dl, dil
	add	edi, edi
	sar	dl, 7
	shr	sil
	and	edx, -61
	xor	edi, edx
	jmp	.L2
.L8:
	ret
	.cfi_endproc
.LFE0:
	.size	M, .-M
	.globl	kuz_lt
	.type	kuz_lt, @function
kuz_lt:
.LFB1:
	.cfi_startproc
	sub	rsp, 16
	.cfi_def_cfa_offset 24
	mov	r8, rdi
	mov	r9b, 16
	mov	BYTE PTR [rsp], -108
	mov	BYTE PTR 1[rsp], 32
	mov	r11, rsp
	mov	BYTE PTR 2[rsp], -123
	mov	BYTE PTR 3[rsp], 16
	mov	BYTE PTR 4[rsp], -62
	mov	BYTE PTR 5[rsp], -64
	mov	BYTE PTR 6[rsp], 1
	mov	BYTE PTR 7[rsp], -5
	mov	BYTE PTR 8[rsp], 1
	mov	BYTE PTR 9[rsp], -64
	mov	BYTE PTR 10[rsp], -62
	mov	BYTE PTR 11[rsp], 16
	mov	BYTE PTR 12[rsp], -123
	mov	BYTE PTR 13[rsp], 32
	mov	BYTE PTR 14[rsp], -108
	mov	BYTE PTR 15[rsp], 1
.L11:
	mov	r10b, BYTE PTR 15[r8]
	mov	ecx, 14
.L10:
	movzx	edi, BYTE PTR [r8+rcx]
	movzx	esi, BYTE PTR [rcx+r11]
	mov	BYTE PTR 1[r8+rcx], dil
	call	M
	dec	rcx
	xor	r10d, eax
	cmp	rcx, -1
	jne	.L10
	dec	r9b
	mov	BYTE PTR [r8], r10b
	jne	.L11
	add	rsp, 16
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	kuz_lt, .-kuz_lt
	.globl	kuz_subbytes
	.type	kuz_subbytes, @function
kuz_subbytes:
.LFB2:
	.cfi_startproc
	lea	rcx, S[rip]
	xor	eax, eax
.L16:
	movzx	edx, BYTE PTR [rdi+rax]
	mov	dl, BYTE PTR [rcx+rdx]
	mov	BYTE PTR [rdi+rax], dl
	inc	rax
	cmp	rax, 16
	jne	.L16
	ret
	.cfi_endproc
.LFE2:
	.size	kuz_subbytes, .-kuz_subbytes
	.globl	kuznyechik
	.type	kuznyechik, @function
kuznyechik:
.LFB3:
	.cfi_startproc
	push	r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	push	r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	xor	eax, eax
	push	r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	push	r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	mov	r13, rsi
	push	rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	push	rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	sub	rsp, 56
	.cfi_def_cfa_offset 112
	lea	rbp, 24[rsp]
.L19:
	mov	dl, BYTE PTR [rdi+rax]
	inc	rax
	mov	BYTE PTR -1[rax+rbp], dl
	cmp	rax, 32
	jne	.L19
	lea	rbx, 8[rsp]
	xor	r12d, r12d
	xor	r14d, r14d
.L28:
	test	r12b, 7
	jne	.L20
	lea	rax, 56[rsp]
	mov	r15, rbp
	mov	QWORD PTR [rsp], rax
.L21:
	xor	eax, eax
.L22:
	mov	cl, BYTE PTR [r15+rax]
	xor	BYTE PTR 0[r13+rax], cl
	inc	rax
	cmp	rax, 16
	jne	.L22
	inc	r14d
	cmp	r14b, 10
	je	.L18
	mov	rdi, r13
	add	r15, 16
	call	kuz_subbytes
	mov	rdi, r13
	call	kuz_lt
	cmp	QWORD PTR [rsp], r15
	jne	.L21
.L20:
	xor	eax, eax
.L24:
	mov	BYTE PTR [rax+rbx], 0
	inc	rax
	cmp	rax, 16
	jne	.L24
	inc	r12d
	mov	rdi, rbx
	mov	BYTE PTR 23[rsp], r12b
	call	kuz_lt
	xor	eax, eax
.L25:
	mov	dl, BYTE PTR [rbx+rax]
	xor	dl, BYTE PTR 0[rbp+rax]
	mov	BYTE PTR [rbx+rax], dl
	inc	rax
	cmp	rax, 16
	jne	.L25
	mov	rdi, rbx
	call	kuz_subbytes
	mov	rdi, rbx
	call	kuz_lt
	lea	rcx, 16[rbp]
	xor	eax, eax
.L26:
	mov	dl, BYTE PTR [rbx+rax]
	xor	dl, BYTE PTR [rcx+rax]
	mov	BYTE PTR [rbx+rax], dl
	inc	rax
	cmp	rax, 16
	jne	.L26
	lea	rcx, 16[rbp]
	xor	eax, eax
.L27:
	mov	dl, BYTE PTR 0[rbp+rax]
	mov	BYTE PTR [rcx+rax], dl
	mov	dl, BYTE PTR [rbx+rax]
	mov	BYTE PTR 0[rbp+rax], dl
	inc	rax
	cmp	rax, 16
	jne	.L27
	jmp	.L28
.L18:
	add	rsp, 56
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
.LFE3:
	.size	kuznyechik, .-kuznyechik
	.globl	S
	.data
	.align 32
	.type	S, @object
	.size	S, 256
S:
	.byte	-4
	.byte	-18
	.byte	-35
	.byte	17
	.byte	-49
	.byte	110
	.byte	49
	.byte	22
	.byte	-5
	.byte	-60
	.byte	-6
	.byte	-38
	.byte	35
	.byte	-59
	.byte	4
	.byte	77
	.byte	-23
	.byte	119
	.byte	-16
	.byte	-37
	.byte	-109
	.byte	46
	.byte	-103
	.byte	-70
	.byte	23
	.byte	54
	.byte	-15
	.byte	-69
	.byte	20
	.byte	-51
	.byte	95
	.byte	-63
	.byte	-7
	.byte	24
	.byte	101
	.byte	90
	.byte	-30
	.byte	92
	.byte	-17
	.byte	33
	.byte	-127
	.byte	28
	.byte	60
	.byte	66
	.byte	-117
	.byte	1
	.byte	-114
	.byte	79
	.byte	5
	.byte	-124
	.byte	2
	.byte	-82
	.byte	-29
	.byte	106
	.byte	-113
	.byte	-96
	.byte	6
	.byte	11
	.byte	-19
	.byte	-104
	.byte	127
	.byte	-44
	.byte	-45
	.byte	31
	.byte	-21
	.byte	52
	.byte	44
	.byte	81
	.byte	-22
	.byte	-56
	.byte	72
	.byte	-85
	.byte	-14
	.byte	42
	.byte	104
	.byte	-94
	.byte	-3
	.byte	58
	.byte	-50
	.byte	-52
	.byte	-75
	.byte	112
	.byte	14
	.byte	86
	.byte	8
	.byte	12
	.byte	118
	.byte	18
	.byte	-65
	.byte	114
	.byte	19
	.byte	71
	.byte	-100
	.byte	-73
	.byte	93
	.byte	-121
	.byte	21
	.byte	-95
	.byte	-106
	.byte	41
	.byte	16
	.byte	123
	.byte	-102
	.byte	-57
	.byte	-13
	.byte	-111
	.byte	120
	.byte	111
	.byte	-99
	.byte	-98
	.byte	-78
	.byte	-79
	.byte	50
	.byte	117
	.byte	25
	.byte	61
	.byte	-1
	.byte	53
	.byte	-118
	.byte	126
	.byte	109
	.byte	84
	.byte	-58
	.byte	-128
	.byte	-61
	.byte	-67
	.byte	13
	.byte	87
	.byte	-33
	.byte	-11
	.byte	36
	.byte	-87
	.byte	62
	.byte	-88
	.byte	67
	.byte	-55
	.byte	-41
	.byte	121
	.byte	-42
	.byte	-10
	.byte	124
	.byte	34
	.byte	-71
	.byte	3
	.byte	-32
	.byte	15
	.byte	-20
	.byte	-34
	.byte	122
	.byte	-108
	.byte	-80
	.byte	-68
	.byte	-36
	.byte	-24
	.byte	40
	.byte	80
	.byte	78
	.byte	51
	.byte	10
	.byte	74
	.byte	-89
	.byte	-105
	.byte	96
	.byte	115
	.byte	30
	.byte	0
	.byte	98
	.byte	68
	.byte	26
	.byte	-72
	.byte	56
	.byte	-126
	.byte	100
	.byte	-97
	.byte	38
	.byte	65
	.byte	-83
	.byte	69
	.byte	70
	.byte	-110
	.byte	39
	.byte	94
	.byte	85
	.byte	47
	.byte	-116
	.byte	-93
	.byte	-91
	.byte	125
	.byte	105
	.byte	-43
	.byte	-107
	.byte	59
	.byte	7
	.byte	88
	.byte	-77
	.byte	64
	.byte	-122
	.byte	-84
	.byte	29
	.byte	-9
	.byte	48
	.byte	55
	.byte	107
	.byte	-28
	.byte	-120
	.byte	-39
	.byte	-25
	.byte	-119
	.byte	-31
	.byte	27
	.byte	-125
	.byte	73
	.byte	76
	.byte	63
	.byte	-8
	.byte	-2
	.byte	-115
	.byte	83
	.byte	-86
	.byte	-112
	.byte	-54
	.byte	-40
	.byte	-123
	.byte	97
	.byte	32
	.byte	113
	.byte	103
	.byte	-92
	.byte	45
	.byte	43
	.byte	9
	.byte	91
	.byte	-53
	.byte	-101
	.byte	37
	.byte	-48
	.byte	-66
	.byte	-27
	.byte	108
	.byte	82
	.byte	89
	.byte	-90
	.byte	116
	.byte	-46
	.byte	-26
	.byte	-12
	.byte	-76
	.byte	-64
	.byte	-47
	.byte	102
	.byte	-81
	.byte	-62
	.byte	57
	.byte	75
	.byte	99
	.byte	-74
	.ident	"GCC: (Debian 6.3.0-18+deb9u1) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
