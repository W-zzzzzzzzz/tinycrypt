;
; Decompresses Y. Collet's LZ4 compressed stream data in 16-bit real mode.
; Optimized for 8088/8086 CPUs.
; Code by Trixter/Hornet (trixter@oldskool.org) on 20130105
; Updated 2019/06/17 -- thanks to Peter Ferrie, Terje Mathsen,
; and Axel Kern for suggestions and improvements!
; Updated 2019/06/30: Fixed an alignment bug in lz4_decompress_small
;
;---------------------------------------------------------------
;
; function lz4_decompress_small(inb,outb:pointer):word; assembler;
;
; Same as LZ4_Decompress but optimized for size, not speed. Still pretty fast,
; although roughly 30% slower than lz4_decompress and RLE sequences are not
; optimally handled.  Same Input, Output, and Trashes as lz4_decompress.
; Minus the Turbo Pascal preamble/postamble, assembles to 78 bytes.
;---------------------------------------------------------------
;
; Updated 2020/01/09: Converted to 32-bit, assembles to 81 bytes.
; Uses cdecl calling convention: uint32_t lz4_decompress(void *outbuf, uint32_t inlen, void *inbuf);
;
; yasm -fwin32 lz4.asm -olz4.obj
;
; Odzhan
;
        bits    32
        
        %ifndef BIN
          global lz4_decompress
          global _lz4_decompress
        %endif
        
lz4_decompress:
_lz4_decompress:
        pushad
        
        lea     esi, [esp+32+4]
        lodsd
        xchg    eax, edi        ; EDI = output
        lodsd
        xchg    eax, ebx        ; EBX = chunk size
        lodsd
        xchg    eax, esi        ; ESI = input / start of chunk
        
        add     ebx, esi        ; EBX = threshold to stop decompression
        xor     eax, eax
@@parsetoken:                   ; ECX = 0 here because of REP at end of loop
        lodsb                   ; grab token to AL
        movzx   edx, al         ; preserve packed token in DL
        push    4
        pop     ecx
        shr     al, cl          ; unpack upper 4 bits
        call    buildfullcount  ; build full literal count if necessary
        rep     movsb           ; if ECX = 0 nothing happens

;At this point, we might be done; all LZ4 data ends with five literals and the
;offset token is ignored.  If we're at the end of our compressed chunk, stop.

        cmp     esi, ebx        ; are we at the end of our compressed chunk?
        jae     @@done          ; if so, jump to exit; otherwise, process match
@@copymatches:
        lodsw                   ; AX = match offset
        xchg    edx, eax        ; AX = packed token, DX = match offset
        and     al, 0Fh         ; unpack match length token
        call    buildfullcount  ; build full match count if necessary
@@domatchcopy:
        push    esi             ; xchg with ax would destroy ah
        mov     esi, edi
        sub     esi, edx
        add     ecx, 4          ; minmatch = 4
        rep     movsb           ; copy match run if any left
        pop     esi
        jmp     @@parsetoken

buildfullcount:
                                ; CH has to be 0 here to ensure AH remains 0
        cmp     al, 0Fh         ; test if unpacked literal length token is 15?
        xchg    ecx, eax        ; ECX = unpacked literal length token; flags unchanged
        jne     builddone       ; if AL was not 15, we have nothing to build
buildloop:
        lodsb                   ; load a byte
        add     ecx, eax        ; add it to the full count
        cmp     al, 0FFh        ; was it FF?
        je      buildloop       ; if so, keep going
builddone:
        ret
_lz4_decompress@@done:
        sub     edi, [esp+32+4] ; subtract original offset from where we are now
        mov     [esp+28], edi   ; EAX = decompressed size
        popad
        ret
