#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <tools/debug.h>

int main(int argc, const char *argv[])
{
	int rc;
	char tmp[4] = {0x00,0x00,0x00,0x01};
	unsigned char md[16];
	unsigned int mdl;
	EVP_MD_CTX *ctx;
	ctx = EVP_MD_CTX_new();

	rc = EVP_DigestInit_ex(ctx, EVP_md5(), NULL);
	if (rc != 1) {
		printf("Digest failed: Init\n");
		goto out;
	}

	rc = EVP_DigestUpdate(ctx, tmp, 4);
	if (rc != 1) {
		printf("Digest failed: Update\n");
		goto out;
	}

	rc = EVP_DigestFinal_ex(ctx, md, &mdl);
	if (rc != 1) {
		printf("Digest failed: Final\n");
		goto out;
	}

	print_hex_dump_debug("md5:", md, mdl);
out:

	EVP_MD_CTX_free(ctx);

	return 0;
}
