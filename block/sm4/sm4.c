/**
  Copyright © 2017 Odzhan. All Rights Reserved.

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

#include "sm4.h"

B A(B x) {     
    B m=0xA7,s=0,t;
    do {
      for(t=x&m;t;t>>=1)s^=(t&1);
      s=(s>>1)|(s<<7);
      m=(m<<1)|(m>>7);
    } while(m!=0xA7);
    return s^0xD3; 
}

B S(B x) {
    B i, c, y;
    // affine transformation
    x = A(x);
    // multiplicative inverse
    // uses x^8 + x^7 + x^6 + x^5 + x^4 + x^2 + 1 as IRP
    if (x) {
      for(c=i=0,y=1;--i;y=(!c&&y==x)?c=1:y,y^=(y<<1)^((-(y>>7))&0xF5));
      x=y;
    }
    // affine transformation
    return A(x);
}

void sm4(void *mk, void *data) {
    W *p,c,i,j,s,x[8];
    W fk[4]={0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};
    // load the 128-bit key and 128-bit plaintext
    F(i,4)x[i+4]=rev(((W*)mk)[i])^fk[i],x[i]=rev(((W*)data)[i]);
    // encrypt plaintext
    F(i,32) {
      // calculate round constant
      F(j,4)c<<=8,c|=((((i*4)+(j))*7)&255);
      for(s=1;(int)s>=0;s--) {
        p=&x[s*4];
        // add round constant or sub key
        c^=p[(i+1)%4]^p[(i+2)%4]^p[(i+3)%4];
        // non-linear layer
        F(j,4)c=(c&-256)|S(c),c=R(c,8);
        // linear layer
        c=p[i%4]^=c^((s)?R(c,19)^R(c,9):R(c,30)^R(c,22)^R(c,14)^R(c,8));
      }
    }
    // store ciphertext
    F(i,4)((W*)data)[3-i]=rev(x[i]);
}
