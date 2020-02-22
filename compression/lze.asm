;   LZE DECODE ROUTINE
;   Copyright (C)1995,2008 GORRY.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Converted to x86 assembly, by odzhan (22/02/2020, 100 bytes)
;
        bits 32

    %ifndef BIN
      global lze_depack
      global _lze_depack
    %endif
    
; Decode LZE data.
;
; in:  esi = Pointer to data before decoding
; out: edi = Pointer to decoded data
;    
lze_depack:
_lze_depack:
    pushad

    mov    esi, [esp+32+4] ; esi = in
    mov    edi, [esp+32+8] ; edi = out
    
    mov    dh, 1
    call   init_getbit
BitTest:  
    dec    dh
    jnz    lbl1
    mov    dl, [esi]
    inc    esi
    mov    dh, 8
lbl1:
    add    dl, dl
    ret
init_getbit:
    pop    ebp
DECODE_LZE_lp0:
DECODE_LZE_1:
    movsb
DECODE_LZE_lp1:
    call   ebp
    jc     DECODE_LZE_1
DECODE_LZE_0:
    or     eax, -1
    call   ebp
    jc     DECODE_LZE_01
DECODE_LZE_00:
    xor    ecx, ecx
    call   ebp
    adc    ecx, ecx
    call   ebp
    adc    ecx, ecx
    lodsb
DECODE_LZE_00_1:
    inc    ecx
DECODE_LZE_00_2:
    movsx  eax, ax
    push   esi
    lea    esi, [edi+eax]
    inc    ecx
    rep    movsb
    pop    esi
    jmp    DECODE_LZE_lp1
DECODE_LZE_01:
    lodsw
    xchg   al, ah
    mov    ecx, eax
    shr    eax, 3
    or     ah, 0e0h
    and    ecx, 7
    jnz    DECODE_LZE_00_1
    mov    cl, [esi]
    inc    esi
    or     cl, cl
    jnz    DECODE_LZE_00_2
DECODE_LZE_e:
    sub    edi, [esp+32+8]
    mov    [esp+28], edi
    popad
    ret
