#include "chacha20.h"

#ifdef __cplusplus
extern "C" {
#endif

void chacha_init(struct chacha_ctx *ctx, uint32_t key[8], uint32_t nonce[3])
{
	ctx->i = 0;
	chacha_block_init(ctx->in, key, nonce, 1);
	chacha_block(ctx->out, ctx->in);
}

uint8_t chacha_next_byte(struct chacha_ctx *ctx)
{
	uint8_t out;

	if (ctx->i == 64) {
        ctx->in[12]++;
		chacha_block(ctx->out, ctx->in);
		ctx->i = 0;
	}
	out = ctx->out[ctx->i >> 2] >> ((ctx->i & 3) << 3);
	ctx->i++;

	return out;
}

uint32_t chacha_next_uint32(struct chacha_ctx *ctx)
{
	return chacha_next_byte(ctx) | chacha_next_byte(ctx) << 8 |
	       chacha_next_byte(ctx) << 16 | chacha_next_byte(ctx) << 24;
}

void chacha_encrypt(uint8_t *data, size_t len, uint32_t key[8], uint32_t nonce[3])
{
    struct chacha_ctx ctx;
    size_t i;

    chacha_init(&ctx, key, nonce);
    for (i = 0; i < len; i++) {
        data[i] ^= chacha_next_byte(&ctx);
    }
}

void chacha_decrypt(uint8_t *data, size_t len, uint32_t key[8], uint32_t nonce[3])
{
    chacha_encrypt(data, len, key, nonce);
}

void chacha_block_init(uint32_t block[16], uint32_t key[8], uint32_t nonce[3],
		       uint32_t counter)
{
	int i;

	if (block == NULL) {
		return;
	}

	block[0] = 0x61707865;
	block[1] = 0x3320646e;
	block[2] = 0x79622d32;
	block[3] = 0x6b206574;
	if (key != NULL) {
		for (i = 0; i < 8; i++) {
			block[i + 4] = key[i];
		}
	}
	block[12] = counter;
	if (nonce != NULL) {
		block[13] = nonce[0];
		block[14] = nonce[1];
		block[15] = nonce[2];
	}
}

#define ROTL(a, b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d)                                                         \
	a += b;                                                                \
	d ^= a;                                                                \
	d = ROTL(d, 16);                                                       \
	c += d;                                                                \
	b ^= c;                                                                \
	b = ROTL(b, 12);                                                       \
	a += b;                                                                \
	d ^= a;                                                                \
	d = ROTL(d, 8);                                                        \
	c += d;                                                                \
	b ^= c;                                                                \
	b = ROTL(b, 7);
#define ROUNDS 20

void chacha_block(uint32_t out[16], uint32_t const in[16])
{
	int i;
	uint32_t x[16];

	for (i = 0; i < 16; ++i) {
		x[i] = in[i];
	}
	for (i = 0; i < ROUNDS; i += 2) {
		QR(x[0], x[4], x[8], x[12]);
		QR(x[1], x[5], x[9], x[13]);
		QR(x[2], x[6], x[10], x[14]);
		QR(x[3], x[7], x[11], x[15]);

		QR(x[0], x[5], x[10], x[15]);
		QR(x[1], x[6], x[11], x[12]);
		QR(x[2], x[7], x[8], x[13]);
		QR(x[3], x[4], x[9], x[14]);
	}
	for (i = 0; i < 16; ++i) {
		out[i] = x[i] + in[i];
	}
}

#ifdef __cplusplus
}
#endif