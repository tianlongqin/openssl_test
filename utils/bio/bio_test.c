#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <base.h>
#include <digest.h>
#include <cipher.h>
static const struct option long_options[] = {
	{"version", 0, NULL, 'v'},
	{"help", 0, NULL, 'h'},

	{NULL, 0, 0, 0},
};

static void parse_args(int argc, char *argv[])
{
	int c;

	do {
		c = getopt_long(argc, argv, "vh", long_options, NULL);
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
		default:
			printf(help);
			exit(1);
			break;
		}
	} while (1);
}

int main(int argc, char *argv[])
{
	if (!strcmp(argv[1], "digest")) {
		digest(argc, argv);
	} else if (!strcmp(argv[1], "cipher")){
		cipher(argc, argv);
	} else {
		parse_args(argc, argv);
	}

	return 0;
}
