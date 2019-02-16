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

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define X(a,b)(t)=(a),(a)=(b),(b)=(t)
#define F(n)for(i=0;i<n;i++)

typedef unsigned long long Q;
typedef unsigned int W;
typedef unsigned char B;

typedef struct _blake2s_ctx {
    W s[16], idx, outlen;
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    union {
      W w[2];
      Q q;
    }len;
}blake2s_ctx;

int blake2s_init(blake2s_ctx*,W,const void*,W);
void blake2s_update(blake2s_ctx*,const void*,W);
void blake2s_final(void*,blake2s_ctx*);

    W v[8]=
    { 0xC840, 0xD951, 0xEA62, 0xFB73,
      0xFA50, 0xCB61, 0xD872, 0xE943 };
    
    Q sigma[10] = 
    { 0xfedcba9876543210, 0x357b20c16df984ae,
      0x491763eadf250c8b, 0x8f04a562ebcd1397,
      0xd386cb1efa427509, 0x91ef57d438b0a6c2,
      0xb8293670a4def15c, 0xa2684f05931ce7bd,
      0x5a417d2c803b9ef6, 0x0dc3e9bf5167482a };
      
    W iv[8] =
    { 0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
      0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19 };
      
void G(W *s, W *m) {
    W i, j, a, b, c, d, r, t;

    Q *p=sigma, z;
    
    for(i=0;i<80;) {
      z=*p++;
      while(z) {
        d=v[i++%8];
        a=(d&15);b=(d>>4&15);
        c=(d>>8&15);d>>=12;
        r=0x07080C10;
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
      
void blake2s_compress(blake2s_ctx*c, W last) {
    W i, v[16];
    
    F(8)v[i]=c->s[i], v[i+8] = iv[i];
    v[12]^=c->len.w[0];v[13]^=c->len.w[1];v[14]^=-last;
    G(v,c->x.w);
    F(8)c->s[i]^=v[i]^v[i+8];
}
      
int blake2s_init (blake2s_ctx*c,W outlen,const void*key,W keylen) {
    W i;
    
    if(outlen == 0 || outlen > 32 || keylen > 32) return -1;
    
    // initialize iv
    F(8)c->s[i]=iv[i];
    
    c->s[0]  ^= 0x01010000 ^ (keylen << 8) ^ outlen;
    c->len.q  = 0;
    c->idx    = 0;
    c->outlen = outlen;
    
    for(i=keylen;i<64;i++) c->x.b[i]=0;
    
    if(keylen > 0) {
      blake2s_update(c, key, keylen);
      c->idx = 64;
    }
    
    return 0;
}

void blake2s_update(blake2s_ctx*c,const void*in,W len) {
    B *p=(B*)in;
    W i;
    
    F(len) {
      if(c->idx==64) {
        c->len.q += 64;
        blake2s_compress(c,0);
        c->idx=0;
      }
      c->x.b[c->idx++] = *p++;
    }
}

void blake2s_final(void*out, blake2s_ctx*c) {
    W i;
    
    c->len.q += c->idx;
    while(c->idx<64) c->x.b[c->idx++]=0;
    blake2s_compress(c,1);
    F(c->outlen)((B*)out)[i]=((B*)c->s)[i];
}


#ifdef TEST

// test code written by Markku-Juhani O. Saarinen

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int blake2s(void *out, size_t outlen,
    const void *key, size_t keylen,
    const void *in, size_t inlen)
{
    blake2s_ctx ctx;

    if (blake2s_init(&ctx, outlen, key, keylen))
        return -1;
    blake2s_update(&ctx, in, inlen);
    blake2s_final(out, &ctx);

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


int blake2s_selftest(void)
{
    // Grand hash of hash results.
    const uint8_t blake2s_res[32] = {
        0x6A, 0x41, 0x1F, 0x08, 0xCE, 0x25, 0xAD, 0xCD,
        0xFB, 0x02, 0xAB, 0xA6, 0x41, 0x45, 0x1C, 0xEC,
        0x53, 0xC5, 0x98, 0xB2, 0x4F, 0x4F, 0xC7, 0x87,
        0xFB, 0xDC, 0x88, 0x79, 0x7F, 0x4C, 0x1D, 0xFE
    };
    // Parameter sets.
    const size_t b2s_md_len[4] = { 16, 20, 28, 32 };
    const size_t b2s_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32], key[32];
    blake2s_ctx ctx;

    // 256-bit hash for testing.
    if (blake2s_init(&ctx, 32, NULL, 0))
        return -1;

    for (i = 0; i < 4; i++) {
        outlen = b2s_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = b2s_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            blake2s(md, outlen, NULL, 0, in, inlen);
            blake2s_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            blake2s(md, outlen, key, outlen, in, inlen);
            blake2s_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // Compute and compare the hash of hashes.
    blake2s_final(md, &ctx);
    for (i = 0; i < 32; i++) {
        if (md[i] != blake2s_res[i])
            return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("blake2s_selftest() = %s\n",
         blake2s_selftest() ? "FAIL" : "OK");

    return 0;
}

#endif
