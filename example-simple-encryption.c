// gcc chacha20.c example-simple-encryption.c -o example-simple-encryption

#include "chacha20.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	struct chacha_ctx ctx;
	uint32_t key[8] = { 0xf95fd5b4, 0x1783595e, 0x41f4cbcd, 0x8dc26a78,
			    0x2599184e, 0x97ccd768, 0xac531aae, 0x729781d3 };
	uint32_t nonce[3] = { 0x84133f22, 0x61ef4479, 0x6e3669dc };
	uint8_t data[] = "Hello, world!";
	size_t len = strlen(data);

	printf("Plaintext: %s\n", data);
	printf("Plaintext in hex: ");
	for (int i = 0; i < len; i++) {
		printf("%02hhX", data[i]);
	}
	printf("\n");

	chacha_encrypt(data, len, key, nonce);

	printf("Ciphertext in hex: ");
	for (int i = 0; i < len; i++) {
		printf("%02hhX", data[i]);
	}
	printf("\n");

	chacha_decrypt(data, len, key, nonce);

	printf("Plaintext: %s\n", data);
	printf("Plaintext in hex: ");
	for (int i = 0; i < len; i++) {
		printf("%02hhX", data[i]);
	}
	printf("\n");
	return 0;
}