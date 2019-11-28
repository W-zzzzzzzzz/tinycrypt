
                        public  _u_nrv2b_start
                        public  _u_nrv2b_end
                        public  _u_nrv2d_start
                        public  _u_nrv2d_end
                        public  _u_nrv2e_start
                        public  _u_nrv2e_end

                        .386
                        .model  flat
                        locals  __

                        .code

_u_nrv2b_start:

                push    -1
                pop     ebp

                call    __pop_getbit
                add     ebx, ebx
                jnz     __x1
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__x1:           retn
__pop_getbit:

                ;;

                jmp     __dcl1_n2b
__decompr_literals_n2b:
                movsb
__decompr_loop_n2b:
                add     ebx, ebx
                jnz     __dcl2_n2b
__dcl1_n2b:
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__dcl2_n2b:
                jc      __decompr_literals_n2b

                xor     eax, eax
                inc     eax
__loop1_n2b:
                call    dword ptr [esp] ; getbit
                adc     eax, eax
                call    dword ptr [esp] ; getbit
                jnc     __loop1_n2b
                xor     ecx, ecx
                sub     eax, 3
                jb      __decompr_ebpeax_n2b
                shl     eax, 8
                mov     al, [esi]
                inc     esi
                xor     eax, -1
                jz      __decompr_end_n2b
                mov     ebp, eax
__decompr_ebpeax_n2b:
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                jnz     __decompr_got_mlen_n2b
                inc     ecx
__loop2_n2b:
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                call    dword ptr [esp] ; getbit
                jnc     __loop2_n2b
                inc     ecx
                inc     ecx
__decompr_got_mlen_n2b:
                cmp     ebp, -0D00h
                adc     ecx, 1
                push    esi
                lea     esi, [edi+ebp]
                rep     movsb
                pop     esi
                jmp     __decompr_loop_n2b
__decompr_end_n2b:

                pop     ecx     ; free ptr to getbit

_u_nrv2b_end:
_u_nrv2d_start:

                push    -1
                pop     ebp

                call    __pop_getbit
                add     ebx, ebx
                jnz     __x1
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__x1:           retn
__pop_getbit:

                ;;

                jmp     __dcl1_n2d
__decompr_literals_n2d:
                movsb
__decompr_loop_n2d:
                add     ebx, ebx
                jnz     __dcl2_n2d
__dcl1_n2d:
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__dcl2_n2d:
                jc      __decompr_literals_n2d

                xor     eax, eax
                inc     eax
__loop1_n2d:
                call    dword ptr [esp] ; getbit
                adc     eax, eax
                call    dword ptr [esp] ; getbit
                jc      __loopend1_n2d
                dec     eax
                call    dword ptr [esp] ; getbit
                adc     eax, eax
                jmp     __loop1_n2d
__loopend1_n2d:
                xor     ecx, ecx
                sub     eax, 3
                jb      __decompr_prev_dist_n2d
                shl     eax, 8
                mov     al, [esi]
                inc     esi
                xor     eax, -1
                jz      __decompr_end_n2d
                sar     eax, 1                  ; shift low-bit into carry
                mov     ebp, eax
                jmp     __decompr_ebpeax_n2d
__decompr_prev_dist_n2d:
                call    dword ptr [esp] ; getbit
__decompr_ebpeax_n2d:
                adc     ecx, ecx
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                jnz     __decompr_got_mlen_n2d
                inc     ecx
__loop2_n2d:
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                call    dword ptr [esp] ; getbit
                jnc     __loop2_n2d
                inc     ecx
                inc     ecx
__decompr_got_mlen_n2d:
                cmp     ebp, -500h
                adc     ecx, 1
                push    esi
                lea     esi, [edi+ebp]
                rep
                movsb
                pop     esi
                jmp     __decompr_loop_n2d
__decompr_end_n2d:

                pop     ecx     ; free ptr to getbit

_u_nrv2d_end:
_u_nrv2e_start:

                push    -1
                pop     ebp

                call    __pop_getbit
                add     ebx, ebx
                jnz     __x1
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__x1:           retn
__pop_getbit:

                ;;

                jmp     __dcl1_n2e
__decompr_literals_n2e:
                movsb


__decompr_loop_n2e:
                add     ebx, ebx
                jnz     __dcl2_n2e
__dcl1_n2e:
                mov     ebx, [esi]
                sub     esi, -4
                adc     ebx, ebx
__dcl2_n2e:
                jc      __decompr_literals_n2e
                xor     eax, eax
                inc     eax
__loop1_n2e:
                call    dword ptr [esp] ; getbit
                adc     eax, eax
                call    dword ptr [esp] ; getbit
                jc      __loopend1_n2e
                dec     eax
                call    dword ptr [esp] ; getbit
                adc     eax, eax
                jmp     __loop1_n2e

__decompr_mlen1_n2e:
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                jmp     __decompr_got_mlen_n2e

__loopend1_n2e:
                xor     ecx, ecx
                sub     eax, 3
                jb      __decompr_prev_dist_n2e
                shl     eax, 8
                mov     al, [esi]
                inc     esi
                xor     eax, -1
                jz      __decompr_end_n2e
                sar     eax, 1                  ; shift low-bit into carry
                mov     ebp, eax
                jmp     __decompr_ebpeax_n2e
__decompr_prev_dist_n2e:
                call    dword ptr [esp] ; getbit
__decompr_ebpeax_n2e:
                jc      __decompr_mlen1_n2e
                inc     ecx
                call    dword ptr [esp] ; getbit
                jc      __decompr_mlen1_n2e
__loop2_n2e:
                call    dword ptr [esp] ; getbit
                adc     ecx, ecx
                call    dword ptr [esp] ; getbit
                jnc     __loop2_n2e
                inc     ecx
                inc     ecx
__decompr_got_mlen_n2e:
                cmp     ebp, -500h
                adc     ecx, 2
                push    esi
                lea     esi, [edi+ebp]
                rep
                movsb
                pop     esi
                jmp     __decompr_loop_n2e
__decompr_end_n2e:

                pop     ecx     ; free ptr to getbit

_u_nrv2e_end:

                        end
