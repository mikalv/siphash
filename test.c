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
#include <stdlib.h>

uint32_t hsh32(const void *in, const size_t inlen, const void *key);
int halfsiphash(const uint8_t *in, const size_t inlen, const uint8_t *k,
        uint8_t *out, const size_t outlen);
 
int sh32x(
  const uint8_t *data, 
  const size_t inlen, 
  const uint8_t *key);

int shp(
  const uint8_t *data, 
  const size_t inlen, 
  const uint8_t *key);

int shx(
  const uint8_t *data, 
  const size_t inlen, 
  const uint8_t *key);
  
int main (int argc, char *argv[])
{
  uint32_t h;
  uint8_t sh[8];
  
  if (argc!=3) {
    printf ("\nhsh32 <string> <key>\n");
    return 0;
  }
  
  h = hsh32x(argv[1], strlen(argv[1]), argv[2]);

  printf("\n");
    
  halfsiphash(argv[1], strlen(argv[1]), argv[2], sh, 32);
  
  printf ("\n%08X = %08X | 92EA1052 or 435353F5", h, ((uint32_t*)sh)[0]);
  
  return 0;
}
