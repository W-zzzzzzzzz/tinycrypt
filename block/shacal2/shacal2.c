/**
  Copyright © 2018 Odzhan. All Rights Reserved.

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

void shacal2(void*mk,void*data) {
    W t1,t2,i,w[64],s[8],*k=mk,*x=data;
    
    W K[64]=
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
    
    // load master key
    F(16)w[i]=rev32(k[i]);
    // expand key
    for(i=16;i<64;i++)
      w[i]=SIG1(w[i-2])+w[i-7]+SIG0(w[i-15])+w[i-16];
    // load 256-bit plaintext
    F(8)s[i]=rev32(x[i]);
    // encrypt
    F(64) {
      t1=s[7]+EP1(s[4])+CH(s[4],s[5],s[6])+w[i]+K[i];
      t2=EP0(s[0])+MAJ(s[0],s[1],s[2]);
      s[7]=s[6],s[6]=s[5],s[5]=s[4],s[4]=s[3]+t1;
      s[3]=s[2],s[2]=s[1],s[1]=s[0],s[0]=t1+t2;
    }
    // store 256-bit ciphertext
    F(8)x[i]=rev32(s[i]);
}

#ifdef TEST
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

char *tv_key[]=
{ "80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "40000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "20000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "08000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
  "A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8"
  };

char *tv_pt[]=
{ "0000000000000000000000000000000000000000000000000000000000000000",
  "0000000000000000000000000000000000000000000000000000000000000000",
  "0000000000000000000000000000000000000000000000000000000000000000",
  "0000000000000000000000000000000000000000000000000000000000000000",
  "0000000000000000000000000000000000000000000000000000000000000000",
  "A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8A8"};
 
char *tv_ct[]=
{ "361AB6322FA9E7A7BB23818D839E01BDDAFDF47305426EDD297AEDB9F6202BAE",
  "F3BAF53E5301E08813F8BE6F651BB19E9722151FF15063BA42A6FEF7CF3BF3D7",
  "E485005217441B60EE5B48EE8AF924B268B6B952D7F593E6102AC83D7DA72838",
  "AE70E355CB7E26FF12421F46CDAD5CB98367FE0E86CC234EDF97481765CD1AD9",
  "00CECD0B01311F881018E7A20BCE169766C089D91FF161346C4E1BD122EA199F",
  "1374026DD442B1C1E0BA34570240F6A9E99781C8307A1544A9D3C91857C7E6E1"
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

int main(void) {
  
    // a key is 64-bytes or 512-bits
    // a block size is 32-bytes or 256-bits
  
    uint8_t    pt[32], ct[32], key[64];
    int        i, fail=0;
    sha256_ctx c1,c2;
    
    for(i=0;i<sizeof(tv_key)/sizeof(char*);i++) {
      hex2bin(key,tv_key[i]);
      hex2bin(pt, tv_pt[i]);
      hex2bin(ct, tv_ct[i]);
      
      shacal2(key,pt);
      
      if(memcmp(pt, ct, 32)) {
        printf ("Hash for test vector %i failed\n", i+1);
        fail++;
      }     
    }
    if(!fail) printf ("All SHACAL2 tests passed\n");  
    return 0;
}
#endif
