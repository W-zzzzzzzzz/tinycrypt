
#ifndef SHA1_H
#define SHA1_H

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
} sha1_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void sha1_init(sha1_ctx *c);
void sha1_update(sha1_ctx *c,const void *in,W len);
void sha1_final(void *h,sha1_ctx *c);

#ifdef __cplusplus
}
#endif

#endif
