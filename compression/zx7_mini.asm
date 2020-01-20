

      bits   32
      
zx7_decompress:
_zx7_decompress:
      pushad
      mov    esi, [esp+32+4]
      mov    edi, [esp+32+8]
      mov    al, 128
      
      call   zx7_init
      add    al, al
      jnz    xit
      lodsb
      adc    al, al
xit:
      ret
zx7_init:
      pop    ebp
copyby:
      movsb
mainlo:
      call   ebp
      jnc    copyby
      push   1
      pop    ecx
      xor    ebx, ebx
lenval:
      call   ebp
      rcl    cl, 1
      jc     exit_decompress
      call   ebp
      jnc    lenval
      mov    dl, [esi]
      mov    dh, bl
      push   esi
      mov    esi, edi
      sbb    esi, edx
      rep    movsb
      pop    esi
      jmp    mainlo
exit_decompress:
      popad
      ret
      