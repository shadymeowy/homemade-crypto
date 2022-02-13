#ifndef BLAKE2S_H
#define BLAKE2S_H

#include <stdint.h>
#include <stddef.h>

void blake2s(uint32_t *M, size_t len, uint32_t out[8]);

#endif
