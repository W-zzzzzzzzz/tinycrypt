; Listing generated by Microsoft (R) Optimizing Compiler Version 19.10.25019.0 

	TITLE	c:\hub\chacha\cx.c
	.686P
	.XMM
	include listing.inc
	.model	flat

INCLUDELIB LIBCMT
INCLUDELIB OLDNAMES

PUBLIC	_xchacha_permute
PUBLIC	_chacha20
; Function compile flags: /Ogspy
; File c:\hub\chacha\cx.c
;	COMDAT _chacha20
_TEXT	SEGMENT
_c$ = -64						; size = 64
_len$ = 8						; size = 4
_in$ = 12						; size = 4
_s$ = 16						; size = 4
_chacha20 PROC						; COMDAT

; 95   : {

	sub	esp, 64					; 00000040H
	push	ebx

; 96   :     uint8_t *p=(uint8_t*)in;

	mov	ebx, DWORD PTR _in$[esp+64]
	push	esi

; 97   :     int     r, i;
; 98   :     w512_t  c;
; 99   :     
; 100  :     // if we have data
; 101  :     if (len != 0) {

	mov	esi, DWORD PTR _len$[esp+68]
	push	edi
	test	esi, esi
	je	SHORT $LN7@chacha20
	push	ebp
	push	64					; 00000040H
	pop	ebp
$LL2@chacha20:

; 102  :       while (len) {   
; 103  :         // permutate stream      
; 104  :         xchacha_permute(s, &c);

	lea	eax, DWORD PTR _c$[esp+80]
	push	eax
	push	DWORD PTR _s$[esp+80]
	call	_xchacha_permute
	pop	ecx
	pop	ecx

; 105  :         
; 106  :         // xor full block or whatever remains
; 107  :         r=(len>64) ? 64 : len;

	cmp	esi, ebp
	mov	ecx, esi
	cmova	ecx, ebp

; 108  :         
; 109  :         // xor input with stream
; 110  :         for (i=0; i<r; i++) {

	test	ecx, ecx
	jle	SHORT $LN5@chacha20
	lea	edi, DWORD PTR _c$[esp+80]
	mov	edx, ebx
	sub	edi, ebx
	mov	ebp, ecx
$LL14@chacha20:

; 111  :           p[i] ^= c.b[i];

	mov	al, BYTE PTR [edi+edx]
	xor	BYTE PTR [edx], al
	inc	edx
	sub	ebp, 1
	jne	SHORT $LL14@chacha20
	push	64					; 00000040H
	pop	ebp
$LN5@chacha20:

; 112  :         }      
; 113  :         len -= r;
; 114  :         p += r;

	add	ebx, ecx
	sub	esi, ecx
	jne	SHORT $LL2@chacha20
	pop	ebp

; 115  :       }
; 116  :     } else {  

	jmp	SHORT $LN8@chacha20
$LN7@chacha20:

; 117  :       // initialize state with key, nonce, counter
; 118  :       // store "expand 32-byte k"
; 119  :       s->w[0] = 0x61707865; s->w[1] = 0x3320646E;

	mov	edx, DWORD PTR _s$[esp+72]

; 120  :       s->w[2] = 0x79622D32; s->w[3] = 0x6B206574;
; 121  :       
; 122  :       // store 256-bit key
; 123  :       memcpy (&s->b[16], p, 32);      

	mov	esi, ebx
	push	8
	pop	ecx
	mov	DWORD PTR [edx], 1634760805		; 61707865H
	lea	edi, DWORD PTR [edx+16]
	mov	DWORD PTR [edx+4], 857760878		; 3320646eH
	mov	DWORD PTR [edx+8], 2036477234		; 79622d32H
	mov	DWORD PTR [edx+12], 1797285236		; 6b206574H
	rep movsd

; 124  :       
; 125  :       // initialize 64-bit counter
; 126  :       s->w[12] = 0; s->w[13] = 0;

	and	DWORD PTR [edx+48], 0
	and	DWORD PTR [edx+52], 0

; 127  :       
; 128  :       // store 64-bit nonce
; 129  :       memcpy (&s->b[56], &p[32], 8);   

	mov	eax, DWORD PTR [ebx+32]
	mov	DWORD PTR [edx+56], eax
	mov	eax, DWORD PTR [ebx+36]
	mov	DWORD PTR [edx+60], eax
$LN8@chacha20:
	pop	edi
	pop	esi
	pop	ebx

; 130  :     }
; 131  : }

	add	esp, 64					; 00000040H
	ret	0
_chacha20 ENDP
_TEXT	ENDS
; Function compile flags: /Ogspy
; File c:\hub\chacha\cx.c
;	COMDAT _xchacha_permute
_TEXT	SEGMENT
tv467 = -28						; size = 4
_j$1$ = -24						; size = 4
tv462 = -20						; size = 4
_idx16$ = -16						; size = 16
_state$ = 8						; size = 4
_r$1$ = 12						; size = 4
_out$ = 12						; size = 4
_xchacha_permute PROC					; COMDAT

