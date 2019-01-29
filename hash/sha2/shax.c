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

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define F(n)for(i=0;i<n;i++)

#define rev32(x) __builtin_bswap32(x)
#define rev64(x) __builtin_bswap64(x)

typedef unsigned long long Q;
typedef unsigned int W;
typedef unsigned char B;

typedef struct _sha256_ctx {
    W s[8];
    union {
      B b[64];
      W w[16];
      Q q[8];
    }x;
    Q len;
}sha256_ctx;

#define CH(x,y,z)(((x)&(y))^(~(x)&(z)))
#define MAJ(x,y,z)(((x)&(y))^((x)&(z))^((y)&(z)))
#define EP0(x)(R(x,2)^R(x,13)^R(x,22))
#define EP1(x)(R(x,6)^R(x,11)^R(x,25))
#define SIG0(x)(R(x,7)^R(x,18)^((x)>>3))
#define SIG1(x)(R(x,17)^R(x,19)^((x)>>10))

void sha256_compress(sha256_ctx*c) {
    W t1,t2,i,j,w[64],x[8];
    W k[64]=
    { 0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 
      0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
      0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
      0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 
      0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 
      0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
      0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
      0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 
      0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 
      0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
      0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 }; 
    
    F(16)w[i]=rev32(c->x.w[i]);
    
    for(i=16;i<64;i++)
      w[i]=SIG1(w[i-2])+w[i-7]+SIG0(w[i-15])+w[i-16];
    
    F(8)x[i]=c->s[i];
    
    F(64) {
      t1=x[7]+EP1(x[4])+CH(x[4],x[5],x[6])+w[i]+k[i];
      t2=EP0(x[0])+MAJ(x[0],x[1],x[2]);x[7]=t1+t2;
      x[3]+=t1;
      t1=x[0];
      for(j=1;j<8;j++)t2=x[j],x[j]=t1,t1=t2;
      x[0]=t1;
    }
    F(8)c->s[i]+=x[i];
}

void sha256_init(sha256_ctx*c) {    
    c->s[0]=0x6a09e667;
    c->s[1]=0xbb67ae85;
    c->s[2]=0x3c6ef372;
    c->s[3]=0xa54ff53a;
    c->s[4]=0x510e527f;
    c->s[5]=0x9b05688c;
    c->s[6]=0x1f83d9ab;
    c->s[7]=0x5be0cd19;
    c->len =0;
}

void sha256_update(sha256_ctx*c,void*in,W len) {
    B *p=in;
    W r,i,idx;

    idx=c->len&63;
    c->len+=len;
    
    while(len) {
      r=len<(64-idx)?len:(64-idx);
      F(r)c->x.b[i]=p[i];
      if((idx+r)<64)break;
      sha256_compress(c);
      len-=r;
      idx=0;
      p+=r;
    }
}

void sha256_final(void*h,sha256_ctx*c) {
    W i,len,*p=h;
    
    len=c->len&63;
    F(64-len)c->x.b[len+i]=0;
    c->x.b[len]=0x80;
    if(len>=56) {
      sha256_compress(c);
      F(8)c->x.q[i]=0;
    }
    c->x.q[7]=rev64((Q)c->len*8);
    sha256_compress(c);
    F(8)p[i]=rev32(c->s[i]);
}

#ifdef TEST
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

char *tv_str[]=
{ "",
  "a",
  "abc",
  "message digest",
  "abcdefghijklmnopqrstuvwxyz",
  "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
  "12345678901234567890123456789012345678901234567890123456789012345678901234567890"};

char *tv_hash[] =
{ "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
  "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb",
  "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
  "f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650",
  "71c480df93d6ae2f1efad1447c66c9525e316218cf51fc8d9ed832f2daf18b73",
  "db4bfcbd4da0cd85a60c3c37d3fbd8805c77f15fc6b1fdfe614ee0a7c8fdb4c0",
  "f371bc4a311f2b009eef952dd83ca80e2b60026c8e935592d0f9c308453c813e"};


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

int main(void) {
  
    uint8_t    h[32], r[32];
    int        i, fail=0;
    sha256_ctx c;
    
    for(i=0;i<sizeof(tv_hash)/sizeof(char*);i++) {
      hex2bin(h, tv_hash[i]);
      
      sha256_init(&c);
      sha256_update(&c, tv_str[i], strlen(tv_str[i]));
      sha256_final(r, &c);
      
      if(memcmp(h, r, 32)) {
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All SHA-256 tests passed\n");  
    return 0;
}
#endif
