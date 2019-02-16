
#include <openssl/sha.h>

#define sha256_init(x) SHA256_Init(x)
#define sha256_update(x,y,z) SHA256_Update(x,y,z)
#define sha256_final(x,y) SHA256_Final(x,y)

typedef SHA256_CTX sha256_ctx;

// test code written by Markku-Juhani O. Saarinen

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sha256(void *out, const void *in, size_t inlen)
{
    sha256_ctx ctx;

    sha256_init(&ctx);
    sha256_update(&ctx, in, inlen);
    sha256_final(out, &ctx);
}

// Deterministic sequences (Fibonacci generator).

static void selftest_seq(uint8_t *out, size_t len, uint32_t seed)
{
    size_t i;
    uint32_t t, a , b;

    a = 0xDEAD4BAD * seed;              // prime
    b = 1;

    for (i = 0; i < len; i++) {         // fill the buf
        t = a + b;
        a = b;
        b = t;
        out[i] = (t >> 24) & 0xFF;
    }
}


int sha256_selftest(void)
{
    // Grand hash of hash results.
    const uint8_t sha256_res[32] = {
       0x6c, 0x06, 0xc1, 0x3d, 0xf2, 0x4a, 0x1c, 0x8c,
       0x0a, 0x90, 0x63, 0xc7, 0x94, 0x46, 0x76, 0x22,
       0xbe, 0xae, 0x5b, 0x03, 0x31, 0xa2, 0x3f, 0x93,
       0x26, 0x15, 0x5a, 0xdb, 0x9c, 0x8b, 0xaa, 0xbd };
    // Parameter sets.
    const size_t s2_md_len[4] = { 16, 20, 28, 32 };
    const size_t s2_in_len[6] = { 0,  3,  64, 65, 255, 1024 };

    size_t i, j, outlen, inlen;
    uint8_t in[1024], md[32], key[32];
    sha256_ctx ctx;

    // 256-bit hash for testing.
    sha256_init(&ctx);

    for (i = 0; i < 4; i++) {
        outlen = s2_md_len[i];
        for (j = 0; j < 6; j++) {
            inlen = s2_in_len[j];

            selftest_seq(in, inlen, inlen);     // unkeyed hash
            sha256(md, in, inlen);
            sha256_update(&ctx, md, outlen);   // hash the hash

            selftest_seq(key, outlen, outlen);  // keyed hash
            sha256(md, in, inlen);
            sha256_update(&ctx, md, outlen);   // hash the hash
        }
    }

    // Compute and compare the hash of hashes.
    sha256_final(md, &ctx);
    for (i = 0; i < 32; i++) {
     // if (!(i&7)) putchar('\n');
     // printf(" 0x%02x,", md[i]);
      
      if (md[i] != sha256_res[i])
        return -1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    printf("sha256_selftest() = %s\n",
         sha256_selftest() ? "FAIL" : "OK");

    return 0;
}
