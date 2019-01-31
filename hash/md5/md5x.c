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

typedef struct _md5_ctx {
    W s[4];
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    Q len;
}md5_ctx;

#define FF(x,y,z)((z)^((x)&((y)^(z))))
#define GG(x,y,z)FF(z,x,y)
#define HH(x,y,z)((x)^(y)^(z))
#define II(x,y,z)((y)^((x)|(~z)))

void md5_compress(md5_ctx*ctx) {
    W a, b, c, d, i, t, s;
    B rotf[]={7,12,17,22};
    B rotg[]={5, 9,14,20};
    B roth[]={4,11,16,23};
    B roti[]={6,10,15,21};

    B sigma[]=
    { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
      1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
      5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
      0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9 };

    W tc[64] =
    { 0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 
      0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
      0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
      0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
      0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
      0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
      0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
      0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
      0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
      0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
      0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
      0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
      0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
      0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
      0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
      0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391 }; 
    
    a = ctx->s[0];
    b = ctx->s[1];
    c = ctx->s[2];
    d = ctx->s[3];

    for(i=0;i<64;i++) {
      if(i<16) {
        s=rotf[i%4];
        a+=FF(b,c,d);
      } else if(i<32) {
        s=rotg[i%4];
        a+=GG(b,c,d);
      } else if (i<48) {
        s=roth[i%4];
        a+=HH(b, c, d);
      } else {
        s=roti[i%4];
        a+=II(b,c,d);
      }
      a+=ctx->x.w[sigma[i]]+tc[i];
      a=R(a,s)+b;
      t=a;a=d;d=c;c=b;b=t;
    }
    ctx->s[0] += a;
    ctx->s[1] += b;
    ctx->s[2] += c;
    ctx->s[3] += d;
}

void md5_init(md5_ctx*c) {
    c->s[0]=0x67452301;
    c->s[1]=0xefcdab89;
    c->s[2]=0x98badcfe;
    c->s[3]=0x10325476;
    c->len =0;
}

void md5_update(md5_ctx*c,void*in,W len) {
    B *p=in;
    W r,i,idx;

    idx=c->len&63;
    c->len+=len;
    
    while(len) {
      r=len<(64-idx)?len:(64-idx);
      F(r)c->x.b[i]=p[i];
      if((idx+r)<64)break;
      md5_compress(c);
      len-=r;
      idx=0;
      p+=r;
    }
}

void md5_final(void*h,md5_ctx*c) {
    W i,len,*p=h;
    
    len=c->len&63;
    F(64-len)c->x.b[len+i]=0;
    c->x.b[len]=0x80;
    if(len>=56) {
      md5_compress(c);
      F(16)c->x.w[i]=0;
    }
    c->x.q[7]=c->len*8;
    md5_compress(c);
    F(4)p[i]=c->s[i];
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
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890" };

char *tv_hash[] =
{ "d41d8cd98f00b204e9800998ecf8427e",
  "0cc175b9c0f1b6a831c399e269772661",
  "900150983cd24fb0d6963f7d28e17f72",
  "f96b697d7cb7938d525a2f31aaf161d0",
  "c3fcd3d76192e4007dfb496cca67e13b",
  "d174ab98d277d9f5a5611c2c9f419d9f",
  "57edf4a22be3c955ac49da2e2107b67a" };


size_t hex2bin (void *bin, char hex[]) {
    size_t  len, i;
    int     x;
    B *p=(B*)bin;

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
      p[i] = (B)x;
    } 
    return len / 2;
} 

void bin2hex(B *x) {
    int i;
    
    for (i=0; i<16; i++)
      printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
  
    B h[16], r[16];
    int     i, fail=0;
    md5_ctx c;
    
    for(i=0;i<sizeof(tv_hash)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      md5_init(&c);
      md5_update(&c, tv_str[i], strlen(tv_str[i]));
      md5_final(r, &c);
      
      if(memcmp(h, r, 16)) {
        bin2hex(h);
        bin2hex(r);
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All MD5 tests passed\n");  
    return 0;
}
#endif
