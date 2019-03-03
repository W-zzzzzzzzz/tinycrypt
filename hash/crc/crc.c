
#include "crc.h"

// crc32 code derived from code here: https://crccalc.com/

uint32_t crc_table[256];

uint32_t reflectx(uint32_t x) {
    uint32_t r = 0;
    int      i;
    
    for(i=0; i<32; i++) {
      if((x & (1 << i)) != 0) {
        r |= ((1 << (31 - i)) & 0xFFFFFFFF);
      }
    }
    return r;
}

uint32_t reflect(uint32_t x) {
    x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
    x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
    x = ((x & 0x0F0F0F0F) << 4) | ((x >> 4) & 0x0F0F0F0F);
    x = (x << 24) | ((x & 0xFF00) << 8) | ((x >> 8) & 0xFF00) | (x >> 24);

    return x;
}
    
void create_table(crc_param *p) {
    int      i, j;
    uint32_t r;
    
    for(i=0; i<256; i++) {
      r = (p->refin) ? reflect(i) : i << 24;

      for (j=0; j<8; j++) {
        if ((r & 0x80000000) != 0) {
          r = ((r << 1) ^ p->poly);
        } else {
          r <<= 1;
        }
      }
      crc_table[i] = (p->refout) ? reflect(r) : r;
    }
}

uint32_t crc32(const void *input, size_t len, crc_param *p) {
    uint32_t crc;
    uint8_t  *data=(uint8_t*)input;
    int      i;
    
    create_table(p);
    
    crc = p->refin ? reflect(p->iv) : p->iv;
    
    for(i=0; i<len; i++) {
      if (p->refout) {
        crc = (crc >> 8) ^ crc_table[(crc&0xFF)^data[i]];
      } else {
        crc = (crc << 8) ^ crc_table[(crc>>24)^data[i]];  
      }
    }
    return (crc ^ p->xor);
}
