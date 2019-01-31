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
#define ROTATE(a,b) (((a) >> (b)) | ((a) << (32 - b)))

#define F(a,b)for(a=0;a<b;a++)
typedef unsigned int W;
typedef unsigned char B;

void cube2(void*p)
{
  int i;
  int r;
  W y[16];
  W *x=p;

  for (r = 0;r < 16;++r) {
    for (i = 0;i < 16;++i) x[i + 16] += x[i];
    for (i = 0;i < 16;++i) y[i ^ 8] = x[i];
    for (i = 0;i < 16;++i) x[i] = ROTATE(y[i],25);
    for (i = 0;i < 16;++i) x[i] ^= x[i + 16];
    for (i = 0;i < 16;++i) y[i ^ 2] = x[i + 16];
    for (i = 0;i < 16;++i) x[i + 16] = y[i];
    for (i = 0;i < 16;++i) x[i + 16] += x[i];
    for (i = 0;i < 16;++i) y[i ^ 4] = x[i];
    for (i = 0;i < 16;++i) x[i] = ROTATE(y[i],21);
    for (i = 0;i < 16;++i) x[i] ^= x[i + 16];
    for (i = 0;i < 16;++i) y[i ^ 1] = x[i + 16];
    for (i = 0;i < 16;++i) x[i + 16] = y[i];
  }
}

void cube(void*p) {
    W   i,r,k,y[16],*s=p;
    int j;
    B   c=1;
    
    F(r,16) {
      for(k=25,j=8;j>0;k-=4,j-=4) {          
        F(i,16)s[i+16]+=s[i];
        F(i,16)y[i^j]=s[i];
        F(i,16)s[i]=R(y[i],k);
        F(i,16)s[i]^=s[i+16];
        F(i,16)y[i^(j>>2)]=s[i+16];
        F(i,16)s[i+16]=y[i];
      }
    }
}


#ifdef TEST

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *tv_state[]=
{"534D1C35DBE88600CCF525E53DE91A8C83425B17A53A1A8AAC00850AB08CB7C6FD40825F5132D1EE01BCB857C0C77A7614B76BF552C9E516E87AFC96AADABCB667261935DEB144A8378B1FEFE30A2977DC046EF05D2F5BD15FD7CAF3049DA9FEA013322093ADF1499686F6DCA85826AFA9362D186694C4118CD41D2A4EC18116",
  "18E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB218E3DAB27AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A267AC45A26"};

void bin2hex(char *str, uint8_t *x, int len) {
    int i;
    
    printf("%s : ", str);
    for (i=0; i<len; i++)
      printf("%02X", x[i]);
    printf("\n");
}

int main(void) {
    uint8_t s[128];
    int     i;
    
    memset(s, 0xFF, sizeof(s));
    
    F(i,10) cube(s);
    
    bin2hex("state", s, 64);
    return 0;
}

#endif
