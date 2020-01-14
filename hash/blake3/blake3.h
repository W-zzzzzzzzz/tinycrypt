


#ifndef BLAKE3_H
#define BLAKE3_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define R(v,n)(((v)>>(n))|((v)<<(32-(n))))
#define X(a,b)(t)=(a),(a)=(b),(b)=(t)
#define F(n)for(i=0;i<n;i++)

typedef unsigned char B;
typedef unsigned int W;
typedef unsigned long long Q;

#define WORDLEN  32
#define BLOCKLEN 64
#define ROUNDS   8*7
#define ROTATION 0x07080C10
#define OUTLEN   32
#define KEYLEN   32

#define BLAKE3_KEY_LEN 32
#define BLAKE3_OUT_LEN 32
#define BLAKE3_BLOCK_LEN 64

typedef struct _blake3_ctx {
    W s[16], idx, outlen;
    union {
      B b[BLOCKLEN];
      W w[BLOCKLEN/(WORDLEN/8)];
    }x;
    Q ctr;
} blake3_ctx;

#ifdef __cplusplus
extern "C" {
#endif

int blake3_init(blake3_ctx*,W,const void*,W);
void blake3_update(blake3_ctx*,const void*,W);
void blake3_final(void*,blake3_ctx*);

#ifdef __cplusplus
}
#endif

#endif
