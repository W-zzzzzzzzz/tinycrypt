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
#define X(a,b)t=a,a=b,b=t

#define HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LO_NIBBLE(b) ((b) & 0x0F)

typedef unsigned char B;
typedef unsigned int W;

// substitution box
void subbytes (W *in, W idx) {
    B s[16], p[16], i, t, n, m, cy;
    
    B sbox[8][8] = 
    { { 0x83, 0x1F, 0x6A, 0xB5, 0xDE, 0x24, 0x07, 0xC9 },
      { 0xCF, 0x72, 0x09, 0xA5, 0xB1, 0x8E, 0xD6, 0x43 },
      { 0x68, 0x97, 0xC3, 0xFA, 0x1D, 0x4E, 0xB0, 0x25 },
      { 0xF0, 0x8B, 0x9C, 0x36, 0x1D, 0x42, 0x7A, 0xE5 },
      { 0xF1, 0x38, 0x0C, 0x6B, 0x52, 0xA4, 0xE9, 0xD7 },
      { 0x5F, 0xB2, 0xA4, 0xC9, 0x30, 0x8E, 0x6D, 0x17 },
      { 0x27, 0x5C, 0x48, 0xB6, 0x9E, 0xF1, 0x3D, 0x0A },
      { 0xD1, 0x0F, 0x8E, 0xB2, 0x47, 0xAC, 0x39, 0x65 }};

    for(i=0; i<16; i+=2) {
      t = sbox[idx%8][i/2];
      s[i+0] = LO_NIBBLE(t);
      s[i+1] = HI_NIBBLE(t);
    }
    
    // initial permutation
    for(n=0; n<16; n++) {
      for(m=0; m<8; m++) {
        cy = in[m%4] & 1;
        in[m%4] >>= 1;
        p[n] = (cy << 7) | (p[n] >> 1);
      }
    }
    
    for(i=0; i<16; i++)
      p[i]=(s[HI_NIBBLE(p[i])]<<4)|s[LO_NIBBLE(p[i])];

    // final permutation
    for(n=0; n<4; n++) {
      for(m=0; m<32; m++) {
        cy = ((W*)p)[n] & 1;
        ((W*)p)[n] >>= 1;
        in[m%4] = (cy << 31) | (in[m%4] >> 1);
      }
    }
}

void serpent(void*mk,void*in) {
    W i,j,s,t,rk[4],k[8],*x=in;

    // copy 256-bit master key to local buffer
    for(i=0;i<8;i++)k[i]=((W*)mk)[i];
      
    for(i=0;;) {
      // create a round key
      for(j=0;j<4;j++) {
        rk[j]=R((k[0]^k[3]^k[5]^k[7]^0x9e3779b9UL^i*4+j),21);
        for(s=0;s<7;s++) k[s]=k[s+1];
        k[7]=rk[j];
      }
      subbytes(rk,3-i);
      
      // add round key
      x[0]^=rk[0];x[1]^=rk[1];
      x[2]^=rk[2];x[3]^=rk[3];

      if(i==32)break;
      
      // non-linear layer
      subbytes(x,i);
      
      // linear layer
      if(++i!=32) {
        x[0]=R(x[0],19);
        x[2]=R(x[2],29);
        x[1]^=x[0]^x[2];
        x[3]^=x[2]^(x[0]<<3);
        x[1]=R(x[1],31);
        x[3]=R(x[3],25);
        x[0]^=x[1]^x[3];
        x[2]^=x[3]^(x[1]<<7);
        x[0]=R(x[0],27);
        x[2]=R(x[2],10);
      }
    }
}
