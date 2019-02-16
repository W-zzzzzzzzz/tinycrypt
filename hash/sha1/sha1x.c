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

#define rev32(x) __builtin_bswap32(x)
#define rev64(x) __builtin_bswap64(x)

typedef unsigned long long Q;
typedef unsigned int W;
typedef unsigned char B;

typedef struct _sha1_ctx {
    W s[5];
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    Q len;
}sha1_ctx;

#define FF(x,y,z)((z)^((x)&((y)^(z))))
#define GG(x,y,z)(((x)& (y))|((z)&((x)|(y))))
#define HH(x,y,z)((x)^(y)^(z))

void sha1_compress(sha1_ctx*c) {
    W t,i,w[80],x[5];

    // load 64-bytes in big endian byte order
    F(16)w[i]=rev32(c->x.w[i]);
    
    // expand buffer
    for(i=16;i<80;i++)
      w[i]=R(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);
    
    // load 160-bit state
    F(5)x[i]=c->s[i];
    
    // permute
    F(80) {
      if(i<20){
        t = FF(x[1],x[2],x[3])+0x5A827999L;
      } else if(i<40) {
        t = HH(x[1],x[2],x[3])+0x6ED9EBA1L;
      } else if(i<60) {
        t = GG(x[1],x[2],x[3])+0x8F1BBCDCL;
      } else {
        t = HH(x[1],x[2],x[3])+0xCA62C1D6L;
      }
      t+=R(x[0],5)+x[4]+w[i];
      x[4]=x[3];x[3]=x[2];x[2]=R(x[1],30);x[1]=x[0];x[0]=t;
    }
    // update state with result
    F(5)c->s[i]+=x[i];
}

void sha1_init(sha1_ctx*c) {
    c->s[0] = 0x67452301;
    c->s[1] = 0xefcdab89;
    c->s[2] = 0x98badcfe;
    c->s[3] = 0x10325476;
    c->s[4] = 0xc3d2e1f0;
    c->len  = 0;
}

void sha1_update(sha1_ctx*c,const void*in,W len) {
    B *p=(B*)in;
    W i, idx;
    
    idx = c->len & 63;
    c->len += len;
    
    for (i=0;i<len;i++) {
      c->x.b[idx]=p[i]; idx++;
      if(idx==64) {
        sha1_compress(c);
        idx=0;
      }
    }
}

void sha1_final(void*h,sha1_ctx*c) {
    W i,len,*p=h;
    
    i = len = c->len & 63;
    while(i<64) c->x.b[i++]=0;
    c->x.b[len]=0x80;
    if(len>=56) {
      sha1_compress(c);
      F(16)c->x.w[i]=0;
    }
    c->x.q[7]=rev64(c->len*8);
    sha1_compress(c);
    F(5)p[i]=rev32(c->s[i]);
}

#ifdef TEST

// this test code was originally written by Markku-Juhani O. Saarinen
// i've adapted to work with this

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
