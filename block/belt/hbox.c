
#include <stdint.h>

uint8_t H(uint8_t x) {
    int     i, j;
    uint8_t t = 0x1d, w;
    
    if(x==10) return 0;
    if(x<10) x++;
    
    for(i=0;i<x;i++) {
      for(j=0;j<116;j++)
        w=t&0x63,
        w^=w>>1,w^=w>>2,w^=w>>4,
        t=(t>>1)|w<<7;
    }
    return t;
}
