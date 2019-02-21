

// RC5 test in C
// Odzhan

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "rc5.h"

void bin2hex(const char *s, void *bin, int len) {
    int  i;
    char x;
    
    printf ("\n%s=", s);
    
    for (i=0; i<len; i++) {
      if ((i & 7)==0) putchar('\n');
      if (i==len-1) x='\n'; else x=',';
      printf ("0x%02x%c ", ((uint8_t*)bin)[i], x);
    }
}

// 128-bit key
unsigned char tv_key[16]={
  0x1b,0xec,0x52,0xf7,0xfc,0xcc,0x95,0x24,
  0x49,0x3d,0x8f,0xae,0x11,0x7a,0x0b,0xc8 };
  
// 64-bit plaintext
unsigned char tv_plaintext[8]={
  0x4d,0xbf,0x44,0xc6,0xb1,0xbe,0x73,0x6e };
  
// 64-bit ciphertext
uint8_t tv_ciphertext[8]={
  0x02, 0xb5, 0xd6, 0x01, 0x24, 0x1f, 0xc6, 0x2b };
  
int main(void) {
    int     equ;
    uint8_t buf[8];
    
    memcpy(buf, tv_plaintext, 8);
    rc5(tv_key, buf);
    equ = (memcmp(tv_ciphertext, buf, 8)==0);
    printf("RC5 test : %s\n", equ ? "OK" : "FAILED");
    
    return 0;
}
