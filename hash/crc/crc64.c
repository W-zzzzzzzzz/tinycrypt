
#include "crc.h"

// crc64 code derived from code here: https://crccalc.com/

static uint64_t crc_table[256];

static uint64_t reflect(uint64_t x) {
    uint64_t i, r = 0;
    
    for(i=0; i<64; i++) {
      if((x & (1ULL << i)) != 0) {
        r |= ((1ULL << (63 - i)));
      }
    }
    return r;
}
 
static void create_table(crc_param *p) {
    int      j;
    uint64_t i, r;
    
    for(i=0; i<256; i++) {
      r = (p->refin) ? reflect(i) : i << 56;

      for (j=0; j<8; j++) {
        if ((r & 0x8000000000000000ULL) != 0) {
          r = ((r << 1ULL) ^ p->poly);
        } else {
          r <<= 1ULL;
        }
      }
      crc_table[i] = (p->refout) ? reflect(r) : r;
    }
}

uint64_t crc64(const void *input, size_t len, crc_param *p) {
    uint64_t crc;
    uint8_t  *data=(uint8_t*)input;
    int      i;
    
    create_table(p);
    
    crc = p->refin ? reflect(p->iv) : p->iv;
    
    for(i=0; i<len; i++) {
      if (p->refout) {
        crc = (crc >> 8) ^ crc_table[(crc&0xFF)^data[i]];
      } else {
        crc = (crc << 8) ^ crc_table[(crc>>(64-8))^data[i]];  
      }
    }
    return (crc ^ p->xor);
}
