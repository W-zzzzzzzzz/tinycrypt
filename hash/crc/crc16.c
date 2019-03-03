
#include "crc.h"

// crc16 code derived from code here: https://crccalc.com/

static uint16_t crc_table[256];

static uint16_t reflect(uint16_t x) {
    uint16_t r = 0;
    int      i;
    
    for(i=0; i<16; i++) {
      if((x & (1 << i)) != 0) {
        r |= ((1 << (15 - i)) & 0xFFFF);
      }
    }
    return r;
}
 
static void create_table(crc_param *p) {
    int      i, j;
    uint16_t r;
    
    for(i=0; i<256; i++) {
      r = (p->refin) ? reflect(i) : i << 8;

      for (j=0; j<8; j++) {
        if ((r & 0x8000) != 0) {
          r = ((r << 1) ^ p->poly);
        } else {
          r <<= 1;
        }
      }
      crc_table[i] = (p->refout) ? reflect(r) : r;
    }
}

uint16_t crc16(const void *input, size_t len, crc_param *p) {
    uint16_t crc;
    uint8_t  *data=(uint8_t*)input;
    int      i;
    
    create_table(p);
    
    crc = p->refin ? reflect(p->iv) : p->iv;
    
    for(i=0; i<len; i++) {
      if (p->refout) {
        crc = (crc >> 8) ^ crc_table[(crc&0xFF)^data[i]];
      } else {
        crc = (crc << 8) ^ crc_table[(crc>>8)^data[i]];  
      }
    }
    return (crc ^ p->xor);
}

