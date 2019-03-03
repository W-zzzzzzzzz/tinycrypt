
#include "crc.h"

// crc code derived from code here: https://crccalc.com/

static uint64_t crc_table[256];

static uint64_t reflect(uint64_t bitlen, uint64_t x) {
    uint64_t i, r = 0;
    
    for(i=0; i<bitlen; i++) {
      if((x & (1ULL << i)) != 0) {
        r |= ((1ULL << (bitlen - i - 1)));
      }
    }
    return r;
}
 
static void create_table(crc_param *p) {
    int      j;
    uint64_t i, r, m=~0ULL;
    
    if(p->bitlen<64) m = (1ULL << p->bitlen) - 1;
    
    for(i=0; i<256; i++) {
      r = (p->refin) ? reflect(p->bitlen, i) : i << (p->bitlen - 8);

      for (j=0; j<8; j++) {
        if (r & (1ULL << (p->bitlen - 1))) {
          r = ((r << 1ULL) ^ p->poly);
        } else {
          r <<= 1ULL;
        }
      }
      r = (p->refout) ? reflect(p->bitlen, r) : r;
      crc_table[i] = (r & m);
    }
}

uint64_t crc(const void *input, size_t len, crc_param *p) {
    uint64_t crc, m=~0ULL;
    uint8_t  *data=(uint8_t*)input;
    int      i;
    
    if(p->bitlen<64) m = (1ULL << p->bitlen) - 1;

    create_table(p);
    
    crc = p->refin ? reflect(p->bitlen, p->iv) : p->iv;
    
    for(i=0; i<len; i++) {
      if (p->refout) {
        crc = (crc >> 8) ^ crc_table[(crc&0xFF)^data[i]];
      } else {
        crc = (crc << 8) ^ crc_table[(crc>>(p->bitlen-8))^data[i]];  
      }
      crc &= m;
    }
    return (crc ^ p->xor) & m;
}
