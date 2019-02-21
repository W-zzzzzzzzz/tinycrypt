

#ifndef SHA256_H
#define SHA256_H

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
} sha256_ctx;

#ifdef __cplusplus
extern "C" {
#endif

void sha256_init(sha256_ctx *c);
void sha256_update(sha256_ctx *c, const void *in, W len);
void sha256_final(void *out, sha256_ctx *c);

#ifdef __cplusplus
}
#endif

#endif
