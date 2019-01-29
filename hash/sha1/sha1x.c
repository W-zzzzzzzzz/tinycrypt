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

    F(16)w[i]=rev32(c->x.w[i]);
    
    for(i=16;i<80;i++)
      w[i]=R(w[i-3]^w[i-8]^w[i-14]^w[i-16],1);
    
    F(5)x[i]=c->s[i];
    
    F(80) {
      if(i<20){
        t=FF(x[1],x[2],x[3])+0x5A827999L;
      } else if(i<40) {
        t=HH(x[1],x[2],x[3])+0x6ED9EBA1L;
      } else if(i<60) {
        t=GG(x[1],x[2],x[3])+0x8F1BBCDCL;
      } else {
        t=HH(x[1],x[2],x[3])+0xCA62C1D6L;
      }
      t+=R(x[0],5)+x[4]+w[i];
      x[4]=x[3];x[3]=x[2];x[2]=R(x[1],30);x[1]=x[0];x[0]=t;
    }
    F(5)c->s[i]+=x[i];
}

/************************************************
 *
 * initialize context
 *
 ************************************************/
void sha1_init(sha1_ctx*c) {
    c->s[0] = 0x67452301;
    c->s[1] = 0xefcdab89;
    c->s[2] = 0x98badcfe;
    c->s[3] = 0x10325476;
    c->s[4] = 0xc3d2e1f0;
    c->len  = 0;
}

void sha1_update(sha1_ctx*c,void*in,W len) {
    B *p=in;
    W r,i,idx;

    idx=c->len&63;
    c->len+=len;
    
    while(len) {
      r=len<(64-idx)?len:(64-idx);
      F(r)c->x.b[i]=p[i];
      if((idx+r)<64)break;
      sha1_compress(c);
      len-=r;
      idx=0;
      p+=r;
    }
}

void sha1_final(void*h,sha1_ctx*c) {
    W i,len,*p=h;
    
    len=c->len&63;
    F(64-len)c->x.b[len+i]=0;
    c->x.b[len]=0x80;
    if(len>=56) {
      sha1_compress(c);
      F(16)c->x.w[i]=0;
    }
    c->x.q[7]=rev64((Q)c->len*8);
    sha1_compress(c);
    F(5)p[i]=rev32(c->s[i]);
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
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890"
};

char *tv_hash[] =
{ "da39a3ee5e6b4b0d3255bfef95601890afd80709",
  "86f7e437faa5a7fce15d1ddcb9eaeaea377667b8",
  "a9993e364706816aba3e25717850c26c9cd0d89d",
  "c12252ceda8be8994d5fa0290a47231c1d16aae3",
  "32d10c7b8cf96570ca04ce37f2a19d84240d3a89",
  "761c457bf73b14d27e9e9265c46f4b4dda11f940",
  "50abf5706a150990a08b2c5ea40fa0e585554732"
};


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
    
    for (i=0; i<20; i++)
      printf(" %02X", x[i]);
    printf("\n");
}

int main(void) {
  
    uint8_t  h[20], r[20];
    int      i, fail=0;
    sha1_ctx c;
    
    for(i=0;i<sizeof(tv_hash)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      sha1_init(&c);
      sha1_update(&c, tv_str[i], strlen(tv_str[i]));
      sha1_final(r, &c);
      
      if(memcmp(h, r, 20)) {
        bin2hex(r);
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All SHA-1 tests passed\n");  
    return 0;
}
#endif
