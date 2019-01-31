/**
  Copyright © 2015, 2016 Odzhan. All Rights Reserved.

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

#define R(v,n)(((v)>>(n))|((v)<<(64-(n))))
#define F(a,b)for(a=0;a<b;a++)
  
typedef unsigned long long W;
typedef unsigned char B;

typedef struct _sha3_ctx {
    union {
      B b[200];
      W q[25];
    } s;
    int i,h,r;
}sha3_ctx;

void P(void*p) {
    W n,i,j,r,x,y,t,Y,b[5],*s=p;
    B c=1;

    F(n,24){
      F(i,5){b[i]=0;F(j,5)b[i]^=s[i+j*5];}
      F(i,5){
        t=b[(i+4)%5]^R(b[(i+1)%5],63);
        F(j,5)s[i+j*5]^=t;}
      t=s[1],y=r=0,x=1;
      F(j,24)
        r+=j+1,Y=(x*2)+(y*3),x=y,y=Y%5,
        Y=s[x+y*5],s[x+y*5]=R(t, -r),t=Y;
      F(j,5){
        F(i,5)b[i]=s[i+j*5];
        F(i,5)
          s[i+j*5]=b[i]^( b[(i+2)%5] &~ b[(i+1)%5]);}
      F(j,7)
        if((c=(c<<1)^((c>>7)*113))&2)
          *s^=1ULL<<((1<<j)-1);
    }
}

void sha3_init(sha3_ctx*c, int len) {
    W i;
    
    F(i,25)c->s.q[i]=0;
    
    c->h = len;
    c->r = 200-(2*len);
    c->i = 0;
}

void sha3_update(sha3_ctx*c, void*in, W len) {
    W i;
    B *p=in;
  
    F(i,len) {
      c->s.b[c->i++] ^= *p++;    
      if(c->i == c->r) {
        P(&c->s); 
        c->i=0;
      }
    }
}

void sha3_final(void*h, sha3_ctx*c) {
    B   *p=h;
    int i;
    
    c->s.b[c->i]^=6;
    c->s.b[c->r-1]^=0x80;
    P(&c->s);
    F(i,c->h)p[i]=c->s.b[i];
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
{ "a7ffc6f8bf1ed76651c14756a061d662f580ff4de43b49fa82d80a4b80f8434a",
  "80084bf2fba02475726feb2cab2d8215eab14bc6bdd8bfb2c8151257032ecd8b",
  "3a985da74fe225b2045c172d6bd390bd855f086e3e9d525b46bfe24511431532",
  "edcdb2069366e75243860c18c3a11465eca34bce6143d30c8665cefcfd32bffd",
  "7cab2dc765e21b241dbc1c255ce620b29f527c6d5e7f5f843e56288f0d707521",
  "a79d6a9da47f04a3b9a9323ec9991f2105d4c78a7bc7beeb103855a7a11dfb9f",
  "293e5ce4ce54ee71990ab06e511b7ccd62722b1beb414f5ff65c8274e0f5be1d" };

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
  
    uint8_t  h[32], r[32];
    int      i, fail=0;
    sha3_ctx c;
    
    for(i=0;i<sizeof(tv_hash)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      sha3_init(&c, 32);
      sha3_update(&c, tv_str[i], strlen(tv_str[i]));
      sha3_final(r, &c);
      
      if(memcmp(h, r, 32)) {
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All SHA3-256 tests passed\n");  
    return 0;
}
#endif
