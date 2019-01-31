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
    W s[16], h[8], idx, outlen;
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

int blake2s_init(blake2s_ctx*c,W outlen,void*key,W keylen);
void blake2s_update(blake2s_ctx*,void*,W);
void blake2s_final(void*out,blake2s_ctx*c);

void G(W *s, W *m) {
    W i, j, a, b, c, d, r, t;
    W v[8]=
    { 0xC840, 0xD951, 0xEA62, 0xFB73,
      0xFA50, 0xCB61, 0xD872, 0xE943 };
    
    Q sigma[10] = 
    { 0xfedcba9876543210, 0x357b20c16df984ae,
      0x491763eadf250c8b, 0x8f04a562ebcd1397,
      0xd386cb1efa427509, 0x91ef57d438b0a6c2,
      0xb8293670a4def15c, 0xa2684f05931ce7bd,
      0x5a417d2c803b9ef6, 0x0dc3e9bf5167482a };
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
    
    F(8)v[i]=c->s[i], v[i+8]=c->h[i];
    v[12]^=c->len.w[0];v[13]^=c->len.w[1];v[14]^=-last;
    G(v,c->x.w);
    F(8)c->s[i]^=v[i]^v[i+8];
}

int blake2s_init (blake2s_ctx*c,W outlen,void*key,W keylen) {
    W i;
    W iv[8] =
    { 0x6A09E667, 0xBB67AE85, 0x3C6EF372, 0xA54FF53A,
      0x510E527F, 0x9B05688C, 0x1F83D9AB, 0x5BE0CD19 };
    
    if(outlen==0||outlen>32||keylen>32) return -1;
    
    F(8)c->h[i]=c->s[i]=iv[i];
    
    c->s[0]  ^= 0x01010000^(keylen<<8)^outlen;
    c->len.q  = 0;
    c->idx    = 0;
    c->outlen = outlen;
    
    if(keylen>0) {
      blake2s_update(c,key,keylen);
      c->idx=64;
    }
    return 0;
}

void blake2s_update(blake2s_ctx*c,void*in,W len) {
    B *p=in;
    W i;
    
    F(len) {
      if(c->idx==64) {
        c->len.q+= 64;
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

char *tv_str[] =
{ "",
  "a",
  "abc",
  "message digest",
  "abcdefghijklmnopqrstuvwxyz",
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};

char *tv_hash[] =
{ "69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9",
  "4a0d129873403037c2cd9b9048203687f6233fb6738956e0349bd4320fec3e90",
  "508c5e8c327c14e2e1a72ba34eeb452f37458b209ed63a294d999b4c86675982",
  "fa10ab775acf89b7d3c8a6e823d586f6b67bdbac4ce207fe145b7d3ac25cd28c",
  "bdf88eb1f86a0cdf0e840ba88fa118508369df186c7355b4b16cf79fa2710a12",
  "c75439ea17e1de6fa4510c335dc3d3f343e6f9e1ce2773e25b4174f1df8b119b",
  "fdaedb290a0d5af9870864fec2e090200989dc9cd53a3c092129e8535e8b4f66"};

char *tv_bin[] =
{ "",
  "00",
  "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f2021222324252627"
  "28292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f"
  "505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f7071727374757677"
  "78797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9f"
  "a0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7"
  "c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeef"
  "f0f1f2f3f4f5f6f7f8f9fafbfcfdfe"};

char *tv_key[] =
{ "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",
  "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f",
  "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f"};

char *tv_hash2[] =
{ "48a8997da407876b3d79c0d92325ad3b89cbb754d86ab71aee047ad345fd2c49",
  "40d15fee7c328830166ac3f918650f807e7e01e177258cdc0a39b11f598066f1",
  "3fb735061abc519dfe979e54c1ee5bfad0a9d858b3315bad34bde999efd724ddok"};


size_t hex2bin (void *bin, char hex[]) {
    size_t  len, i;
    int     x;
    uint8_t *p=(uint8_t*)bin;

    len = strlen (hex);

    if ((len & 1) != 0) {
      return 0; 
    }

    for (i=0; i<len; i++) {
      if (isxdigit((int)hex[i]) == 0) {
        return 0; 
      }
    }

    for (i=0; i<len / 2; i++) {
      sscanf (&hex[i * 2], "%2x", &x);
      p[i] = (uint8_t)x;
    } 
    return len / 2;
} 

void bin2hex(uint8_t *x) {
    int i;
    
    for (i=0; i<32; i++)
      printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
  
    uint8_t     h[32], r[32];
    int         i, fail=0;
    blake2s_ctx c;
    
    for(i=0;i<sizeof(tv_str)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      blake2s_init(&c, 32, NULL, 0);
      blake2s_update(&c, tv_str[i], strlen(tv_str[i]));
      blake2s_final(r, &c);
      
      if(memcmp(h, r, 32)) {
        bin2hex(r);
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All BLAKE2s tests passed\n");  
    return 0;
}
#endif
