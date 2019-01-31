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
    W s[16], h[8], idx, outlen;
    union {
      B b[128];
      W w[16];
    }x;
    W len;
}blake2b_ctx;

int blake2b_init(blake2b_ctx*c,W outlen,void*key,W keylen);
void blake2b_update(blake2b_ctx*,void*,W);
void blake2b_final(void*out,blake2b_ctx*c);

void G(W *s, W *m) {
    W i, j, a, b, c, d, r, t, *p, z;
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
      
    F(8)v[i]=c->s[i], v[i+8]=c->h[i];
    v[12]^=c->len;v[14]^=-last;
    G(v,c->x.w);
    F(8)c->s[i]^=v[i]^v[i+8];
}

int blake2b_init (blake2b_ctx*c,W outlen,void*key,W keylen) {
    W i;
    W iv[8] =
    { 0x6A09E667F3BCC908, 0xBB67AE8584CAA73B,
      0x3C6EF372FE94F82B, 0xA54FF53A5F1D36F1,
      0x510E527FADE682D1, 0x9B05688C2B3E6C1F,
      0x1F83D9ABFB41BD6B, 0x5BE0CD19137E2179 };
    
    if(outlen==0||outlen>64||keylen>64) return -1;
    
    F(8)c->h[i]=c->s[i]=iv[i];
    
    c->s[0]  ^= 0x01010000^(keylen<<8)^outlen;
    c->len    = 0;
    c->idx    = 0;
    c->outlen = outlen;
    
    if(keylen>0) {
      blake2b_update(c,key,keylen);
      c->idx=128;
    }
    return 0;
}

void blake2b_update(blake2b_ctx*c,void*in,W len) {
    B *p=in;
    W i;
    
    while(len) {
      if(c->idx==128) {
        c->len += 128;
        blake2b_compress(c,0);
        c->idx=0;
      }
      c->x.b[c->idx++] = *p++;
      --len;
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
{ "786a02f742015903c6c6fd852552d272912f4740e15847618a86e217f71f5419d25e1031afee585313896444934eb04b903a685b1448b755d56f701afe9be2ce",
  "333fcb4ee1aa7c115355ec66ceac917c8bfd815bf7587d325aec1864edd24e34d5abe2c6b1b5ee3face62fed78dbef802f2a85cb91d455a8f5249d330853cb3c",
  "ba80a53f981c4d0d6a2797b69f12f6e94c212f14685ac4b74b12bb6fdbffa2d17d87c5392aab792dc252d5de4533cc9518d38aa8dbf1925ab92386edd4009923",
  "3c26ce487b1c0f062363afa3c675ebdbf5f4ef9bdc022cfbef91e3111cdc283840d8331fc30a8a0906cff4bcdbcd230c61aaec60fdfad457ed96b709a382359a",
  "c68ede143e416eb7b4aaae0d8e48e55dd529eafed10b1df1a61416953a2b0a5666c761e7d412e6709e31ffe221b7a7a73908cb95a4d120b8b090a87d1fbedb4c",
  "99964802e5c25e703722905d3fb80046b6bca698ca9e2cc7e49b4fe1fa087c2edf0312dfbb275cf250a1e542fd5dc2edd313f9c491127c2e8c0c9b24168e2d50",
  "686f41ec5afff6e87e1f076f542aa466466ff5fbde162c48481ba48a748d842799f5b30f5b67fc684771b33b994206d05cc310f31914edd7b97e41860d77d282"};

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
    
    for (i=0; i<64; i++)
      printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
  
    uint8_t     h[64], r[64];
    int         i, fail=0;
    blake2b_ctx c;
    
    for(i=0;i<sizeof(tv_str)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      blake2b_init(&c, 64, NULL, 0);
      blake2b_update(&c, tv_str[i], strlen(tv_str[i]));
      blake2b_final(r, &c);
      
      if(memcmp(h, r, 64)) {
        bin2hex(r);
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All BLAKE2b tests passed\n");  
    return 0;
}
#endif
