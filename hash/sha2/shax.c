/**
  Copyright © 2015 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define F(n)for(i=0;i<n;i++)

#define rev32(x) __builtin_bswap32(x)
#define rev64(x) __builtin_bswap64(x)

typedef unsigned long long Q;
typedef unsigned int W;
typedef unsigned char B;

typedef struct _sha256_ctx {
    W s[8];
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    Q len;
}sha256_ctx;

#define CH(x,y,z)(((x)&(y))^(~(x)&(z)))
#define MAJ(x,y,z)(((x)&(y))^((x)&(z))^((y)&(z)))

#define EP0(x)(R(x,2)^R(x,13)^R(x,22))
#define EP1(x)(R(x,6)^R(x,11)^R(x,25))
#define SIG0(x)(R(x,7)^R(x,18)^((x)>>3))
#define SIG1(x)(R(x,17)^R(x,19)^((x)>>10))

void sha256_compress(sha256_ctx*c) {
    W t1,t2,i,w[64],x[8];
    
    W k[64]=
    { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
      0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
      0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
      0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
      0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
      0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 }; 
    
    // load input in big endian byte order
    F(16)w[i] = rev32(c->x.w[i]);
    
    // expand input
    for(i=16;i<64;i++)
      w[i] = SIG1(w[i-2])+w[i-7]+SIG0(w[i-15])+w[i-16];
    
    // load state
    F(8)x[i] = c->s[i];
    
    // permute
    F(64) {
      t1 = x[7] + EP1(x[4]) + CH(x[4],x[5],x[6]) + w[i] + k[i];
      t2 = EP0(x[0]) + MAJ(x[0],x[1],x[2]);
      x[7] = x[6],x[6] = x[5],x[5] = x[4],x[4] = x[3] + t1;
      x[3] = x[2],x[2] = x[1],x[1] = x[0], x[0] = t1 + t2;
    }
    // update state
    F(8)c->s[i] += x[i];
}

void sha256_init(sha256_ctx*c) {    
    c->s[0]=0x6a09e667;
    c->s[1]=0xbb67ae85;
    c->s[2]=0x3c6ef372;
    c->s[3]=0xa54ff53a;
    c->s[4]=0x510e527f;
    c->s[5]=0x9b05688c;
    c->s[6]=0x1f83d9ab;
    c->s[7]=0x5be0cd19;
    c->len =0;
}

void sha256_update(sha256_ctx*c,const void*in,W len) {
    B *p=(B*)in;
    W i, idx;
    
    idx = c->len & 63;
    c->len += len;
    
    for (i=0;i<len;i++) {
      c->x.b[idx]=p[i]; idx++;
      if(idx==64) {
        sha256_compress(c);
        idx=0;
      }
    }
}

void sha256_final(void*out,sha256_ctx*c) {
    W i,len,*p=(W*)out;
    
    i = len = c->len & 63;
    while(i < 64) c->x.b[i++]=0;
    c->x.b[len]=0x80;
    
    if(len >= 56) {
      sha256_compress(c);
      F(16)c->x.w[i]=0;
    }
    c->x.q[7]=rev64(c->len*8);
    sha256_compress(c);
    F(8)p[i]=rev32(c->s[i]);
}

#ifdef TEST

// this test code was originally written by Markku-Juhani O. Saarinen
// i've adapted to work with this

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sha256(void *out, const void *in, size_t inlen)
{
    sha256_ctx ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, in, inlen);
    sha256_final(out, &ctx);
}

// Deterministic sequences (Fibonacci generator).

static void selftest_seq(uint8_t *out, size_t len, uint32_t seed)
{
    size_t i;
    uint32_t t, a , b;

    a = 0xDEAD4BAD * seed;              // prime
    b = 1;

    for (i = 0; i < len; i++) {         // fill the buf
        t = a + b;
        a = b;
        b = t;
        out[i] = (t >> 24) & 0xFF;
    }
}

int sha256_selftest(void)
{
    // Grand hash of hash results.
    const uint8_t sha256_res[32] = {
       0x6c, 0x06, 0xc1, 0x3d, 0xf2, 0x4a, 0x1c, 0x8c,
       0x0a, 0x90, 0x63, 0xc7, 0x94, 0x46, 0x76, 0x22,
       0xbe, 0xae, 0x5b, 0x03, 0x31, 0xa2, 0x3f, 0x93,
       0x26, 0x15, 0x5a, 0xdb, 0x9c, 0x8b, 0xaa, 0xbd };
    // Parameter sets.
    const size_t s2_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32];
    sha256_ctx ctx;

    // 256-bit hash for testing.
    sha256_init(&ctx);

    for (j = 0; j < 6; j++) {
        inlen = s2_in_len[j];

        selftest_seq(in, inlen, inlen);
        sha256(md, in, inlen);
        sha256_update(&ctx, md, outlen);
    }

    // Compute and compare the hash of hashes.
    sha256_final(md, &ctx);
    
    for (i = 0; i < 32; i++) {
      if (md[i] != sha256_res[i])
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int        i;
    sha256_ctx c;
    uint8_t    dgst[32];
    
    if (argc>1) {
      sha256_init(&c);
      sha256_update(&c, argv[1], strlen(argv[1]));
      sha256_final(dgst, &c);
      
      for(i=0;i<32;i++) {
        printf("%02x", dgst[i]);
      }
      putchar('\n');
    }
    printf("sha256_selftest() = %s\n",
         sha256_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
