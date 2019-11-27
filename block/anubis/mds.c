
/**
  Maximum Distance Separable (MDS) codes are used as diffusion layers. They provide optimal diffusion effect to meet security of a round function of a block cipher. On the other hand, the constructions of these diffusion layers are various. For example, while the AES use a 4×4 MDS matrix over GF(28), Khazad use an 8×8 involutory MDS matrix over GF(28). In this study, a new involutory 4×4 MDS matrix for the AES-like block ciphers is proposed and an efficient software implementation of this matrix is given. The new involutory matrix replaces Mix Columns operation used in the AES cipher in order to provide equally good performance for both encryption and decryption operations. In the design of our involutory MDS matrix, we use Hadamard matrix construction instead of circulant matrices such as in the AES.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// linear layer using involutary matrix
uint32_t mds(uint32_t w) {
    int i;
    uint32_t j, x0, y;
    union {
      uint8_t  b[4];
      uint32_t w;
    } acc, x;

#if defined(JOLTIK)
    uint8_t m[4][4] = {
      { 1,  4,  9, 13 },
      { 4,  1, 13,  9 },
      { 9, 13,  1,  4 },
      {13,  9,  4,  1} };
#elif defined(HADAMARD)
    uint8_t m[4][4] = {
      { 0x01, 0x02, 0xfc, 0xfe },
      { 0x02, 0x01, 0xfe, 0xfc },
      { 0xfc, 0xfe, 0x01, 0x02 },
      { 0xfe, 0xfc, 0x02, 0x01 } };
#else
    uint8_t m[4][4] = { 
      { 0x01, 0x02, 0x04, 0x06 },
      { 0x02, 0x01, 0x06, 0x04 },
      { 0x04, 0x06, 0x01, 0x02 },
      { 0x06, 0x04, 0x02, 0x01 } };
#endif
    x.w = w;
    acc.w = 0;

    for (i=0; i<4; i++) {
      for (j=0; j<4; j++) {
        x0 = m[i][j];
        y  = x.b[j];
        while (y) {
          if (x0 > (x0 ^ 0x169))
            x0 ^= 0x169;
          if (y & 1)
            acc.b[i] ^= x0;
          x0 <<= 1;
          y >>= 1;
        }
      }
    }
    return acc.w;
}

// non-linear layer using involutary 4x4 sbox
// This is the Gamma function from Noekeon
void sbox(uint32_t s[]) {
    uint32_t a, b, c, d, t;
    
    a = s[0]; b = s[1]; c = s[2]; d = s[3];
    
    b ^= ~(d | c); 
    
    t  = d; 
    
    d  = a ^ (c & b); 
    
    a  = t;
    
    c ^= a ^ b ^ d; 
    
    b ^= ~(d | c);
    
    a ^= c & b;
    
    s[0] = a; s[1] = b; s[2] = c; s[3] = d;
}

int main(void) {
    union {
      uint8_t  b[16];
      uint32_t w[4];
    } s;
    int  i;
    
    for(i=0; i<16; i++) s.b[i] = (i+1);
    
    printf("\nBefore MDS\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    sbox(s.w);
    for(i=0; i<4; i++)  s.w[i] = mds(s.w[i]);
    
    printf("\n\nAfter MDS and Sbox\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    
    for(i=0; i<4; i++)  s.w[i] = mds(s.w[i]);
    sbox(s.w);
        
    printf("\n\nAfter MDS and Sbox\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    putchar('\n');
    
    return 0;
}
