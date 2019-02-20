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
  POSSIBILITY OF SUCH DAMAGE.*/

// parameters for BLAKE2s
#ifdef S
#define VERSION  "s"
#define WORDLEN  32
#define BLOCKLEN 64
#define ROUNDS   80
#define ROTATION 0x07080C10
#define OUTLEN   32
#define KEYLEN   32
typedef unsigned int W;
    W iv[8] =
    { 0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
      0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19 };
#else
// parameters for BLAKE2b
#define VERSION   "b"
#define WORDLEN   64
#define BLOCKLEN 128
#define ROUNDS    96
#define ROTATION 0x3F101820
#define OUTLEN    64
#define KEYLEN    64
typedef unsigned long long W;
    W iv[8] =
    { 0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
      0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
      0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179 };
#endif

#define R(v,n)(((v)>>(n))|((v)<<(WORDLEN-(n))))
#define X(a,b)(t)=(a),(a)=(b),(b)=(t)
#define F(n)for(i=0;i<n;i++)

typedef unsigned char B;
typedef unsigned long long Q;

typedef struct _blake2_ctx {
    W s[16], idx, outlen;
    union {
      B b[BLOCKLEN];
      W w[BLOCKLEN/WORDLEN];
    }x;
    Q len;
}blake2_ctx;

int blake2_init(blake2_ctx*,W,const void*,W);
void blake2_update(blake2_ctx*,const void*,W);
void blake2_final(void*,blake2_ctx*);

    W v_idx[8]=
    { 0xC840, 0xD951, 0xEA62, 0xFB73,
      0xFA50, 0xCB61, 0xD872, 0xE943 };
      
    Q sigma[12] = 
    { 0xfedcba9876543210, 0x357b20c16df984ae,
      0x491763eadf250c8b, 0x8f04a562ebcd1397,
      0xd386cb1efa427509, 0x91ef57d438b0a6c2,
      0xb8293670a4def15c, 0xa2684f05931ce7bd,
      0x5a417d2c803b9ef6, 0x0dc3e9bf5167482a,
      0xfedcba9876543210, 0x357b20c16df984ae };
      
void blake2_compress(blake2_ctx *ctx, W last) {
    W i, j, a, b, c, d, r, t, s[16];
    Q z, *p=sigma;
      
    F(8)s[i]=ctx->s[i], s[i+8]=iv[i];
  #ifdef S
    s[12]^=((W*)&ctx->len)[0];s[13]^=((W*)&ctx->len)[1];
  #else
    s[12]^=ctx->len;
  #endif
    s[14]^=-last;
    for(i=0;i<ROUNDS;) {
      z=*p++;
      do {
        d=v_idx[i++%8];
        a=(d&15);b=(d>>4&15);
        c=(d>>8&15);d>>=12;
        r=ROTATION;
        for(j=0;j<4;j++) {
          if(!(j&1)) {
            s[a]+=ctx->x.w[z&15];
            z>>=4;
          }
          s[a]+=s[b];
          s[d]=R(s[d]^s[a],(r&255));
          X(a,c),X(b,d);
          r>>=8;
        }
      } while(z!=0);
    }
    F(8)ctx->s[i]^=s[i]^s[i+8];
}

int blake2_init (blake2_ctx *c,W outlen,const void *key,W keylen) {
    W i;
    
    if(outlen == 0 || outlen > OUTLEN || keylen > KEYLEN) return -1;
    
    // set the initial values
    F(8)c->s[i]=iv[i];
    
    c->s[0]  ^= 0x01010000^(keylen<<8)^outlen;
    c->len    = 0;
    c->idx    = 0;
    c->outlen = outlen;
    
    // zero initialize buffer
    for(i=keylen;i<BLOCKLEN;i++) c->x.b[i]=0;
    
    // add key if provided
    if(keylen>0) {
      blake2_update(c,key,keylen);
      c->idx = BLOCKLEN;
    }
    return 0;
}

void blake2_update(blake2_ctx *c,const void *in,W len) {
    B *p=(B*)in;
    W i;
    
    F(len) {
      // buffer filled?
      if(c->idx == BLOCKLEN) {
        // update length
        c->len += BLOCKLEN;
        blake2_compress(c, 0);
        // reset buffer index
        c->idx = 0;
      }
      // add byte to buffer, increase index
      c->x.b[c->idx] = p[i]; c->idx++;
    }
}

void blake2_final(void *out, blake2_ctx *c) {
    W i;
    
    c->len += c->idx;
    // zero remainder of buffer
    while(c->idx < BLOCKLEN) c->x.b[c->idx++] = 0;
    // permute
    blake2_compress(c, 1);
    // return hash
    F(c->outlen)((B*)out)[i]=((B*)c->s)[i];
}

#ifdef TEST

// test code written by Markku-Juhani O. Saarinen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int blake2(void *out, size_t outlen,
    const void *key, size_t keylen,
    const void *in, size_t inlen)
{
    blake2_ctx ctx;

    if (blake2_init(&ctx, outlen, key, keylen))
        return -1;
    blake2_update(&ctx, in, inlen);
    blake2_final(out, &ctx);

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

int blake2_selftest()
{
  #ifdef S
    // grand hash of hash results for BLAKE2s
    const uint8_t blake2_res[32] = {
        0x6A, 0x41, 0x1F, 0x08, 0xCE, 0x25, 0xAD, 0xCD,
        0xFB, 0x02, 0xAB, 0xA6, 0x41, 0x45, 0x1C, 0xEC,
        0x53, 0xC5, 0x98, 0xB2, 0x4F, 0x4F, 0xC7, 0x87,
        0xFB, 0xDC, 0x88, 0x79, 0x7F, 0x4C, 0x1D, 0xFE };
    const size_t b2_md_len[4] = { 16, 20, 28, 32 };
    const size_t b2_in_len[6] = { 0,  3,  64, 65, 255, 1024 };
  #else
    // grand hash of hash results for BLAKE2b
    const uint8_t blake2_res[32] = {
        0xC2, 0x3A, 0x78, 0x00, 0xD9, 0x81, 0x23, 0xBD,
        0x10, 0xF5, 0x06, 0xC6, 0x1E, 0x29, 0xDA, 0x56,
        0x03, 0xD7, 0x63, 0xB8, 0xBB, 0xAD, 0x2E, 0x73,
        0x7F, 0x5E, 0x76, 0x5A, 0x7B, 0xCC, 0xD4, 0x75 };
    const size_t b2_md_len[4] = { 20, 32, 48, 64 };
    const size_t b2_in_len[6] = { 0, 3, 128, 129, 255, 1024 };
  #endif
    // parameter sets


    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[OUTLEN], key[KEYLEN];
    blake2_ctx ctx;

    // 256-bit hash for testing
    if (blake2_init(&ctx, 32, NULL, 0))
        return -1;

    for (i = 0; i < 4; i++) {
        outlen = b2_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            blake2(md, outlen, NULL, 0, in, inlen);
            blake2_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            blake2(md, outlen, key, outlen, in, inlen);
            blake2_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // compute and compare the hash of hashes
    blake2_final(md,&ctx);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2_res[i])
            return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("blake2%s_selftest() = %s\n", VERSION,
         blake2_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
