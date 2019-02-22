
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// 128-bit master key
uint8_t key[16]=
{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
  
// 128-bit plain text
uint8_t plain[16]=
{ 0xed, 0x1f, 0x7c, 0x59, 0xec, 0x86, 0xa4, 0x9e,
  0x2c, 0x6c, 0x22, 0xae, 0x20, 0xb4, 0xae, 0xde };
  
// 128-bit cipher text
uint8_t cipher[16]=
{ 0xd0, 0x36, 0x19, 0x4c, 0xc6, 0x70, 0x3b, 0x6e,
  0x32, 0xcc, 0x2b, 0x6f, 0xa4, 0xd1, 0x21, 0x40 };

void noekeon(void *mk, void *data);

int main(void) {
    uint8_t data[16];
    int     equ;
    
    memcpy(data, plain, 16);
    noekeon(key, data);
    equ = (memcmp (data, cipher, 16)==0);
    printf ("NOEKEON test : %s\n", equ ? "OK" : "FAILED");
    return 0;
}
