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

#define R(v,n)(((v)<<(n))|((v)>>(32-(n))))
#define F(n)for(i=0;i<n;i++)

typedef unsigned long long Q;
typedef unsigned int W;
typedef unsigned char B;

typedef struct _md4_ctx {
    W s[4];
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    Q len;
}md4_ctx;

#define FF(x,y,z)((z)^((x)&((y)^(z))))
#define GG(x,y,z)(((x)& (y))|((z)&((x)|(y))))
#define HH(x,y,z)((x)^(y)^(z))

void md4_compress(md4_ctx*c) {
    W i,t,s[4]; 
    B r[16]={3,7,11,19,3,5,9,13,3,9,11,15};
    B g[16]={0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15};
    B h[16]={0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15};

    // load state
    F(4)s[i]=c->s[i];
    
    // permute
    F(48) {
      if(i<16){
        s[0]+=FF(s[1],s[2],s[3])+c->x.w[i];
        t=r[i%4];
      } else if(i<32){
        s[0]+=GG(s[1],s[2],s[3])+c->x.w[g[i%16]]+0x5a827999L;
        t=r[4+i%4];
      } else {
        s[0]+=HH(s[1],s[2],s[3])+c->x.w[h[i%16]]+0x6ed9eba1L;
        t=r[8+i%4];
      }
      t=R(s[0],t);s[0]=s[3];s[3]=s[2];s[2]=s[1];s[1]=t;
    }
    // update state
    F(4)c->s[i]+=s[i];
}

void md4_init(md4_ctx*c) {
    c->s[0]=0x67452301;
    c->s[1]=0xefcdab89;
    c->s[2]=0x98badcfe;
    c->s[3]=0x10325476;
    c->len =0;
}

void md4_update(md4_ctx*c,const void*in,W len) {
    B *p=(B*)in;
    W i, idx;
    
    idx = c->len & 63;
    c->len += len;
    
    for (i=0;i<len;i++) {
      c->x.b[idx]=p[i]; idx++;
      if(idx==64) {
        md4_compress(c);
        idx=0;
      }
    }
}

void md4_final(void*h,md4_ctx*c) {
    W i,len,*p=(W*)h;
    
    i = len = c->len & 63;
    while(i<64) c->x.b[i++]=0;
    c->x.b[len]=0x80;
    if(len>=56) {
      md4_compress(c);
      F(16)c->x.w[i]=0;
    }
    c->x.q[7]=c->len*8;
    md4_compress(c);
    F(4)p[i]=c->s[i];
}

#ifdef TEST

// this test code was originally written by Markku-Juhani O. Saarinen
// i've adapted to work with this

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md4(void *out, const void *in, size_t inlen)
{
    md4_ctx ctx;

    md4_init(&ctx);
    md4_update(&ctx, in, inlen);
    md4_final(out, &ctx);
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

int md4_selftest(void)
{
    // Grand hash of hash results.
    const uint8_t md4_res[16] = {
       0x75, 0xe6, 0x08, 0x16, 0x24, 0xf7, 0x1a, 0x25, 
       0x14, 0x22, 0x38, 0xf0, 0xdb, 0xe2, 0xf2, 0xfa };
    // Parameter sets.
    const size_t s2_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, inlen;
    uint8_t in[1024], md[16];
    md4_ctx ctx;

    // 256-bit hash for testing.
    md4_init(&ctx);

    for (j = 0; j < 6; j++) {
        inlen = s2_in_len[j];

        selftest_seq(in, inlen, inlen);
        md4(md, in, inlen);
        md4_update(&ctx, md,16);
    }

    // Compute and compare the hash of hashes.
    md4_final(md, &ctx);
    
    for (i = 0; i < 16; i++) {
      if (md[i] != md4_res[i])
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    int     i;
    md4_ctx c;
    uint8_t dgst[16];
    
    if (argc>1) {
      md4_init(&c);
      md4_update(&c, argv[1], strlen(argv[1]));
      md4_final(dgst, &c);
      
      for(i=0;i<16;i++) {
        printf("%02x", dgst[i]);
      }
      putchar('\n');
    }
    printf("md4_selftest() = %s\n",
         md4_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
