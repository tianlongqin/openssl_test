#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <tools/debug.h>
#include <tools/file.h>
#include <bio_test.h>

#define VERSION "v1.0"

struct crypte_operations {
	void (*digest)(void);
};

static struct {
#define A(a) (args.a)
	struct crypte_operations crypt;
	uint32_t md_com;
	uint8_t *in_file;
	uint8_t *out_file;
} args;

const char *help =	
"help:\n"
"\t -v, --version	print version info\n"
"\t -h, --help		print help message\n"
"\t -i,	--in		input file name and path\n"
"\t -o, --out		output file name and path\n"
"\t -d, --digest	message digest commands. for example md5/sha1/sha256/sm3\n"
"example:\n"
"\t digest:\n"
"\t\t ./bio -dmd5 -ibio \n"
"\t\t ./bio -dsm3 -ibio \n"
"\t\t echo 1234 | ./bio -dmd5 \n"
"\t\t echo 1234 | ./bio -dsm3 \n";
			
static const struct option long_options[] = {
	{"version", 0, NULL, 'v'},
	{"help", 0, NULL, 'h'},


	{"in", 1, NULL, 'i'},
	{"out", 1, NULL, 'o'},
	{"digest", 1, NULL, 'd'},
	{"enc", 1, NULL, '1'},
	{NULL, 0, 0, 0},
};

static void digest_in_file(void);
static void digest_stdin();
static void parse_args(int argc, char *argv[])
{
	int c;

	do {
		c = getopt_long(argc, argv, "hvi:o:d:", long_options, NULL);
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
			case 'd':
				if (strstr(optarg, "md5"))
					A(md_com) = EVP_MD5;
				else if (strstr(optarg, "sha1"))
					A(md_com) = EVP_SHA1;
				else if (strstr(optarg, "sha256"))
					A(md_com) = EVP_SHA256;
				else if (strstr(optarg, "sm3"))
					A(md_com) = EVP_SM3;
				A(crypt.digest) = digest_stdin;
				break;
			case 'i':
				A(in_file) = optarg;
				A(crypt.digest) = digest_in_file;
				break;
			case 'o':
				A(out_file) = optarg;
				break;
			default:
				printf(help);
				exit(1);
				break;
		}
	} while (1);
}

static void digest_in_file(void)
{
	BIO *b_md;
	BIO *b_file;
	char buf[1024];
	int relen;

	BIO_E_NULL(b_file, BIO_new_file(A(in_file), "r"));
	BIO_E_NULL(b_md, BIO_new(BIO_f_md()));

	switch (A(md_com)) {
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

static void digest_stdin(void)
{
	BIO *b_in, *b_md;
	int relen;
	char buf[1024];

	if (!file_b_fifo(0, F_FIFO)) {
		printf("example: echo test | ./bio -dmd5 \n");
		return;
	}

	BIO_E_NULL(b_in, BIO_new(BIO_s_fd()));
	BIO_set_fd(b_in, 0, BIO_NOCLOSE);
	BIO_E_NULL(b_md, BIO_new(BIO_f_md()));

	switch (A(md_com)) {
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

int main(int argc, char *argv[])
{
	parse_args(argc, argv);

	if (A(crypt.digest))
		A(crypt.digest)();

	return 0;
}


