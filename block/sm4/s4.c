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

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define F(a,b)for(a=0;a<b;a++)
#define X(x,y)t=x,x=y,y=t;
typedef unsigned char B;
typedef unsigned int W;
#define rev __builtin_bswap32

B AX(B x) {     
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
    x = AX(x);
    // multiplicative inverse
    // uses x^8 + x^7 + x^6 + x^5 + x^4 + x^2 + 1 as IRP
    if (x) {
      for(c=i=0,y=1;--i;y=(!c&&y==x)?c=1:y,y^=(y<<1)^(y>>7)*0xF5);
      x=y;
    }
    // affine transformation
    return AX(x);
}

void sm4(void*mk,void*in) {
    W *p,c,i,j,s,t,k[4],x[4];
    W fk[4]={0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};
    // load the key and plaintext
    F(i,4)k[i]=rev(((W*)mk)[i])^fk[i],x[i]=rev(((W*)in)[i]);
    // encrypt plaintext
    F(i,32) {
      // calculate round constant
      F(j,4)t<<=8,t|=((((i*4)+j)*7)&255);
      F(s,2) {
        p=(s)?x:k;
        // add round constant or sub key
        t=p[(i+1)%4]^p[(i+2)%4]^p[(i+3)%4]^t;
        // non-linear layer
        F(j,4)t=(t&-256)|S(t),t=R(t,8);
        // linear layer
        t=p[i%4]^=t^((s)?R(t,30)^R(t,22)^R(t,14)^R(t,8):R(t,19)^R(t,9));
      }
    }
    // swap
    X(x[0],x[3]);X(x[1],x[2]);
    // store ciphertext
    F(i,4)((W*)in)[i]=rev(x[i]);
}


/**

There are some operations within the SM4 cipher that I would argue aren't
necessary at all. First is the byte swap operation before and after
encryption. Second is the swap/permutation of values at the end of
encryption. Omission of this shouldn't affect the security of the cipher.

Despite this, we see similar operations used in various block ciphers. 
e.g Blowfish,Bel-T, DES. IMHO, it's just a wasted operation that achieves
nothing. Little-endian should be the preferred storage of data, because
almost 100% of the CPUs in circulation use little-endian mode. Not that
there's anything wrong with the 90s, but we're not in the 90s anymore.
This cipher was probably designed around the same time as AES.

It's a poorly designed cipher based on AES.
*/
