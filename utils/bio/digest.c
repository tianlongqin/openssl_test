#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <base.h>

static void parse_args(int argc, char *argv[]);
static void digest_in_file(void *args);
static void digest_stdin(void *args);

static struct args args;
void digest(int argc, char *argv[])
{
	parse_args(argc, argv);
	if(args.crypt.digest)
		args.crypt.digest(&args);
	else
		printf("not find diest functions");
}

static void digest_in_file(void *args)
{
	BIO *b_md;
	BIO *b_file;
	char buf[1024];
	int relen;
	struct args *a = args;

	BIO_E_NULL(b_file, BIO_new_file(a->in_file, "r"));
	BIO_E_NULL(b_md, BIO_new(BIO_f_md()));

	switch (a->type) {
	case EVP_MD5:
		BIO_set_md(b_md, EVP_md5());
		break;
	case EVP_SHA1:
		BIO_set_md(b_md, EVP_sha1());
		break;
	case EVP_SHA256:
		BIO_set_md(b_md, EVP_sha256());
		break;
	case EVP_SM3:
		BIO_set_md(b_md, EVP_sm3());
		break;
	}

	b_md = BIO_push(b_md, b_file);

	do {
		relen = BIO_read(b_md, buf, sizeof(buf));
	} while(relen == 1024);

	relen = BIO_gets(b_md, buf, sizeof(buf));
	print_hex_dump_debug("digest:", buf, relen);

}

static void digest_stdin(void *args)
{
	BIO *b_in, *b_md;
	int relen;
	char buf[1024];
	struct args *a = args;

	if (!file_b_fifo(0, F_FIFO)) {
		printf("example: echo test | ./bio -dmd5 \n");
		return;
	}

	BIO_E_NULL(b_in, BIO_new(BIO_s_fd()));
	BIO_set_fd(b_in, 0, BIO_NOCLOSE);
	BIO_E_NULL(b_md, BIO_new(BIO_f_md()));

	switch (a->type) {
	case EVP_MD5:
		BIO_set_md(b_md, EVP_md5());
		break;
	case EVP_SHA1:
		BIO_set_md(b_md, EVP_sha1());
		break;
	case EVP_SHA256:
		BIO_set_md(b_md, EVP_sha256());
		break;
	case EVP_SM3:
		BIO_set_md(b_md, EVP_sm3());
		break;
	}

	BIO_push(b_md, b_in);

	do {
		relen = BIO_read(b_md, buf, sizeof(buf));
	} while(relen);

	relen = BIO_gets(b_md, buf, sizeof(buf));

	print_hex_dump_debug("digest:", buf, relen);
}

static const struct option long_options[] = {
	{"version", 0, NULL, 'v'},
	{"help", 0, NULL, 'h'},

	{"in", 1, NULL, 'i'},
	{"out", 1, NULL, 'o'},
	{"type", 1, NULL, 't'},
	{NULL, 0, 0, 0},
};

static void parse_args(int argc, char *argv[])
{
	int c;

	do {
		c = getopt_long(argc, argv, "vhi:o:t:", long_options, NULL);
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

		case 't':
			if (strstr(optarg, "md5"))
				args.type = EVP_MD5;
			else if (strstr(optarg, "sha1"))
				args.type = EVP_SHA1;
			else if (strstr(optarg, "sha256"))
				args.type = EVP_SHA256;
			else if (strstr(optarg, "sm3"))
				args.type = EVP_SM3;
			break;
		default:
			printf(help);
			exit(1);
			break;
		}
	} while (1);

	if (args.in_file)
		args.crypt.digest = digest_in_file;
	else
		args.crypt.digest = digest_stdin;
}

