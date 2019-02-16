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

#define R(v,n)(((v)>>(n))|((v)<<(64-(n))))
#define X(a,b)(t)=(a),(a)=(b),(b)=(t)
#define F(n)for(i=0;i<n;i++)

typedef unsigned long long W;
typedef unsigned char B;

typedef struct _blake2s_ctx {
    W s[16], idx, outlen;
    union {
      B b[128];
      W w[16];
    }x;
    W len;
}blake2b_ctx;

int blake2b_init(blake2b_ctx*,W,const void*,W);
void blake2b_update(blake2b_ctx*,const void*,W);
void blake2b_final(void*,blake2b_ctx*);

    W v[8]=
    { 0xC840, 0xD951, 0xEA62, 0xFB73,
      0xFA50, 0xCB61, 0xD872, 0xE943 };
    
    W sigma[12] = 
    { 0xfedcba9876543210, 0x357b20c16df984ae,
      0x491763eadf250c8b, 0x8f04a562ebcd1397,
      0xd386cb1efa427509, 0x91ef57d438b0a6c2,
      0xb8293670a4def15c, 0xa2684f05931ce7bd,
      0x5a417d2c803b9ef6, 0x0dc3e9bf5167482a,
      0xfedcba9876543210, 0x357b20c16df984ae };
   
    W iv[8] =
    { 0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
      0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
      0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179 };
      
void G(W *s, W *m) {
    W i, j, a, b, c, d, r, t, *p, z;

    p=sigma;
    
    for(i=0;i<96;) {
      z=*p++;
      while(z) {
        d=v[i++%8];
        a=(d&15);b=(d>>4&15);
        c=(d>>8&15);d>>=12;
        r=0x3F101820;
        for(j=0;j<4;j++) {
          if(!(j&1)) {
            s[a]+=m[z&15];
            z>>=4;
          }
          s[a]+=s[b];
          s[d]=R(s[d]^s[a],(r&255));
          X(a,c),X(b,d);
          r>>=8;
        }
      }
    }
}

void blake2b_compress(blake2b_ctx*c, W last) {
    W i, v[16];
      
    F(8)v[i]=c->s[i], v[i+8]=iv[i];
    v[12]^=c->len;v[14]^=-last;
    G(v,c->x.w);
    F(8)c->s[i]^=v[i]^v[i+8];
}

int blake2b_init (blake2b_ctx*c,W outlen,const void*key,W keylen) {
    W i;
    
    if(outlen == 0 || outlen > 64 || keylen > 64) return -1;
    
    F(8)c->s[i]=iv[i];
    
    c->s[0]  ^= 0x01010000^(keylen<<8)^outlen;
    c->len    = 0;
    c->idx    = 0;
    c->outlen = outlen;
    
    for(i=keylen;i<128;i++) c->x.b[i]=0;
    
    if(keylen>0) {
      blake2b_update(c,key,keylen);
      c->idx=128;
    }
    return 0;
}

void blake2b_update(blake2b_ctx*c,const void*in,W len) {
    B *p=(B*)in;
    W i;
    
    F(len) {
      if(c->idx==128) {
        c->len += 128;
        blake2b_compress(c,0);
        c->idx=0;
      }
      c->x.b[c->idx++] = *p++;
    }
}

void blake2b_final(void*out, blake2b_ctx*c) {
    W i;
    
    c->len += c->idx;
    while(c->idx < 128) c->x.b[c->idx++]=0;
    blake2b_compress(c,1);
    F(c->outlen)((B*)out)[i]=((B*)c->s)[i];
}

#ifdef TEST

// test code written by Markku-Juhani O. Saarinen

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

int blake2b(void *out, size_t outlen,
    const void *key, size_t keylen,
    const void *in, size_t inlen)
{
    blake2b_ctx ctx;

    if (blake2b_init(&ctx, outlen, key, keylen))
        return -1;
    blake2b_update(&ctx, in, inlen);
    blake2b_final(out, &ctx);

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

int blake2b_selftest()
{
    // grand hash of hash results
    const uint8_t blake2b_res[32] = {
        0xC2, 0x3A, 0x78, 0x00, 0xD9, 0x81, 0x23, 0xBD,
        0x10, 0xF5, 0x06, 0xC6, 0x1E, 0x29, 0xDA, 0x56,
        0x03, 0xD7, 0x63, 0xB8, 0xBB, 0xAD, 0x2E, 0x73,
        0x7F, 0x5E, 0x76, 0x5A, 0x7B, 0xCC, 0xD4, 0x75
    };
    // parameter sets
    const size_t b2b_md_len[4] = { 20, 32, 48, 64 };
    const size_t b2b_in_len[6] = { 0, 3, 128, 129, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[64], key[64];
    blake2b_ctx ctx;

    // 256-bit hash for testing
    if (blake2b_init(&ctx, 32, NULL, 0))
        return -1;

    for (i = 0; i < 4; i++) {
        outlen = b2b_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2b_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            blake2b(md, outlen, NULL, 0, in, inlen);
            blake2b_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            blake2b(md, outlen, key, outlen, in, inlen);
            blake2b_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // compute and compare the hash of hashes
    blake2b_final(md,&ctx);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2b_res[i])
            return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("blake2b_selftest() = %s\n",
         blake2b_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
