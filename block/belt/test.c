

#include <stdio.h>
#include <string.h>
#include <stdint.h>

// 256-bit master key
uint8_t key[32] = {
    0xE9,0xDE,0xE7,0x2C,
    0x8F,0x0C,0x0F,0xA6,
    0x2D,0xDB,0x49,0xF4,
    0x6F,0x73,0x96,0x47,
    0x06,0x07,0x53,0x16,
    0xED,0x24,0x7A,0x37,
    0x39,0xCB,0xA3,0x83,
    0x03,0xA9,0x8B,0xF6 };

// 128-bit plaintext
uint8_t plain[16] = {
    0xB1,0x94,0xBA,0xC8,
    0x0A,0x08,0xF5,0x3B,
    0x36,0x6D,0x00,0x8E,
    0x58,0x4A,0x5D,0xE4 };
                
uint8_t cipher[16] =
  { 0x69, 0xCC, 0xA1, 0xC9, 0x35, 0x57, 0xC9, 0xE3,
    0xD6, 0x6B, 0xC3, 0xE0, 0xFA, 0x88, 0xFA, 0x6E };
        
void belt(void *mk, void *data);

int main() {
    uint8_t data[16];
    int     equ;

    memcpy(data, plain, 16);
    belt(key, data);
    equ = (memcmp(data, cipher, 16)==0);
    printf("Bel-T test : %s\n", equ ? "OK" : "FAILED");
    return 0;
}
