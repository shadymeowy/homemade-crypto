#ifndef CHACHA20_H
#define CHACHA20_H

#include <stdint.h>
#include <stddef.h>

struct chacha_ctx {
    uint32_t i;
    uint32_t in[16];
    uint32_t out[16];
};

void chacha_block(uint32_t out[16], uint32_t const in[16]);
void chacha_block_init(uint32_t block[16], uint32_t key[8], uint32_t nonce[3],
		       uint32_t counter);
void chacha_init(struct chacha_ctx *ctx, uint32_t key[8], uint32_t nonce[2]);
uint8_t chacha_next_byte(struct chacha_ctx *ctx);
uint32_t chacha_next_uint32(struct chacha_ctx *ctx);

#endif
