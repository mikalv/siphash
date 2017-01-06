/**
  Copyright © 2017 Odzhan. All Rights Reserved.

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
  
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* default: SipHash-2-4 */
#define cROUNDS 2
#define dROUNDS 4

#define ROTL(x, b) (uint32_t)(((x) << (b)) | ((x) >> (32 - (b))))

void XSIPROUND(uint32_t v[4])
{
    v[0] += v[1];                                                              
    v[1] = ROTL(v[1], 5);                                                      
    v[1] ^= v[0];                                                              
    v[0] = ROTL(v[0], 16);                                                     
    v[2] += v[3];                                                              
    v[3] = ROTL(v[3], 8);                                                      
    v[3] ^= v[2];                                                              
    v[0] += v[3];                                                              
    v[3] = ROTL(v[3], 7);                                                      
    v[3] ^= v[0];                                                              
    v[2] += v[1];                                                              
    v[1] = ROTL(v[1], 13);                                                     
    v[1] ^= v[2];                                                              
    v[2] = ROTL(v[2], 16);                                                     
}

uint32_t hsh32(
    const void *data, 
    int inlen, 
    const void *key) 
{
    uint32_t v[4];
    int i, j, len, last;
    uint32_t m;
    uint8_t *in = (uint8_t*)data;
    uint32_t *k = (uint32_t*)key;
    
    uint32_t k0 = k[0];
    uint32_t k1 = k[1];
        
    uint32_t b = inlen << 24;
    
    v[0] = 0;
    v[1] = 0;
    v[2] = 0x6c796765;
    v[3] = 0x74656462;
    
    v[3] ^= k1;
    v[2] ^= k0;
    v[1] ^= k1;
    v[0] ^= k0;
   
    for (last=0;last!=1;)
    {
      // 1. are we less than 4 bytes?  
      len = (inlen < 4) ? inlen : 4;
      if (len < 4) {
        // 2. yes, move length << 24 into m
        m = b;
        last=1;
      } else {
        // 3. no, set m to zero
        m = 0;
      }
      // 4. while len is greater than or equal to zero
      while (--len >= 0) {
        // 5. get byte shifted left by 8 * len
        // this is only performed when input available
        if (last) {
          m |= ((uint32_t)in[len]) << (8 * len);
        } else {
          m |= ((uint32_t)*in) << (8 * len);
          in++;
          inlen--;
        }
      }
      // 6. swap bytes
      if (!last) m = _byteswap_ulong(m);
      
      //printf ("\nv2: %08X", m);
      
      v[3] ^= m;

      for (i=0; i<cROUNDS; i++) {
        XSIPROUND(v);
      }

      v[0] ^= m;
    }
      
    v[2] ^= 0xff;

    for (i = 0; i < dROUNDS; ++i)
        XSIPROUND(v);

    return v[1] ^ v[3];
}
