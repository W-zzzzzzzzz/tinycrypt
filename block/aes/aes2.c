
/**
  Copyright © 2015, 2017 Odzhan. All Rights Reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are
  met:

  1. Redistributions of source code must retain the above copyright
  notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

  3. The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY AUTHORS "AS IS" AND ANY EXPRESS OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

#include "aes.h"

#define RotWord(x) ROTR32(x, 8)

uint32_t XT (uint32_t w) {
    uint32_t t = w & 0x80808080;
    
    return ( (w ^ t ) << 1) ^ ( ( t >> 7) * 0x0000001B);
}

// ------------------------------------
// substitute byte
// ------------------------------------
uint8_t SubByte (uint8_t x)
{
    uint8_t i, y=x, sb;

    // multiplicative inverse    
    if (x) {
      // calculate logarithm gen 3
      for (i=1, y=1; i > 0; i++) {
        y ^= XT(y);
        if (y == x) break;
      }
      x = ~i;
      // calculate anti-logarithm gen 3
      for (i=0, y=1; i < x; i++) {
        y ^= XT(y);
      }
    }
    
    sb = y;

    for (i=0; i<4; i++) {
      y   = ROTL8(y, 1);
      sb ^= y;
    }
    return sb ^ 0x63;
}
// ------------------------------------
// substitute 4 bytes
// ------------------------------------
uint32_t SubWord (uint32_t x)
{
    int   i;
    w32_t r;

    r.w = x;
    
    for (i=0; i<4; i++) {
      r.b[i] = SubByte(r.b[i]);
    }
    return r.w;
}

// ------------------------------------
// create AES-256 key
// ------------------------------------
void aes_setkey (aes_ctx *ctx, void *key)
{
    int      i;
    uint32_t x;
    uint32_t *w=(uint32_t*)ctx->w;
    uint32_t rcon=1;

    memcpy(w, key, Nk*4);

    for (i=Nk; i<Nb*(Nr+1); i++)
    {
      x = w[i-1];
      if ((i % Nk)==0) {
        x = RotWord(x);
        x = SubWord(x) ^ rcon;
        rcon = XT(rcon);
      } else if ((i % Nk) == 4) {
        x = SubWord(x);
      }
      w[i] = w[i-Nk] ^ x;
    }
}

void aes_enc (void *data, void *key) {
    w128_t   *x, *k, v;
    uint32_t i, j, w, r;
    uint32_t rk[Nb*(Nr+1)];
    
    x=(w128_t*)data;
    k=(w128_t*)rk;
    
    // initialize round keys
    aes_setkey((aes_ctx*)&rk, key);
    
    // copy data to local buffer
    memcpy (&v, x, 16);
    r = 0;
    goto add_key;
    
    do { 
      // apply sub bytes and shift rows
      for (i=0; i<16; i++) {     
        j = ((i >> 2) + 4 - (i & 3) ) & 3;
        v.m[j][i & 3] = SubByte(x->b[i]);
      }
    
      // if not last round
      if (r != Nr) {
        // mix columns
        for (i=0; i<4; i++) {
          w = v.w[i];
          v.w[i] = ROTR32(w,  8) ^ 
                   ROTR32(w, 16) ^ 
                   ROTR32(w, 24) ^ 
                XT(ROTR32(w,  8) ^ w);
        }
      }
add_key:    
      // add round key
      for (i=0; i<4; i++) {
        x->w[i] = v.w[i] ^ k->w[i];
      }
      // advance to next round key
      k++;    
    } while (++r <= Nr);
}
