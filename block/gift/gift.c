

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//Sbox
uint8_t GIFT_S[16] = { 1,10, 4,12, 6,15, 3, 9, 2,13,11, 7, 5, 0, 8,14};

// round constants
const unsigned char GIFT_RC[62] = {
    0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3E, 0x3D, 0x3B, 0x37, 0x2F,
    0x1E, 0x3C, 0x39, 0x33, 0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B,
    0x16, 0x2C, 0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E,
    0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36, 0x2D, 0x1A,
    0x34, 0x29, 0x12, 0x24, 0x08, 0x11, 0x22, 0x04, 0x09, 0x13,
    0x26, 0x0c, 0x19, 0x32, 0x25, 0x0a, 0x15, 0x2a, 0x14, 0x28,
    0x10, 0x20 };

void gift(void*mk, void*data) {

    uint8_t key[32];
    uint8_t *input=(uint8_t*)data;
    uint8_t GIFT_P[128];
    int     i;
    uint8_t rc=0, t;
    
    for(i=0;i<16;i++) {
      rc ^= 1;
      printf(" 0x%02x, ", rc);
      t = (rc << 1) & 64;
      rc = ((rc << 1) & 63) | (rc >> 5) | (t >> 6);
    }
    
    printf("\n");
    
    // generate permutation bits
    for(i=0;i<128;i++) {
      GIFT_P[i] = (4*(i/16))+32*((3*((i%16)/4)+(i%4))%4)+(i%4);
    }
    
    for (int i=0; i<32;i++){
        key[i] = ((uint8_t*)mk)[i];
    }

    unsigned char bits[128], perm_bits[128];
    unsigned char key_bits[128];
    unsigned char temp_key[32];

    for (int r=0; r<40; r++) {

    //SubCells
    for (int i=0; i<32; i++){
        input[i] = GIFT_S[input[i]];
    }

    //PermBits
    //input to bits
    for (int i=0; i<32; i++){
        for (int j=0; j<4; j++){
            bits[4*i+j] = (input[i] >> j) & 0x1;
        }
    }
    //permute the bits
    for (int i=0; i<128; i++){
        perm_bits[GIFT_P[i]] = bits[i];
    }
    //perm_bits to input
    for (int i=0; i<32; i++){
        input[i]=0;
        for (int j=0; j<4; j++){
             input[i] ^= perm_bits[4*i+j] << j;
        }
    }

    //AddRoundKey
    //input to bits
    for (int i=0; i<32; i++){
        for (int j=0; j<4; j++){
            bits[4*i+j] = (input[i] >> j) & 0x1;
        }
    }
    //key to key_bits
    for (int i=0; i<32; i++){
        for (int j=0; j<4; j++){
            key_bits[4*i+j] = (key[i] >> j) & 0x1;
        }
    }

    //add round key
    int kbc=0;  //key_bit_counter
    for (int i=0; i<32; i++){
        bits[4*i+1] ^= key_bits[kbc];
        bits[4*i+2] ^= key_bits[kbc+64];
        kbc++;
    }

    //add constant
    bits[3] ^= GIFT_RC[r] & 0x1;
    bits[7] ^= (GIFT_RC[r]>>1) & 0x1;
    bits[11] ^= (GIFT_RC[r]>>2) & 0x1;
    bits[15] ^= (GIFT_RC[r]>>3) & 0x1;
    bits[19] ^= (GIFT_RC[r]>>4) & 0x1;
    bits[23] ^= (GIFT_RC[r]>>5) & 0x1;
    bits[127] ^= 1;

    //bits to input
    for (int i=0; i<32; i++){
        input[i]=0;
        for (int j=0; j<4; j++){
             input[i] ^= bits[4*i+j] << j;
        }
    }

    //key update
    //entire key>>32
    for(int i=0; i<32; i++){
        temp_key[i] = key[(i+8)%32];
    }
    for(int i=0; i<24; i++) key[i] = temp_key[i];
    //k0>>12
    key[24] = temp_key[27];
    key[25] = temp_key[24];
    key[26] = temp_key[25];
    key[27] = temp_key[26];
    //k1>>2
    key[28] = ((temp_key[28]&0xc)>>2) ^ ((temp_key[29]&0x3)<<2);
    key[29] = ((temp_key[29]&0xc)>>2) ^ ((temp_key[30]&0x3)<<2);
    key[30] = ((temp_key[30]&0xc)>>2) ^ ((temp_key[31]&0x3)<<2);
    key[31] = ((temp_key[31]&0xc)>>2) ^ ((temp_key[28]&0x3)<<2);
  }
}

int main(void) {
  int i, j;
  
  uint8_t txt[32] = { 
    0xe3,0x9c,0x14,0x1f,0xa5,0x7d,0xba,0x43,
    0xf0,0x8a,0x85,0xb6,0xa9,0x1f,0x86,0xc1 };
  uint8_t key[32] = { 
    0xd0,0xf5,0xc5,0x9a,0x77,0x00,0xd3,0xe7,
    0x99,0x02,0x8f,0xa9,0xf9,0x0a,0xd8,0x37 };
  
  // cd 0b d7 38 38 8a d3 f6 68 b1 5a 36 ce b6 ff 92 
  
  memset(txt, 0, sizeof(txt));
  memset(key, 0, sizeof(key));
  
  for(i=0;i<128;i++) {
    if ((i&15)==0) putchar('\n');
    printf(" %d,", (4*(i/16))+32*((3*((i%16)/4)+(i%4))%4)+ (i % 4));
  }
  printf("\n\n");
  gift(key,txt);
  
  printf("ciphertext : ");
  for(i=0;i<32;i++) {
    printf("%x", txt[31-i]);
    if ((i&1))putchar(' ');
  }
  printf("\n\n");
  return 0;
}
