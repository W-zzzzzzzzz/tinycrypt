
/**
  Maximum Distance Separable (MDS) codes are used as diffusion layers. They provide optimal diffusion effect to meet security of a round function of a block cipher. On the other hand, the constructions of these diffusion layers are various. For example, while the AES use a 4×4 MDS matrix over GF(28), Khazad use an 8×8 involutory MDS matrix over GF(28). In this study, a new involutory 4×4 MDS matrix for the AES-like block ciphers is proposed and an efficient software implementation of this matrix is given. The new involutory matrix replaces Mix Columns operation used in the AES cipher in order to provide equally good performance for both encryption and decryption operations. In the design of our involutory MDS matrix, we use Hadamard matrix construction instead of circulant matrices such as in the AES.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#define ROTR32(v,n)(((v)>>(n))|((v)<<(32-(n))))

// Multiplication
uint8_t gf_mul(uint8_t x, uint8_t y, uint8_t p)
{
    uint8_t z = 0;

    while (y) {
      if (y & 1) {
        z ^= x;
      }
      x = (x << 1) ^ (x & 0x80 ? p : 0x00);
      y >>= 1;
    }
    return z;
}

// linear layer using involutary matrix
uint32_t mds(uint32_t w) {
    int      i, j;
    uint32_t x0, y;
    
    union {
      uint8_t  b[4];
      uint32_t w;
    } acc, x;

#if defined(SUBFIELD)
#define ROOT 0x13
    uint8_t m[4][4] = {
      { 1,  4,  9, 13 },
      { 4,  1, 13,  9 },
      { 9, 13,  1,  4 },
      {13,  9,  4,  1} };
#elif defined(HADAMARD1)
#define ROOT 0x65
    uint8_t m[4][4] = {
      { 0x01, 0x02, 0xb0, 0xb2 },
      { 0x02, 0x01, 0xb2, 0xb0 },
      { 0xb0, 0xb2, 0x01, 0x02 },
      { 0xb2, 0xb0, 0x02, 0x01 } };
#elif defined(HADAMARD2)
#define ROOT 0x1b
    uint8_t m[4][4] = {
      { 0x01, 0x02, 0xfc, 0xfe },
      { 0x02, 0x01, 0xfe, 0xfc },   
      { 0xfc, 0xfe, 0x01, 0x02 },  
      { 0xfe, 0xfc, 0x02, 0x01 } }; 
#else
#define ROOT 0x1d
    uint8_t m[4][4] = { 
      { 0x01, 0x02, 0x04, 0x06 },
      { 0x02, 0x01, 0x06, 0x04 },
      { 0x04, 0x06, 0x01, 0x02 },
      { 0x06, 0x04, 0x02, 0x01 } };
#endif
    x.w   = w;
    acc.w = 0;

    for (i=0; i<4; i++) {
      for (j=0; j<4; j++) {
        acc.b[j] ^= gf_mul(x.b[i], m[i][j], ROOT);
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

// modular inverse
uint8_t gf_inv(uint8_t a) {
    uint8_t j, b = a;
    for (j = 14; --j;)
        b = gf_mul(b, j & 1 ? b : a, 0x1b); 
    return b;
}

uint32_t mulinv32(uint32_t x) {
    int      i;
    uint32_t r;
    
    for(r=i=0; i<4; i++) {
      r <<= 8;
      r |= gf_inv(x & 0xFF);
      x >>= 8;
    }
    return r;
}

void pi(uint8_t *a) {
    uint8_t tmp;

    tmp   = a[ 1]; 
    a[ 1] = a[ 4]; 
    a[ 4] = tmp;
    
    tmp   = a[ 2]; 
    a[ 2] = a[ 8]; 
    a[ 8] = tmp;
    
    tmp   = a[ 6]; 
    a[ 6] = a[ 9]; 
    a[ 9] = tmp;
    
    tmp   = a[ 3]; 
    a[ 3] = a[12]; 
    a[12] = tmp;
    
    tmp   = a[ 7]; 
    a[ 7] = a[13]; 
    a[13] = tmp;
    
    tmp   = a[11]; 
    a[11] = a[14]; 
    a[14] = tmp;
}

uint8_t invmod(uint8_t a, uint8_t m) {
    uint8_t j = 1, i = 0, b = m, c = a, x, y;

    while (c != 0) {
      x = b / c;
      y = b - x * c;
      b = c; 
      c = y;
      y = j;
      j = i - j * x;
      i = y;
    }
    if((int8_t)i < 0) {
      i += m;
    }
    return i;
}

uint8_t mulmod(uint8_t b, uint8_t e, uint8_t m) {
    uint8_t r = 0, t = b;

    while (e > 0) {
      if (e & 1) {
        r = r + t % m;
      }
      t = t + t % m;
      e >>= 1;
    }
    return r;
}

uint8_t power(uint8_t b, uint8_t e) {
    uint8_t r = 1;

    while (e > 0) {
      if (e & 1) {
        r = r * b;
      }
      b = b * b;
      e >>= 1;
    }
    return r;
}

uint8_t gcd (uint8_t a, uint8_t b) {
    uint8_t c;

    while (a != 0) {
      c = a; 
      a = b % a;  
      b = c;
    }
    return b;
}

int main(void) {
    union {
      uint8_t  b[16];
      uint32_t w[4];
    } s;
    int  i, j;

    srand(time(0));
    
    for(i=0; i<16; i++) s.b[i] = rand();
    
    printf("\nBefore MDS\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    //sbox(s.w);
    for(i=0; i<16; i++)  s.b[i] = gf_inv(s.b[i]);
    //pi(s.b);
        
    printf("\n\nAfter MDS and Sbox\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    
    //pi(s.b);
    for(i=0; i<16; i++)  s.b[i] = gf_inv(s.b[i]);
    //sbox(s.w);
        
    printf("\n\nAfter MDS and Sbox\n");
    for(i=0; i<16; i++) printf("%02x ", s.b[i]);
    putchar('\n');
    
    return 0;
}
