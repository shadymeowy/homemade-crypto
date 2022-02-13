#include "blake2s.h"
#include <stdio.h>

void test(uint32_t *M, size_t len, uint32_t *out)
{
	/*for (int i = 0; i < 16; i++) {
		printf("%08x", M[i]);
	}
	printf("\n");*/

	blake2s(M, len, out);

	for (int i = 0; i < 8; i++) {
		printf("%08x ", out[i]);
	}
	printf("\n");
}

int main()
{
	uint32_t M[52] = { 0 };
	uint32_t out[8] = { 0 };
    test(M, 0, out);
    test(M, 16, out);
    test(M, 20, out);
    test(M, 64, out);
    test(M, 68, out);
    test(M, 196, out);
}