#ifndef __BIO_TEST_10242_HH__
#define __BIO_TEST_10242_HH__

#define VERSION "v1.0"

enum EVP_MD {
	EVP_MD5,
	EVP_SHA1,
	EVP_SHA256,
	EVP_SM3,
};

struct crypte_operations {
	void (*digest)(void *);
	void (*cipher)(void *);
};

struct args {
	struct crypte_operations crypt;
	void (*parse_args)(int , char **);
	uint32_t type;
	uint8_t *in_file;
	uint8_t *out_file;
};

static const char *help =	
"help:\n"
"\t -v, --version	print version info\n"
"\t -h, --help		print help message\n"
"\t -i,	--in		input file name and path\n"
"\t -o, --out		output file name and path\n"
"\t -t, --type		if digest md5/sha1/sha256/sm3\n"
"			if cipher des3/sm4\n"
"example:\n"
"\t digest:\n"
"\t\t ./bio digest -tmd5 -ibio \n"
"\t\t ./bio digest -tsm3 -ibio \n"
"\t\t echo 1234 | ./bio digest -tmd5 \n"
"\t\t echo 1234 | ./bio digest -tsm3 \n"
"\t cipher:\n";

#endif
