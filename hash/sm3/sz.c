/**
  Copyright © 2017 Odzhan. All Rights Reserved.

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

#include "sm3.h"

#define  F(x, y, z) (((x) ^ (y) ^ (z)))
#define FF(x, y, z) (((x) & (y)) | ((x) & (z)) | ((y) & (z))) 
#define GG(x, y, z) ((z)  ^ ((x) & ((y) ^ (z))))

#define P0(x) x ^ ROTL32(x,  9) ^ ROTL32(x, 17)
#define P1(x) x ^ ROTL32(x, 15) ^ ROTL32(x, 23)

/************************************************
*
* update state with block of data
*
************************************************/
void SM3_Transform (SM3_CTX *ctx) 
{
    uint32_t tt1, tt2, i, t, ss1, ss2, x, y;
    uint32_t w[68], s[8];

    #define a s[0]
    #define b s[1]
    #define c s[2]
    #define d s[3]
    #define e s[4]
    #define f s[5]
    #define g s[6]
    #define h s[7]
    
    // load state into local buffer
    memcpy((uint8_t*)&s[0], (uint8_t*)&ctx->s.w[0], 8*4);
    
    // load data in big endian format
    for (i=0; i<16; i++) {
      w[i] = SWAP32(ctx->buf.w[i]);
    }

    // expand message
    for (i=16; i<68; i++) {
      x = ROTL32(w[i- 3], 15);
      y = ROTL32(w[i-13],  7);
      
      x ^= w[i-16];
      x ^= w[i- 9];
      y ^= w[i- 6];
      
      w[i] = P1(x) ^ y; 
    }

    // compression function
    for (i=0; i<64; i++) 
    {      
      t  = (i < 16) ? 0x79cc4519 : 0x7a879d8a;
      
      ss2 = ROTL32(a, 12);      
      ss1 = ROTL32(ss2 + e + ROTL32(t, i), 7);
      ss2 ^= ss1;
      
      tt1 = d + ss2 + (w[i] ^ w[i+4]);
      tt2 = h + ss1 + w[i];
      
      if (i < 16) {
        tt1 += F(a, b, c);
        tt2 += F(e, f, g);
      } else {
        tt1 += FF(a, b, c);
        tt2 += GG(e, f, g);       
      }
      d = c;
      c = ROTL32(b, 9);
      b = a;
      a = tt1;
      h = g;
      g = ROTL32(f, 19);
      f = e;
      e = P0(tt2); 
    }
    
    // Davies–Meyer idea for compression function
    for (i=0; i<8; i++) {
      ctx->s.w[i] ^= s[i];
    }    
    #undef a
    #undef b
    #undef c
    #undef d
    #undef e
    #undef f
    #undef g
    #undef h
}

/************************************************
*
* initialize context
*
************************************************/
void SM3_Init (SM3_CTX *c) {    
    c->s.w[0] = 0x7380166f;
    c->s.w[1] = 0x4914b2b9;
    c->s.w[2] = 0x172442d7;
    c->s.w[3] = 0xda8a0600;
    c->s.w[4] = 0xa96f30bc;
    c->s.w[5] = 0x163138aa;
    c->s.w[6] = 0xe38dee4d;
    c->s.w[7] = 0xb0fb0e4e;
    c->len    = 0;
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

// test code written by Markku-Juhani O. Saarinen

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sha1(void *out, const void *in, size_t inlen)
{
    sha1_ctx ctx;

    sha1_init(&ctx);
    sha1_update(&ctx, in, inlen);
    sha1_final(out, &ctx);
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


int sha1_selftest(void)
{
    // Grand hash of hash results.
    const uint8_t sha1_res[20] = {
      0x0c, 0xce, 0x6c, 0x1a, 0x08, 0x73, 0xdb, 0x99,
      0xd4, 0x91, 0xd1, 0xc7, 0x82, 0x48, 0x24, 0x18,
      0x20, 0xd9, 0x36, 0xe9 };
    // Parameter sets.
    const size_t s2_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[20];
    sha1_ctx ctx;

    // 160-bit hash for testing.
    sha1_init(&ctx);

    for (j = 0; j < 6; j++) {
        inlen = s2_in_len[j];

        selftest_seq(in, inlen, inlen);
        sha1(md, in, inlen);
        sha1_update(&ctx, md, 20);
    }

    // Compute and compare the hash of hashes.
    sha1_final(md, &ctx);
    
    for (i = 0; i < 20; i++) {
      if (md[i] != sha1_res[i])
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("sha1_selftest() = %s\n",
         sha1_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
