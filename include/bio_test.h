#ifndef __BIO_TEST_10242_HH__
#define __BIO_TEST_10242_HH__

#define VERSION "v1.0"

enum {
	EVP_MD5,
	EVP_SHA1,
	EVP_SHA256,
	EVP_SM3,
};

#define CIPHER_SM4 0x000100000

#define CIPHER_ENC (1 << 0)
#define CIPHER_DEC (1 << 1)
#define CIPHER_ECB (1 << 2)
#define CIPHER_CBC (1 << 3)

struct crypte_operations {
	void (*digest)(void *);
	void (*cipher)(void *);
};

struct args {
	struct crypte_operations crypto;
	uint32_t algo;
	uint8_t *in_file;
	uint8_t *out_file;
	uint8_t *key;
	uint8_t *iv;
};

static const char *help =	
"help:\n"
"\t -v,	--version	print version info\n"
"\t -h,	--help		print help message\n"
"\t -i,	--in		input file name and path\n"
"\t -o,	--out		output file name and path\n"
"\t -g,	--algo		if digest md5/sha1/sha256/sm3\n"
"\t			if cipher sm4\n"
"example:\n"
"\t digest:\n"
"\t\t ./bio digest -gmd5 -ibio \n"
"\t\t ./bio digest -gsm3 -ibio \n"
"\t\t echo 1234 | ./bio digest -gmd5 \n"
"\t\t echo 1234 | ./bio digest -gsm3 \n"
"\t cipher:\n";

#endif
