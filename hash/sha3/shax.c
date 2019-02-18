/**
  Copyright © 2015, 2016 Odzhan. All Rights Reserved.

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

#define R(v,n)(((v)>>(n))|((v)<<(64-(n))))
#define F(a,b)for(a=0;a<b;a++)
  
typedef unsigned long long W;
typedef unsigned char B;

typedef struct _sha3_ctx {
    union {
      B b[200];
      W q[25];
    } s;
    int i,h,r;
}sha3_ctx;

void P(void*p) {
    W n,i,j,r,x,y,t,Y,b[5],*s=p;
    B c=1;

    F(n,24){
      F(i,5){b[i]=0;F(j,5)b[i]^=s[i+j*5];}
      F(i,5){
        t=b[(i+4)%5]^R(b[(i+1)%5],63);
        F(j,5)s[i+j*5]^=t;}
      t=s[1],y=r=0,x=1;
      F(j,24)
        r+=j+1,Y=(x*2)+(y*3),x=y,y=Y%5,
        Y=s[x+y*5],s[x+y*5]=R(t, -r),t=Y;
      F(j,5){
        F(i,5)b[i]=s[i+j*5];
        F(i,5)
          s[i+j*5]=b[i]^( b[(i+2)%5] &~ b[(i+1)%5]);}
      F(j,7)
        if((c=(c<<1)^((c>>7)*113))&2)
          *s^=1ULL<<((1<<j)-1);
    }
}

void sha3_init(sha3_ctx*c, int len) {
    W i;
    
    F(i,25)c->s.q[i]=0;
    
    c->h = len;
    c->r = 200-(2*len);
    c->i = 0;
}

void sha3_update(sha3_ctx*c, const void*in, W len) {
    W i;
    B *p=(B*)in;
  
    F(i,len) {
      c->s.b[c->i++] ^= *p++;    
      if(c->i == c->r) {
        P(&c->s); 
        c->i=0;
      }
    }
}

void sha3_final(void*out, sha3_ctx*c) {
    B   *p=(B*)out;
    int i;
    
    c->s.b[c->i]^=6;
    c->s.b[c->r-1]^=0x80;
    
    P(&c->s);
    F(i,c->h)p[i]=c->s.b[i];
}

#ifdef TEST

// this test code was originally written by Markku-Juhani O. Saarinen
// i've adapted to work with this

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int sha3(void *out, size_t outlen,
    const void *in, size_t inlen)
{
    sha3_ctx ctx;

    sha3_init(&ctx, outlen);
    sha3_update(&ctx, in, inlen);
    sha3_final(out, &ctx);

    return 0;
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

int sha3_selftest()
{
    // grand hash of hash results
    const uint8_t sha3_res[32] = {
      0xfe, 0xd2, 0x6d, 0x05, 0x27, 0x6a, 0xf6, 0x00,
      0x6c, 0xaa, 0x39, 0x36, 0xe1, 0x4b, 0xaa, 0x09,
      0x33, 0x3e, 0xce, 0x1d, 0x12, 0xad, 0x66, 0x6e,
      0xab, 0x62, 0x2c, 0x9c, 0xda, 0xe7, 0x3f, 0x89 };
      
    // parameter sets
    const size_t md_len[4] = { 20, 32, 48, 64 };
    const size_t in_len[6] = { 0, 3, 128, 129, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[64], key[64];
    sha3_ctx ctx;

    // 256-bit hash for testing
    sha3_init(&ctx, 32);

    for (i = 0; i < 4; i++) {
        outlen = md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            sha3(md, outlen, in, inlen);
            sha3_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // compute and compare the hash of hashes
    sha3_final(md,&ctx);
    
    for (i = 0; i < 32; i++) {
      if (md[i] != sha3_res[i])
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("sha3_selftest() = %s\n",
         sha3_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
