
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Nk 8      // key length in words
#define Nr 14     // number of rounds for 256-bit
#define Nb 4      // number of words in each block

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define F(n)for(i=0;i<n;i++)
typedef unsigned char B;
typedef unsigned int W;

W aes256_tv[] = {
  0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
  0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c,
  0x9fc273a5, 0x98c476a1, 0x93ce7fa9, 0x9cc072a5,
  0xcda85116, 0xdabe4402, 0xc1a45d1a, 0xdeba4006,
  0xf0df87ae, 0x681bf10f, 0xfbd58ea6, 0x6715fc03,
  0x48f1e16d, 0x924fa56f, 0x53ebf875, 0x8d51b873,
  0x7f8256c6, 0x1799a7c9, 0xec4c296f, 0x8b59d56c,
  0x753ae23d, 0xe7754752, 0xb49ebf27, 0x39cf0754,
  0x5f90dc0b, 0x48097bc2, 0xa44552ad, 0x2f1c87c1,
  0x60a6f545, 0x87d3b217, 0x334d0d30, 0x0a820a64,
  0x1cf7cf7c, 0x54feb4be, 0xf0bbe613, 0xdfa761d2,
  0xfefa1af0, 0x7929a8e7, 0x4a64a5d7, 0x40e6afb3,
  0x71fe4125, 0x2500f59b, 0xd5bb1388, 0x0a1c725a,
  0x99665a4e, 0xe04ff2a9, 0xaa2b577e, 0xeacdf8cd,
  0xcc79fc24, 0xe97909bf, 0x3cc21a37, 0x36de686d };

// Multiplication over GF(2**8)
W M(W x){
    W t=x&0x80808080;
    return((x^t)*2)^((t>>7)*27);
}
// SubByte
B S(B x){
    B i,y,c;
    if(x){
      for(c=i=0,y=1;--i;y=(!c&&y==x)?c=1:y,y^=M(y));
      x=y;F(4)x^=y=(y<<1)|(y>>7);
    }
    return x^99;
}
void E(B *s){
    W i,j=0,w,x[12],c=1,*k=(W*)&x[4];

    // copy plain text + 256-bit master key to x
    F(12)x[i]=((W*)s)[i];

    for(;;){
      // 1st part of ExpandKey, AddRoundKey
      w=k[j%2?3:7];
      F(4)w=(w&-256)|S(w),w=R(w,8),((W*)s)[i]=x[i]^k[(j%2)*4+i]; 
      
      // AddConstant, update constant
      if(!(j%2))w=R(w,8)^c,c=M(c);

      // perform 2nd part of ExpandKey
      F(4)w=k[(j%2)*4+i]^=w;
      
      // if round 15, stop
      if (j==14) break;
      // update j
      j++;
      
      // SubBytes and ShiftRows
      F(16)((B*)x)[(i%4)+(((i/4)-(i%4))%4)*4]=S(s[i]);

      // if not round 15, MixColumns
      if(j!=14)
        F(4)w=x[i],x[i]=R(w,8)^R(w,16)^R(w,24)^M(R(w,8)^w);
    }
}
/**
  for(j=0; j<Nr+1; j++) {
    F(4)t->w[i] = k[(j%2)*4+i];
    t++;
    w = k[j%2?3:7];
    F(4)w=(w&-256)|S(w),w=R(w,8);
    if(!(j%2)) w=R(w,8)^c, c=M(c);
    F(4)w=k[(j%2)*4+i]^=w;
  }
  */
// ------------------------------------
// substitute 4 bytes
// ------------------------------------
W SubWord (W x) {
    W i, r=0;

    for (i=0; i<4; i++) {
      r |= S(x & 255);
      r  = R(r, 8);
      x >>= 8;
    }
    return r;
}

// ------------------------------------
// create AES-256 key
// ------------------------------------
void aes_setkey (void *mk, void *sk) {
    int i;
    W   x, *w=(W*)sk;
    B   c=1;

    for(i=0; i<Nk; i++) {
      w[i]=((W*)mk)[i];
    }

    for(i=Nk; i<Nb*(Nr+1); i++) {
      x = w[i-1];
      if ((i % Nk)==0) {
        x = R(x, 8);
        x = SubWord(x) ^ c;
        c = M(c);
      } else if ((Nk > 6) && ((i % Nk) == 4)) {
        x=SubWord(x);
      }
      w[i] = w[i-Nk] ^ x;
    }
}

int main(void) {
  
  union {
    B b[32];
    W w[8];
  } mkey;
  
  typedef union {
    B b[16];
    W w[4];
  } skey;
  
  B c=1, buf[16*15];
  W w, equ, k[8];
  struct {
    B s[16];
    B k[32];
  }p;
  
  skey *t=(skey*)buf;
  
  int i, j;
  
  for(i=0;i<16;i++) p.s[i]=i*16+i;;
  for(i=0;i<32;i++) p.k[i]=i;
  
  E((B*)&p);
  
  printf("\n");
  printf("tst: 8e a2 b7 ca 51 67 45 bf ea fc 49 90 4b 49 60 89\n");
  
  for(i=0;i<16;i++) printf(" %02x", p.s[i]);
  printf("\n");
  exit(0);
  
  //aes_setkey(&mkey, k);
  
  for(j=0; j<Nr+1; j++) {
    F(4)t->w[i] = k[(j%2)*4+i];
    t++;
    w = k[j%2?3:7];
    F(4)w=(w&-256)|S(w),w=R(w,8);
    if(!(j%2)) w=R(w,8)^c, c=M(c);
    F(4)w=k[(j%2)*4+i]^=w;
  }
  
  t=(skey*)buf;
  
  for(i=0;i<Nb*(Nr+1); i++) {
    if((i % 4)==0) putchar('\n');
    printf(" %08X", t->w[i]);
  }
  equ = (memcmp((void*)t, aes256_tv, sizeof(aes256_tv)) == 0);
  
  printf("\n\nTest %s\n", equ?"OK":"FAILED");
  return 0;
}

/**
  for(j=Nk; j<Nb*(Nr+1); j++) {
    w=k->w[j-1];
    
    if ((j % Nk) == 0) {
      printf("A: j is %i\n", j);
      F(4)w=(w&-256)|S(w),w=R(w,8);
      w = R(w, 8) ^ c;
      c = M(c);
    } else if ((j % Nk) == 4) {
      printf("B: j is %i\n", j);
      F(4)w=(w&-256)|S(w),w=R(w,8);
    }
    k->w[j] = k->w[j-Nk] ^ w;
    //k++;
  }
  
  for(j=0; j<Nr+1; j++) {
    F(4)t->w[i] = k[ ((j & 1) * 4) + i];
    t++;
    
    w = k[(j & 1) == 0 ? 7 : 3];
    
    w = SubWord(w);
    
    if((j & 1) == 0) {
      w = R(w, 8) ^ c;
      c = M(c);
    }
    
    F(4)w = k[((j & 1)*4)+i] ^= w;
  }
  
*/
