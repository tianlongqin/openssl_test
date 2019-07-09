#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define CERT_PATH "../cert/CA/newcerts/test01.cer"

struct {
	int version;
	struct tm after_time;
	struct tm before_time;
	char serial[32];
} cert;

#define C(x) (cert.x)

static inline int get_version(X509 *x509)
{
	return C(version) = X509_get_version(x509) + 1;
}

static inline char *get_after_time(X509 *x509)
{
	ASN1_TIME_to_tm(X509_get_notBefore(x509), &C(after_time));

	return asctime(&C(after_time));
}

static inline char *get_before_time(X509 *x509)
{
	ASN1_TIME_to_tm(X509_get_notAfter(x509), &C(before_time));

	return asctime(&C(before_time));
}

static inline char *get_serial_number(X509 *x509)
{
	int i;
	ASN1_INTEGER *serial;
	serial = X509_get_serialNumber(x509);
	for (i = 0; i < ASN1_INTEGER_get(serial); i++)
		sprintf(C(serial + i *2), "%02d", serial->data[i]);

	return C(serial);
}

static void print_cert_info(X509 *x509)
{
	printf("version		:%d\n", get_version(x509));
	printf("after time	:%s", get_after_time(x509));
	printf("before time	:%s", get_before_time(x509));
	printf("serial number	:%s\n", get_serial_number(x509));
}

int main(int argc, const char *argv[])
{
	BIO *bio;
	BIO *bio_fp;
	X509 *x509 = NULL;

	ERR_load_BIO_strings();
	bio = BIO_new_file(CERT_PATH, "r");
	bio_fp = BIO_new(BIO_s_file());
	BIO_set_fp(bio_fp, stdout, BIO_NOCLOSE);

	d2i_X509_bio(bio, &x509);
	
	X509_print(bio_fp, x509);

	BIO_free(bio);
	BIO_free(bio_fp);

	return 0;
}
#if 0
int main(int argc, const char *argv[])
{
	FILE *fp;
	X509 *x509 = NULL;
	struct tm tm;
	fp = fopen(CERT_PATH, "r");
	if (!fp) {
		printf("fopen failed :%s\n", strerror(errno));
		return 1;
	}

	if (d2i_X509_fp(fp, &x509) == NULL) {
		printf("d2i_X509_fp failed \n");
		goto out;
	}

	X509_print_fp(stdout, x509);
	//print_cert_info(x509);

out:
	fclose(fp);
	return 0;
}
#endif