; 34   : {

	sub	esp, 28					; 0000001cH

; 35   :     int      i, j, k, idx;
; 36   :     uint32_t *x;
; 37   :     uint32_t a, b, c, d, r, t;
; 38   :     
; 39   :     // 16-bit integers of each index
; 40   :     uint16_t idx16[8]=
; 41   :     { 0xC840, 0xD951, 0xEA62, 0xFB73, 
; 42   :       0xFA50, 0xCB61, 0xD872, 0xE943 };
; 43   :     
; 44   :     // copy state to out
; 45   :     memcpy (out->b, state->b, 64);

	mov	edx, DWORD PTR _out$[esp+24]
	push	ebx
	push	esi
	mov	esi, DWORD PTR _state$[esp+32]
	push	edi
	push	16					; 00000010H
	pop	ecx
	mov	edi, edx
	mov	DWORD PTR _idx16$[esp+40], -648951744	; d951c840H
	mov	DWORD PTR _idx16$[esp+44], -76289438	; fb73ea62H
	mov	DWORD PTR _idx16$[esp+48], -882771376	; cb61fa50H
	mov	DWORD PTR _idx16$[esp+52], -381429646	; e943d872H
	rep movsd
	mov	DWORD PTR tv462[esp+40], 10		; 0000000aH
	push	ebp
$LL4@xchacha_pe:

; 51   :       for (j=0; j<8; j++) {

	xor	esi, esi
	mov	DWORD PTR _j$1$[esp+44], esi
$LL7@xchacha_pe:

; 52   :         idx = idx16[j];

	movzx	edi, WORD PTR _idx16$[esp+esi*2+44]

; 53   :         
; 54   :         a = ((idx      ) & 0xF);
; 55   :         b = ((idx >>  4) & 0xF);
; 56   :         c = ((idx >>  8) & 0xF);
; 57   :         d = ((idx >> 12) & 0xF);
; 58   :         
; 59   :         r = 0x07080C10;

	mov	ecx, 117967888				; 07080c10H
	mov	ebp, edi
	mov	DWORD PTR _r$1$[esp+40], ecx
	mov	ebx, edi
	shr	ebp, 4
	mov	esi, edi
	shr	ebx, 8
	and	esi, 15					; 0000000fH
	and	ebp, 15					; 0000000fH
	and	ebx, 15					; 0000000fH
	shr	edi, 12					; 0000000cH
$LL10@xchacha_pe:

; 60   :         
; 61   :         do {
; 62   :           for (k=0; k<2; k++) {

	mov	DWORD PTR tv467[esp+44], 2
$LL13@xchacha_pe:

; 63   :             x[a]+= x[b]; 

	mov	eax, DWORD PTR [edx+ebp*4]
	add	DWORD PTR [edx+esi*4], eax

; 64   :             x[d] = ROTL32(x[d] ^ x[a], (r & 0xFF));

	mov	eax, DWORD PTR [edx+edi*4]
	xor	eax, DWORD PTR [edx+esi*4]
	movzx	ecx, cl
	rol	eax, cl

; 65   :             XCHG(a, c);
; 66   :             XCHG(b, d);
; 67   :             r >>= 8;            

	mov	ecx, DWORD PTR _r$1$[esp+40]
	mov	DWORD PTR [edx+edi*4], eax
	mov	eax, esi
	mov	esi, ebx
	shr	ecx, 8
	sub	DWORD PTR tv467[esp+44], 1
	mov	ebx, eax
	mov	eax, ebp
	mov	DWORD PTR _r$1$[esp+40], ecx
	mov	ebp, edi
	mov	edi, eax
	jne	SHORT $LL13@xchacha_pe

; 68   :           }
; 69   :         } while (r != 0);

	test	ecx, ecx
	jne	SHORT $LL10@xchacha_pe

; 51   :       for (j=0; j<8; j++) {

	mov	esi, DWORD PTR _j$1$[esp+44]
	inc	esi
	mov	DWORD PTR _j$1$[esp+44], esi
	cmp	esi, 8
	jl	SHORT $LL7@xchacha_pe

; 46   : 
; 47   :     x = out->w;
; 48   :     
; 49   :     // apply 20 rounds
; 50   :     for (i=0; i<20; i+=2) {

	sub	DWORD PTR tv462[esp+44], 1
	jne	SHORT $LL4@xchacha_pe

; 70   :         /**
; 71   :         x[a]+= x[b]; 
; 72   :         x[d] = ROTL32(x[d] ^ x[a],16);
; 73   :         
; 74   :         x[c]+= x[d]; 
; 75   :         x[b] = ROTL32(x[b] ^ x[c],12);        
; 76   : 
; 77   :         x[a]+= x[b]; 
; 78   :         x[d] = ROTL32(x[d] ^ x[a], 8);        
; 79   : 
; 80   :         x[c]+= x[d]; 
; 81   :         x[b] = ROTL32(x[b] ^ x[c], 7);*/
; 82   :       }
; 83   :     }
; 84   :     // add state to out
; 85   :     for (i=0; i<16; i++) {

	mov	ebx, DWORD PTR _state$[esp+40]
	mov	ecx, ebx
	push	16					; 00000010H
	pop	esi
	sub	ecx, edx
	pop	ebp
$LL27@xchacha_pe:

; 86   :       out->w[i] += state->w[i];

	mov	eax, DWORD PTR [ecx+edx]
	add	DWORD PTR [edx], eax
	lea	edx, DWORD PTR [edx+4]
	sub	esi, 1
	jne	SHORT $LL27@xchacha_pe

; 87   :     }
; 88   :     // update counter
; 89   :     state->q[6]++;

	add	DWORD PTR [ebx+48], 1
	pop	edi
	adc	DWORD PTR [ebx+52], esi
	pop	esi
	pop	ebx

; 90   :     // stopping at 2^70 bytes per nonce is user's responsibility
; 91   : }

	add	esp, 28					; 0000001cH
	ret	0
_xchacha_permute ENDP
_TEXT	ENDS
END
