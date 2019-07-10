#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <base.h>

struct args args;

static char key[] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};
static char iv[] = {"\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F"};

static void parse_args(int argc, char *argv[]);
void cipher(int argc, char *argv[])
{
	parse_args(argc, argv);	
	args.crypto.cipher(&args);
}

static void cipher_encrypt_file_out(void *args)
{
	int relen;
	BIO *b_cipher, *b_in, *b_out, *b_base64;
	char buf[1024];
	EVP_CIPHER *cipher;

	struct args *a = args;

	BIO_E_NULL(b_cipher, BIO_new(BIO_f_cipher()));
	if (a->algo & CIPHER_ECB)
		cipher = (EVP_CIPHER *)EVP_sm4_ecb();
	else
		cipher = (EVP_CIPHER *)EVP_sm4_cbc();

	BIO_set_cipher(b_cipher, cipher, a->key, a->iv, 1);
	BIO_pending(b_cipher);
	BIO_E_NULL(b_in, BIO_new_file(a->in_file, "r"));
	BIO_push(b_cipher, b_in);

	BIO_E_NULL(b_base64, BIO_new(BIO_f_base64()));
	BIO_E_NULL(b_out, BIO_new_file(a->out_file, "w"));
	BIO_push(b_base64, b_out);
	
	do {
		relen = BIO_read(b_cipher, buf, sizeof(buf));
		BIO_write(b_base64, buf, relen);
	} while (relen == 1024);

	BIO_flush(b_base64);

	BIO_free_all(b_cipher);
	BIO_free_all(b_base64);
}

static void cipher_encrypt_file(void *args)
{
	int relen;
	BIO *b_cipher, *b_in, *b_out, *b_base64;
	char buf[1024];
	EVP_CIPHER *cipher;

	struct args *a = args;

	BIO_E_NULL(b_cipher, BIO_new(BIO_f_cipher()));
	if (a->algo & CIPHER_ECB)
		cipher = (EVP_CIPHER *)EVP_sm4_ecb();
	else
		cipher = (EVP_CIPHER *)EVP_sm4_cbc();

	BIO_set_cipher(b_cipher, cipher, a->key, a->iv, 1);
	BIO_pending(b_cipher);
	BIO_E_NULL(b_in, BIO_new_file(a->in_file, "r"));
	BIO_push(b_cipher, b_in);

	
	do {
		relen = BIO_read(b_cipher, buf, sizeof(buf));
		print_hex_dump_debug("cipher:", buf, relen);
	} while (relen == 1024);

	BIO_free_all(b_cipher);
}

static const struct option long_options[] = {
	{"version", 0, NULL, 'v'},
	{"help", 0, NULL, 'h'},
	{"enc", 0, NULL, 'e'},
	{"dec", 0, NULL, 'd'},
	{"ecb", 0, NULL, '1'},
	{"cbc", 0, NULL, '2'},

	{"in", 1, NULL, 'i'},
	{"out", 1, NULL, 'o'},
	{"algo", 1, NULL, 'g'},
	{NULL, 0, 0, 0},
};

static void parse_args(int argc, char *argv[])
{
	int c;

	do {
		c = getopt_long(argc, argv, "vhed12i:o:g:", long_options, NULL);
		if (c == -1)
			break;

		switch (c) {
		case 'v':
			printf("version: %s\n", VERSION);
			exit(0);
		case 'h':
			printf(help);
			exit(0);
			break;
		case 'i':
			args.in_file = optarg;
			break;
		case 'o':
			args.out_file = optarg;
			break;
		case 'g':
			if (strstr(optarg, "sm4"))
				args.algo = CIPHER_SM4;
			break;
		case '1':
			args.algo |= CIPHER_ECB;
			break;
		case '2':
			args.algo |= CIPHER_CBC;
			break;
		case 'e':
			args.algo |= CIPHER_ENC;
			break;
		case 'd':
			args.algo |= CIPHER_DEC;
			break;

		default:
			printf(help);
			exit(1);
			break;
		}
	} while (1);


	if (args.algo & CIPHER_ENC) {
		if (args.in_file && args.out_file)
			args.crypto.cipher = cipher_encrypt_file_out;
		else if (args.in_file)
			args.crypto.cipher = cipher_encrypt_file;
	}

	args.key = key;
	if (args.algo & CIPHER_CBC)
		args.iv = iv;
}

