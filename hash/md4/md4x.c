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

    F(4)s[i]=c->s[i];
    
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
    F(4)c->s[i]+=s[i];
}

void md4_init(md4_ctx*c) {
    c->s[0]=0x67452301;
    c->s[1]=0xefcdab89;
    c->s[2]=0x98badcfe;
    c->s[3]=0x10325476;
    c->len =0;
}

void md4_update(md4_ctx*c,void*in,W len) {
    B *p=in;
    W r,i,idx;

    idx=c->len&63;
    c->len+=len;
    
    while(len) {
      r=len<(64-idx)?len:(64-idx);
      F(r)c->x.b[i]=p[i];
      if((idx+r)<64)break;
      md4_compress(c);
      len-=r;
      idx=0;
      p+=r;
    }
}

void md4_final(void*h,md4_ctx*c) {
    W i,len,*p=h;
    
    len=c->len&63;
    F(64-len)c->x.b[len+i]=0;
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
{ "31d6cfe0d16ae931b73c59d7e0c089c0",
  "bde52cb31de33e46245e05fbdbd6fb24",
  "a448017aaf21d8525fc10ae87aa6729d",
  "d9130a8164549fe818874806e1c7014b",
  "d79e1c308aa5bbcdeea8ed63df412da9",
  "043f8582f241db351ce627e153e7f0e4",
  "e33b4ddc9c38f2199c3e7b164fcc0536" };


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
    
    for (i=0; i<16; i++)
      printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
  
    uint8_t h[16], r[16];
    int     i, fail=0;
    md4_ctx c;
    
    for(i=0;i<sizeof(tv_hash)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      md4_init(&c);
      md4_update(&c, tv_str[i], strlen(tv_str[i]));
      md4_final(r, &c);
      
      if(memcmp(h, r, 16)) {
        bin2hex(h);
        bin2hex(r);
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All MD4 tests passed\n");  
    return 0;
}
#endif
