

// test unit for crc
// odzhan

#include "crc.h"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

crc_param tv16[]={
  {"CRC-16/CCITT-0",  0x1021, 0xFFFF, 0, 0, 0x0, 0x29B1},
  {"CRC-16/ARC",  0x8005, 0x0, 1, 1, 0x0, 0xBB3D},
  {"CRC-16/AUG-CCITT",  0x1021, 0x1D0F, 0, 0, 0x0, 0xE5CC},
  {"CRC-16/BUYPASS",  0x8005, 0x0, 0, 0, 0x0, 0xFEE8},
  {"CRC-16/CDMA2000",  0xC867, 0xFFFF, 0, 0, 0x0, 0x4C06},
  {"CRC-16/DDS-110",  0x8005, 0x800D, 0, 0, 0x0, 0x9ECF},
  {"CRC-16/DECT-R",  0x589, 0x0, 0, 0, 0x1, 0x7E},
  {"CRC-16/DECT-X",  0x589, 0x0, 0, 0, 0x0, 0x7F},
  {"CRC-16/DNP",  0x3D65, 0x0, 1, 1, 0xFFFF, 0xEA82},
  {"CRC-16/EN-13757",  0x3D65, 0x0, 0, 0, 0xFFFF, 0xC2B7},
  {"CRC-16/GENIBUS",  0x1021, 0xFFFF, 0, 0, 0xFFFF, 0xD64E},
  {"CRC-16/MAXIM",  0x8005, 0x0, 1, 1, 0xFFFF, 0x44C2},
  {"CRC-16/MCRF4XX",  0x1021, 0xFFFF, 1, 1, 0x0, 0x6F91},
  {"CRC-16/RIELLO",  0x1021, 0xB2AA, 1, 1, 0x0, 0x63D0},
  {"CRC-16/T10-DIF",  0x8BB7, 0x0, 0, 0, 0x0, 0xD0DB},
  {"CRC-16/TELEDISK",  0xA097, 0x0, 0, 0, 0x0, 0xFB3},
  {"CRC-16/TMS37157",  0x1021, 0x89EC, 1, 1, 0x0, 0x26B1},
  {"CRC-16/USB",  0x8005, 0xFFFF, 1, 1, 0xFFFF, 0xB4C8},
  {"CRC-A",  0x1021, 0xc6c6, 1, 1, 0x0, 0xBF05},
  {"CRC-16/KERMIT",  0x1021, 0x0, 1, 1, 0x0, 0x2189},
  {"CRC-16/MODBUS",  0x8005, 0xFFFF, 1, 1, 0x0, 0x4B37},
  {"CRC-16/X-25",  0x1021, 0xFFFF, 1, 1, 0xFFFF, 0x906E},
  {"CRC-16/XMODEM",  0x1021, 0x0, 0, 0, 0x0, 0x31C3} 
};

crc_param tv32[]={
  {"CRC-32",  0x04C11DB7L, 0xFFFFFFFFL, 1, 1, 0xFFFFFFFFL, 0xCBF43926L},
  {"CRC-32/BZIP2",  0x04C11DB7L, 0xFFFFFFFFL, 0, 0, 0xFFFFFFFFL, 0xFC891918L},
  {"CRC-32C",  0x1EDC6F41L, 0xFFFFFFFFL, 1, 1, 0xFFFFFFFFL, 0xE3069283L},
  {"CRC-32D",  0xA833982BL, 0xFFFFFFFFL, 1, 1, 0xFFFFFFFFL, 0x87315576L},
  {"CRC-32/JAMCRC",  0x04C11DB7L, 0xFFFFFFFFL, 1, 1, 0x00000000L, 0x340BC6D9L},
  {"CRC-32/MPEG-2",  0x04C11DB7L, 0xFFFFFFFFL, 0, 0, 0x00000000L, 0x0376E6E7L},
  {"CRC-32/POSIX",  0x04C11DB7L, 0x00000000L, 0, 0, 0xFFFFFFFFL, 0x765E7680L},
  {"CRC-32Q",  0x814141ABL, 0x00000000L, 0, 0, 0x00000000L, 0x3010BF7FL},
  {"CRC-32/XFER",  0x000000AFL, 0x00000000L, 0, 0, 0x00000000L, 0xBD0BE338L}
};

crc_param tv64[]={
{"CRC-64", 0x42F0E1EBA9EA3693L, 0x00000000L, 0, 0, 0x00000000L, 0x6C40DF5F0B497347L},
{"CRC-64/WE", 0x42F0E1EBA9EA3693L, 0xFFFFFFFFFFFFFFFFL, 0, 0, 0xFFFFFFFFFFFFFFFFL,0x62EC59E3F1A4F00AL},
{"CRC-64/XZ", 0x42F0E1EBA9EA3693L, 0xFFFFFFFFFFFFFFFFL, 1, 1, 0xFFFFFFFFFFFFFFFFL,0x995DC9BBDF1939FAL},
};

int main(int argc, char *argv[]) {
    char   *str;
    size_t len, i;
    
    if (argc != 2) {
      printf ("\nusage: test <string>\n");
      return 0;
    }
    
    str = argv[1];
    len = strlen(str);
    
    putchar('\n');
    for(i=0;i<sizeof(tv16)/sizeof(crc_param); i++) {
      printf("%-16s : %04" PRIX16 "\n", 
        tv16[i].str, crc16(str, len, &tv16[i]));
    }
    putchar('\n');
    for(i=0;i<sizeof(tv32)/sizeof(crc_param); i++) {
      printf("%-16s : %08" PRIX32 "\n", 
        tv32[i].str, crc32(str, len, &tv32[i]));
    }
    putchar('\n');
    for(i=0;i<sizeof(tv64)/sizeof(crc_param); i++) {
      printf("%-16s : %016" PRIX64 "\n", 
        tv64[i].str, crc64(str, len, &tv64[i]));
    }
    return 0;
}
