
// test unit for threefish
// odzhan

#include <stdio.h>
#include <string.h>
#include <stdint.h>
                     
uint64_t key[4] = 
{ 0x1716151413121110L, 0x1F1E1D1C1B1A1918L,
  0x2726252423222120L, 0x2F2E2D2C2B2A2928L};

uint64_t plain[4] = 
{ 0xF8F9FAFBFCFDFEFFL, 0xF0F1F2F3F4F5F6F7L,
  0xE8E9EAEBECEDEEEFL, 0xE0E1E2E3E4E5E6E7L};

uint64_t cipher[4] = 
{ 0xDF8FEA0EFF91D0E0ULL,  0xD50AD82EE69281C9ULL,
  0x76F48D58085D869DULL,  0xDF975E95B5567065ULL };

uint64_t tweak[] = 
{ 0x0706050403020100L, 0x0F0E0D0C0B0A0908L };
 
void threefish(void *mkt, void *data);

int main(void) {
    uint8_t data[32], key_tweak[48];
    int     equ;
    
    // copy 256-bit key
    memcpy(key_tweak, key, 32);
    // copy 128-bit tweak
    memcpy(&key_tweak[32], tweak, 16);
    // copy 256-bit plaintext
    memcpy(data, plain, 32);
    // encrypt plaintext
    threefish(key_tweak, data);

    equ = (memcmp(data, cipher, 32)==0);
    printf("Threefish test : %s\n", equ ? "OK" : "FAILED");
    return 0;
}
