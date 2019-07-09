#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <tools/debug.h>
#include <openssl/evp.h>

static unsigned char key[] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

static unsigned char iv[] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

static unsigned char in[] = {
	0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
	0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

static size_t inl = 16;

static unsigned char enc[1024];
static int enc_len;

static void Encrypt_SM4(void)
{
	int rc;
	int outl_tmp;
	EVP_CIPHER_CTX *ctx;

	ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		printf("Encrypt failed :ctx new \n");
		return ;
	}

	rc = EVP_CIPHER_CTX_set_padding(ctx, 1);
	if (rc != 1) {
		printf("Encrypt failed : ctx set padding\n");
		goto out;
	}

	rc = EVP_EncryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
	if (rc != 1) {
		printf("Encrypt failed : EncryptInit\n");
		goto out;
	}

	rc = EVP_EncryptUpdate(ctx, enc, &enc_len, in, inl);
	if (rc != 1) {
		printf("Encrypt failed : EncryptUpdate\n");
		goto out;
	}

	rc = EVP_EncryptFinal_ex(ctx, enc + enc_len, &outl_tmp);
	if (rc != 1) {
		printf("Encrypt failed : EncryptUpdate\n");
		goto out;
	}

	enc_len += outl_tmp;
	print_hex_dump_debug("encrypt:", enc, enc_len);
out:
	EVP_CIPHER_CTX_free(ctx);
	return ;
}

static void Decrypt_SM4(void)
{
	int rc;
	unsigned char out[1024];
	int outl;
	int outl_tmp;
	EVP_CIPHER_CTX *ctx;

	ctx = EVP_CIPHER_CTX_new();
	if (!ctx) {
		printf("Decrypt failed :ctx new \n");
		return ;
	}

	rc = EVP_CIPHER_CTX_set_padding(ctx, 1);
	if (rc != 1) {
		printf("Decrypt failed : ctx set padding\n");
		goto out;
	}
	
	rc = EVP_DecryptInit_ex(ctx, EVP_sm4_cbc(), NULL, key, iv);
	if (rc != 1) {
		printf("Decrypt failed : Decrypt Init\n");
		goto out;
	}

	rc = EVP_DecryptUpdate(ctx, out, &outl, enc, enc_len);
	if (rc != 1) {
		printf("Decrypt failed : Decrypt Update\n");
		goto out;
	}

	rc = EVP_DecryptFinal_ex(ctx, out + outl, &outl_tmp);
	if (rc != 1) {
		printf("Decrypt failed : Decrypt Final\n");
		goto out;
	}

	outl +=outl_tmp;
	print_hex_dump_debug("Decrypt:", out, outl);

out:
	EVP_CIPHER_CTX_free(ctx);
	return ;

}

int main(int argc, const char *argv[])
{
	OpenSSL_add_all_ciphers();
	Encrypt_SM4();
	Decrypt_SM4();
	return 0;
}
