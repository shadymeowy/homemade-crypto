#include <stdint.h>
#include <stddef.h>

int SIGMA[10][16] = {
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },
	{ 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 },
	{ 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
	{ 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 },
	{ 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 },
	{ 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
	{ 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 },
	{ 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 },
	{ 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
	{ 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 },
};

static const uint32_t IV[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
				0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

#define ROTL(a, b) (((a) >> (b)) | ((a) << (32 - (b))))
#define QR(a, b, c, d, x, y)                                                   \
	a += b;                                                                \
	a += x;                                                                \
	d ^= a;                                                                \
	d = ROTL(d, 16);                                                       \
	c += d;                                                                \
	b ^= c;                                                                \
	b = ROTL(b, 12);                                                       \
	a += b;                                                                \
	a += y;                                                                \
	d ^= a;                                                                \
	d = ROTL(d, 8);                                                        \
	c += d;                                                                \
	b ^= c;                                                                \
	b = ROTL(b, 7);
#define ROUNDS 10

static void _blake2s_compress(uint32_t out[8], uint32_t *M, uint64_t compressed,
			      uint64_t len, int f)
{
	size_t i;
	uint32_t v[16];
	uint32_t m[16] = { 0 };

	for (i = 0; i < 8; i++) {
		v[i] = out[i];
	}
	for (i = 0; i < 8; i++) {
		v[i + 8] = IV[i];
	}
	v[12] ^= compressed;
	v[13] ^= compressed >> 32;
	if (f) {
		v[14] ^= 0xFFFFFFFF;
	}

	for (i = 0; i < len >> 2; i++) {
		m[i] = M[i];
	}

	for (i = 0; i < ROUNDS; i++) {
		QR(v[0], v[4], v[8], v[12], m[SIGMA[i][0]], m[SIGMA[i][1]]);
		QR(v[1], v[5], v[9], v[13], m[SIGMA[i][2]], m[SIGMA[i][3]]);
		QR(v[2], v[6], v[10], v[14], m[SIGMA[i][4]], m[SIGMA[i][5]]);
		QR(v[3], v[7], v[11], v[15], m[SIGMA[i][6]], m[SIGMA[i][7]]);

		QR(v[0], v[5], v[10], v[15], m[SIGMA[i][8]], m[SIGMA[i][9]]);
		QR(v[1], v[6], v[11], v[12], m[SIGMA[i][10]], m[SIGMA[i][11]]);
		QR(v[2], v[7], v[8], v[13], m[SIGMA[i][12]], m[SIGMA[i][13]]);
		QR(v[3], v[4], v[9], v[14], m[SIGMA[i][14]], m[SIGMA[i][15]]);
	}
	for (i = 0; i < 8; i++) {
		out[i] ^= v[i] ^ v[i + 8];
	}
}

void blake2s(uint32_t *M, size_t len, uint32_t out[8])
{
	uint64_t compressed = 0;
	uint64_t remaining = len;
	for (int i = 0; i < 8; i++) {
		out[i] = IV[i];
	}
	out[0] ^= 0x01010020;

	while (remaining > 64) {
		compressed += 64;
		remaining -= 64;
		_blake2s_compress(out, M, compressed, 64, 0);
		M += 16;
	}
	compressed += remaining;
	_blake2s_compress(out, M, compressed, remaining, 1);
}