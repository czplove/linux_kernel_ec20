/********************************************************************
    QCEDEV driver test app.
*********************************************************************/

#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <memory.h>
#include "qcedev.h"


#define CRYPTO_HMAC_SUPPORT
#define CRYPTO_CMAC_SUPPORT
#define CRYPTO_AES_XTS_SUPPORT
#define CRYPTO_HMAC_SUPPORT
#define CRYPTO_AES_192BIT_NOT_SUPPORTED

#define NUM_ITERATIONS 10000
#define MAX_PACKET_DATA_KB 65
#define NUM_PTHREADS 8
#define _128K    0x20000
#define _32K    0x8000
#define _16K    0x4000

#define KEY_SIZE_16    16
#define KEY_SIZE_32    32
#define KEY_SIZE_64    64


/*********************************************************************
**  CIPHER TEST VECTORS.
*********************************************************************/

struct test_vector {
    /* only used with keyed hash algorithms */
    char *key;
    unsigned char klen;
    char *iv;
    unsigned char ivlen;
    char *input;
    unsigned char ilen;
    char *result;
    unsigned char rlen;
    char *digest;
    unsigned char diglen;
    enum qcedev_cipher_alg_enum c_alg;
    enum qcedev_cipher_mode_enum mode;
    enum qcedev_sha_alg_enum hash_alg;
};

/*********************************************************************
**  CIPHER TEST KEYS IV AND TEST VECTORS
*********************************************************************/
static unsigned char  PlainText_90[] = {
    0xBA, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
};

static unsigned char  PlainText_10[]  = {
    0xf3, 0x44, 0x81, 0xec, 0x3c, 0xc6, 0x27, 0xba,0xcd, 0x5d, 0xc3, 0xfb, 0x08, 0xf2, 0x73, 0xe6
};


static unsigned char  PlainText_60[] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0xBA, 0xC1, 0xBE, 0xE2, 0x2E, 0x40, 0x9F, 0x96,0xE9, 0x3D, 0x7E, 0x11, 0x73, 0x93, 0x17, 0x2A,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
};

static unsigned char key[64]  = {
    0x10, 0xa5, 0x88, 0x69, 0xd7, 0x4b, 0xe5, 0xa3,0x74, 0xcf, 0x86, 0x7c, 0xfb, 0x47, 0x38, 0x59,
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C,
    0x10, 0xa5, 0x88, 0x69, 0xd7, 0x4b, 0xe5, 0xa3,0x74, 0xcf, 0x86, 0x7c, 0xfb, 0x47, 0x38, 0x59,
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C,
};

static unsigned char iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,0xe8, 0xf9, 0xa1, 0xbc, 0x1c, 0xcd, 0x4e, 0xf4,
};

unsigned char vbuf_src[0x1000];
unsigned char vbuf_dst[0x1000];


/*********************************************************************
**  AES TEST VECTORS
*********************************************************************/

static struct test_vector cipher_aes_tv[] = {
{ /* TEST VECTOR 1  AES-128 ECB: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_ECB,
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .klen   = 16,
        .ivlen    = 0,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result = "\x3a\xd7\x7b\xb4\x0d\x7a\x36\x60"
              "\xa8\x9e\xca\xf3\x24\x66\xef\x97"
              "\xf5\xd3\xd5\x85\x03\xb9\x69\x9d"
              "\xe7\x85\x89\x5a\x96\xfd\xba\xaf"
              "\x43\xb1\xcd\x7f\x59\x8e\xce\x23"
              "\x88\x1b\x00\xe3\xed\x03\x06\x88"
              "\x7b\x0c\x78\x5e\x27\xe8\xad\x3f"
              "\x82\x23\x20\x71\x04\x72\x5d\xd4",
        .rlen   = 64,
#ifndef CRYPTO_AES_192BIT_NOT_SUPPORTED
    }, { /* TEST VECTOR 2 AES-192 ECB: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_ECB,
        .key    = "\x8e\x73\xb0\xf7\xda\x0e\x64\x52"
              "\xc8\x10\xf3\x2b\x80\x90\x79\xe5"
              "\x62\xf8\xea\xd2\x52\x2c\x6b\x7b",
        .klen    = 24,
        .ivlen    = 0,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\xbd\x33\x4f\x1d\x6e\x45\xf2\x5f"
              "\xf7\x12\xa2\x14\x57\x1f\xa5\xcc"
              "\x97\x41\x04\x84\x6d\x0a\xd3\xad"
              "\x77\x34\xec\xb3\xec\xee\x4e\xef"
              "\xef\x7a\xfd\x22\x70\xe2\xe6\x0a"
              "\xdc\xe0\xba\x2f\xac\xe6\x44\x4e"
              "\x9a\x4b\x41\xba\x73\x8d\x6c\x72"
              "\xfb\x16\x69\x16\x03\xc1\x8e\x0e",
        .rlen    = 64,
#endif
    }, { /* TEST VECTOR 3 AES-256 ECB: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_ECB,
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .klen    = 32,
        .ivlen    = 0,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\xf3\xee\xd1\xbd\xb5\xd2\xa0\x3c"
              "\x06\x4b\x5a\x7e\x3d\xb1\x81\xf8"
              "\x59\x1c\xcb\x10\xd4\x10\xed\x26"
              "\xdc\x5b\xa7\x4a\x31\x36\x28\x70"
              "\xb6\xed\x21\xb9\x9c\xa6\xf4\xf9"
              "\xf1\x53\xe7\xb1\xbe\xaf\xed\x1d"
              "\x23\x30\x4b\x7a\x39\xf9\xf3\xff"
              "\x06\x7d\x8d\x8f\x9e\x24\xec\xc7",
        .rlen    = 64,
    }, { /* TEST VECTOR 4 AES-128 CBC: RFC 3602 */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\x06\xa9\x21\x40\x36\xb8\xa1\x5b"
              "\x51\x2e\x03\xd5\x34\x12\x00\x06",
        .klen   = 16,
        .iv    = "\x3d\xaf\xba\x42\x9d\x9e\xb4\x30"
              "\xb4\x22\xda\x80\x2c\x9f\xac\x41",
        .ivlen    = 16,
        .input    = "Single block msg",
        .ilen   = 16,
        .result = "\xe3\x53\x77\x9c\x10\x79\xae\xb8"
              "\x27\x08\x94\x2d\xbe\x77\x18\x1a",
        .rlen   = 16,
    }, { /* TEST VECTOR 5 AES-128 CBC: RFC 3602*/
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\xc2\x86\x69\x6d\x88\x7c\x9a\xa0"
              "\x61\x1b\xbb\x3e\x20\x25\xa4\x5a",
        .klen   = 16,
        .iv     = "\x56\x2e\x17\x99\x6d\x09\x3d\x28"
              "\xdd\xb3\xba\x69\x5a\x2e\x6f\x58",
        .ivlen    = 16,
        .input  = "\x00\x01\x02\x03\x04\x05\x06\x07"
              "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f"
              "\x10\x11\x12\x13\x14\x15\x16\x17"
              "\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f",
        .ilen   = 32,
        .result = "\xd2\x96\xcd\x94\xc2\xcc\xcf\x8a"
              "\x3a\x86\x30\x28\xb5\xe1\xdc\x0a"
              "\x75\x86\x60\x2d\x25\x3c\xff\xf9"
              "\x1b\x82\x66\xbe\xa6\xd6\x1a\xb1",
        .rlen   = 32,
    }, { /* TEST VECTOR 6 AES-128 CBC: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .klen   = 16,
        .iv     = "\x00\x01\x02\x03\x04\x05\x06\x07"
              "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
        .ivlen    = 16,
        .input  = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a",
        .ilen   = 16,
        .result = "\x76\x49\xab\xac\x81\x19\xb2\x46"
              "\xce\xe9\x8e\x9b\x12\xe9\x19\x7d",
        .rlen   = 16,
    }, { /* TEST VECTOR 7 AES-128 CBC: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .klen   = 16,
        .iv     = "\x00\x01\x02\x03\x04\x05\x06\x07"
              "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
        .ivlen    = 16,
        .input  = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51",
        .ilen   = 32,
        .result = "\x76\x49\xab\xac\x81\x19\xb2\x46"
              "\xce\xe9\x8e\x9b\x12\xe9\x19\x7d"
              "\x50\x86\xcb\x9b\x50\x72\x19\xee"
              "\x95\xdb\x11\x3a\x91\x76\x78\xb2",
        .rlen   = 32,
#ifndef CRYPTO_AES_192BIT_NOT_SUPPORTED
    }, { /* TEST VECTOR 8 AES-192 CBC: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\x8e\x73\xb0\xf7\xda\x0e\x64\x52"
              "\xc8\x10\xf3\x2b\x80\x90\x79\xe5"
              "\x62\xf8\xea\xd2\x52\x2c\x6b\x7b",
        .klen    = 24,
        .iv    = "\x00\x01\x02\x03\x04\x05\x06\x07"
              "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
        .ivlen    = 16,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\x4f\x02\x1d\xb2\x43\xbc\x63\x3d"
              "\x71\x78\x18\x3a\x9f\xa0\x71\xe8"
              "\xb4\xd9\xad\xa9\xad\x7d\xed\xf4"
              "\xe5\xe7\x38\x76\x3f\x69\x14\x5a"
              "\x57\x1b\x24\x20\x12\xfb\x7a\xe0"
              "\x7f\xa9\xba\xac\x3d\xf1\x02\xe0"
              "\x08\xb0\xe2\x79\x88\x59\x88\x81"
              "\xd9\x20\xa9\xe6\x4f\x56\x15\xcd",
        .rlen    = 64,
#endif
    }, { /* TEST VECTOR 9 AES-256 CBC: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CBC,
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .klen    = 32,
        .iv    = "\x00\x01\x02\x03\x04\x05\x06\x07"
              "\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f",
        .ivlen    = 16,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\xf5\x8c\x4c\x04\xd6\xe5\xf1\xba"
              "\x77\x9e\xab\xfb\x5f\x7b\xfb\xd6"
              "\x9c\xfc\x4e\x96\x7e\xdb\x80\x8d"
              "\x67\x9f\x77\x7b\xc6\x70\x2c\x7d"
              "\x39\xf2\x33\x69\xa9\xd9\xba\xcf"
              "\xa5\x30\xe2\x63\x04\x23\x14\x61"
              "\xb2\xeb\x05\xe2\xc3\x9b\xe9\xfc"
              "\xda\x6c\x19\x07\x8c\x6a\x9d\x1b",
        .rlen    = 64,
    }, { /* TEST VECTOR 10 AES-128 CTR: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CTR,
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .klen    = 16,
        .iv    = "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7"
              "\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
        .ivlen    = 16,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\x87\x4d\x61\x91\xb6\x20\xe3\x26"
              "\x1b\xef\x68\x64\x99\x0d\xb6\xce"
              "\x98\x06\xf6\x6b\x79\x70\xfd\xff"
              "\x86\x17\x18\x7b\xb9\xff\xfd\xff"
              "\x5a\xe4\xdf\x3e\xdb\xd5\xd3\x5e"
              "\x5b\x4f\x09\x02\x0d\xb0\x3e\xab"
              "\x1e\x03\x1d\xda\x2f\xbe\x03\xd1"
              "\x79\x21\x70\xa0\xf3\x00\x9c\xee",
        .rlen    = 64,
#ifndef CRYPTO_AES_192BIT_NOT_SUPPORTED
    }, { /* TEST VECTOR 11 AES-192 CTR: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CTR,
        .key    = "\x8e\x73\xb0\xf7\xda\x0e\x64\x52"
              "\xc8\x10\xf3\x2b\x80\x90\x79\xe5"
              "\x62\xf8\xea\xd2\x52\x2c\x6b\x7b",
        .klen    = 24,
        .iv    = "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7"
              "\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
        .ivlen    = 16,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\x1a\xbc\x93\x24\x17\x52\x1c\xa2"
              "\x4f\x2b\x04\x59\xfe\x7e\x6e\x0b"
              "\x09\x03\x39\xec\x0a\xa6\xfa\xef"
              "\xd5\xcc\xc2\xc6\xf4\xce\x8e\x94"
              "\x1e\x36\xb2\x6b\xd1\xeb\xc6\x70"
              "\xd1\xbd\x1d\x66\x56\x20\xab\xf7"
              "\x4f\x78\xa7\xf6\xd2\x98\x09\x58"
              "\x5a\x97\xda\xec\x58\xc6\xb0\x50",
        .rlen    = 64,
#endif
    }, { /* TEST VECTOR 12 AES-256 CTR: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_AES,
        .mode    = QCEDEV_AES_MODE_CTR,
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .klen    = 32,
        .iv    = "\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7"
              "\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff",
        .ivlen    = 16,
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .ilen    = 64,
        .result    = "\x60\x1e\xc3\x13\x77\x57\x89\xa5"
              "\xb7\xa7\xf5\x04\xbb\xf3\xd2\x28"
              "\xf4\x43\xe3\xca\x4d\x62\xb5\x9a"
              "\xca\x84\xe9\x90\xca\xca\xf5\xc5"
              "\x2b\x09\x30\xda\xa2\x3d\xe9\x4c"
              "\xe8\x70\x17\xba\x2d\x84\x98\x8d"
              "\xdf\xc9\xc5\x8d\xb6\x7a\xad\xa6"
              "\x13\xc2\xdd\x08\x45\x79\x41\xa6",
        .rlen    = 64,
    },
};

/*********************************************************************
**  DES TEST VECTORS
*********************************************************************/

static struct test_vector cipher_des_tv[] = {
{ /* TEST VECTOR 13 DES CBC: NIST SP800-38A */
        .c_alg    = QCEDEV_ALG_DES,
        .mode    = QCEDEV_DES_MODE_CBC,
        .key    = "\x01\x23\x45\x67\x89\xab\xcd\xef",
        .klen    = 8,
        .iv    = "\xfe\xdc\xba\x98\x76\x54\x32\x10",
        .ivlen    = 8,
        .input    = "\x37\x36\x35\x34\x33\x32\x31\x20"
              "\x4e\x6f\x77\x20\x69\x73\x20\x74"
              "\x68\x65\x20\x74\x69\x6d\x65\x20",
        .ilen    = 24,
        .result    = "\xcc\xd1\x73\xff\xab\x20\x39\xf4"
              "\xac\xd8\xae\xfd\xdf\xd8\xa1\xeb"
              "\x46\x8e\x91\x15\x78\x88\xba\x68",
        .rlen    = 24,
    }, { /* TEST VECTOR 14 FIPS Pub 81 */
        .c_alg    = QCEDEV_ALG_DES,
        .mode    = QCEDEV_DES_MODE_CBC,
        .key    = "\x01\x23\x45\x67\x89\xab\xcd\xef",
        .klen    = 8,
        .iv    = "\x12\x34\x56\x78\x90\xab\xcd\xef",
        .ivlen    = 8,
        .input    = "\x4e\x6f\x77\x20\x69\x73\x20\x74",
        .ilen    = 8,
        .result    = "\xe5\xc7\xcd\xde\x87\x2b\xf2\x7c",
        .rlen    = 8,
    }, { /* TEST VECTOR 15 FIPS Pub 81 */
        .c_alg    = QCEDEV_ALG_DES,
        .mode    = QCEDEV_DES_MODE_CBC,
        .key    = "\x01\x23\x45\x67\x89\xab\xcd\xef",
        .klen    = 8,
        .iv    = "\xe5\xc7\xcd\xde\x87\x2b\xf2\x7c",
        .ivlen    = 8,
        .input    = "\x68\x65\x20\x74\x69\x6d\x65\x20",
        .ilen    = 8,
        .result    = "\x43\xe9\x34\x00\x8c\x38\x9c\x0f",
        .rlen    = 8,
    }, { /* TEST VECTOR 16 FIPS Pub 81 */
        .c_alg    = QCEDEV_ALG_DES,
        .mode    = QCEDEV_DES_MODE_CBC,
        .key    = "\x01\x23\x45\x67\x89\xab\xcd\xef",
        .klen    = 8,
        .iv    = "\x43\xe9\x34\x00\x8c\x38\x9c\x0f",
        .ivlen    = 8,
        .input    = "\x66\x6f\x72\x20\x61\x6c\x6c\x20",
        .ilen    = 8,
        .result    = "\x68\x37\x88\x49\x9a\x7c\x05\xf6",
        .rlen    = 8,
    }, { /* TEST VECTOR 17 FIPS Pub 81 */
        .c_alg    = QCEDEV_ALG_DES,
        .mode    = QCEDEV_DES_MODE_CBC,
        .key    = "\x01\x23\x45\x67\x89\xab\xcd\xef",
        .klen    = 8,
        .iv    = "\xfe\xdc\xba\x98\x76\x54\x32\x10",
        .ivlen    = 8,
        .input    = "\x37\x36\x35\x34\x33\x32\x31\x20"
              "\x4e\x6f\x77\x20\x69\x73\x20\x74"
              "\x68\x65\x20\x74\x69\x6d\x65\x20",
        .ilen    = 24,
        .result    = "\xcc\xd1\x73\xff\xab\x20\x39\xf4"
              "\xac\xd8\xae\xfd\xdf\xd8\xa1\xeb"
              "\x46\x8e\x91\x15\x78\x88\xba\x68",
        .rlen    = 24,
    },
};


/*********************************************************************
**  CMAC TEST VECTORS
*********************************************************************/

static struct test_vector aes_cmac_tv[] = {
    { /* TEST VECTOR 1 */
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a",
        .digest = "\x07\x0a\x16\xb4\x6b\x4d\x41\x44"
              "\xf7\x9b\xdd\x9d\xd0\x4a\x28\x7c",
        .diglen    = 16,
        .ilen    = 16,
        .klen    = 16,
    }, { /* TEST VECTOR 2 */
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .input =  "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11",
        .digest = "\xdf\xa6\x67\x47\xde\x9a\xe6\x30"
              "\x30\xca\x32\x61\x14\x97\xc8\x27",
        .diglen    = 16,
        .ilen    = 40,
        .klen    = 16,
    }, { /* TEST VECTOR 3 */
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .input = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .digest = "\x51\xf0\xbe\xbf\x7e\x3b\x9d\x92"
              "\xfc\x49\x74\x17\x79\x36\x3c\xfe",
        .diglen    = 16,
        .ilen    = 64,
        .klen    = 16,
    }, { /* TEST VECTOR 4 */
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a",
        .digest    = "\x28\xa7\x02\x3f\x45\x2e\x8f\x82"
              "\xbd\x4b\xf2\x8d\x8c\x37\xc3\x5c",
        .diglen    = 16,
        .ilen    = 16,
        .klen    = 32,
    }, { /* TEST VECTOR 5 */
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11",
        .digest    = "\xaa\xf3\xd8\xf1\xde\x56\x40\xc2"
              "\x32\xf5\xb1\x69\xb9\xc9\x11\xe6",
        .diglen    = 16,
        .ilen    = 40,
        .klen    = 32,
    }, { /* TEST VECTOR 6 */
        .key    = "\x60\x3d\xeb\x10\x15\xca\x71\xbe"
              "\x2b\x73\xae\xf0\x85\x7d\x77\x81"
              "\x1f\x35\x2c\x07\x3b\x61\x08\xd7"
              "\x2d\x98\x10\xa3\x09\x14\xdf\xf4",
        .input    = "\x6b\xc1\xbe\xe2\x2e\x40\x9f\x96"
              "\xe9\x3d\x7e\x11\x73\x93\x17\x2a"
              "\xae\x2d\x8a\x57\x1e\x03\xac\x9c"
              "\x9e\xb7\x6f\xac\x45\xaf\x8e\x51"
              "\x30\xc8\x1c\x46\xa3\x5c\xe4\x11"
              "\xe5\xfb\xc1\x19\x1a\x0a\x52\xef"
              "\xf6\x9f\x24\x45\xdf\x4f\x9b\x17"
              "\xad\x2b\x41\x7b\xe6\x6c\x37\x10",
        .digest    = "\xe1\x99\x21\x90\x54\x9f\x6e\xd5"
              "\x69\x6a\x2c\x05\x6c\x31\x54\x10",
        .diglen    = 16,
        .ilen    = 64,
        .klen    = 32,
    }, { /* TEST VECTOR 7 */
        .key    = "\x2b\x7e\x15\x16\x28\xae\xd2\xa6"
              "\xab\xf7\x15\x88\x09\xcf\x4f\x3c",
        .input    = "",
        .digest    = "\xbb\x1d\x69\x29\xe9\x59\x37\x28"
              "\x7f\xa3\x7d\x12\x9b\x75\x67\x46",
        .diglen    = 16,
        .ilen    = 0,
        .klen    = 16,
    },
};

/*********************************************************************
**  HMAC SHA1 TEST VECTORS
*********************************************************************/

static struct test_vector hmac_sha1_tv[] = {
/* http://csrc.nist.gov/publications/fips/fips198/fips-198a.pdf */
    {
        .key    = "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b"
                  "\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b\x0b",
        .klen  = 20,
        .input = "Hi There",
        .ilen  = 8,
        .digest = "\xb6\x17\x31\x86\x55\x05\x72\x64"
              "\xe2\x8b\xc0\xb6\xfb\x37\x8c\x8e\xf1"
              "\x46\xbe",
        .diglen = 20,
    }, {
        .key    = "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa"
              "\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa\xaa",
        .klen  = 20,
        .input = "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
             "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
             "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd"
             "\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd\xdd",
        .ilen  = 50,
        .digest = "\x12\x5d\x73\x42\xb9\xac\x11\xcd\x91\xa3"
              "\x9a\xf4\x8a\xa1\x7b\x4f\x63\xf1\x75\xd3",
        .diglen = 20,
    }, {
        .key    = "\x01\x02\x03\x04\x05\x06\x07\x08"
              "\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10"
              "\x11\x12\x13\x14\x15\x16\x17\x18\x19",
        .klen  = 25,
        .input = "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
             "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
             "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd"
             "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd",
        .ilen  = 50,
        .digest = "\x4c\x90\x07\xf4\x02\x62\x50\xc6\xbc\x84"
             "\x14\xf9\xbf\x50\xc8\x6c\x2d\x72\x35\xda",
        .diglen = 20,
    }, {
        .key = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c"
            "\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19"
            "\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26"
            "\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33"
            "\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f",
        .klen  = 64,
        .input = "Sample #1",
        .ilen  = 9,
        .digest = "\x4f\x4c\xa3\xd5\xd6\x8b\xa7\xcc\x0a\x12\x08\xc9\xc6"
              "\x1e\x9c\x5d\xa0\x40\x3c\x0a",
        .diglen = 20,
    }, {
        .key = "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c"
            "\x5d\x5e\x5f\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69"
            "\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76"
            "\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f\x80\x81\x82\x83"
            "\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90"
            "\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d"
            "\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa"
            "\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3",
        .klen  = 100,
        .input = "Sample #3",
        .ilen  = 9,
        .digest = "\xbc\xf4\x1e\xab\x8b\xb2\xd8\x02\xf3\xd0\x5c\xaf\x7c"
              "\xb0\x92\xec\xf8\xd1\xa3\xaa",
        .diglen = 20,
    }, {
        .key   = "\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c"
             "\x7d\x7e\x7f\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89"
             "\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96"
             "\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0",
        .klen  = 49,
        .input = "Sample #4",
        .ilen  = 9,
        .digest = "\x9e\xa8\x86\xef\xe2\x68\xdb\xec\xce\x42\x0c\x75",
        .diglen = 12,
    }, {
        .key   = "key",
        .klen  = 3,
        .input = "",
        .ilen  = 0,
        .digest = "\xf4\x2b\xb0\xee\xb0\x18\xeb\xbd\x45\x97"
              "\xae\x72\x13\x71\x1e\xc6\x07\x60\x84\x3f",
        .diglen = 20,
    }
};

/*********************************************************************
**  HMAC SHA256 TEST VECTORS
** http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/HMAC_SHA256.pdf
*********************************************************************/
static struct test_vector hmac_sha256_tv[] = {
    { /* TEST VECTOR 1 */
        .key    = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
              "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13"
              "\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D"
              "\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27"
              "\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31"
              "\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B"
              "\x3C\x3D\x3E\x3F",
        .input    = "Sample message for keylen=blocklen",
        .digest = "\x8B\xB9\xA1\xDB\x98\x06\xF2\x0D\xF7\xF7"
              "\x7B\x82\x13\x8C\x79\x14\xD1\x74\xD5\x9E"
              "\x13\xDC\x4D\x01\x69\xC9\x05\x7B\x13\x3E"
              "\x1D\x62",
        .diglen    = 32,
        .ilen    = 34,
        .klen    = 64,
    },  { /*{  TEST VECTOR 2 */
        .key    = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
              "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13"
              "\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D"
              "\x1E\x1F",
        .input    = "Sample message for keylen<blocklen",
        .digest = "\xA2\x8C\xF4\x31\x30\xEE\x69\x6A\x98\xF1"
              "\x4A\x37\x67\x8B\x56\xBC\xFC\xBD\xD9\xE5"
              "\xCF\x69\x71\x7F\xEC\xF5\x48\x0F\x0E\xBD"
              "\xF7\x90",
        .diglen    = 32,
        .ilen    = 34,
        .klen    = 32,
    },  { /* TEST VECTOR 3 */
        .key    = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
              "\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12\x13"
              "\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D"
              "\x1E\x1F\x20\x21\x22\x23\x24\x25\x26\x27"
              "\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x30\x31"
              "\x32\x33\x34\x35\x36\x37\x38\x39\x3A\x3B"
              "\x3C\x3D\x3E\x3F\x40\x41\x42\x43\x44\x45"
              "\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x4E\x4F"
              "\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59"
              "\x5A\x5B\x5C\x5D\x5E\x5F\x60\x61\x62\x63",
        .input    = "Sample message for keylen=blocklen",
        .digest = "\xBD\xCC\xB6\xC7\x2D\xDE\xAD\xB5\x00\xAE"
              "\x76\x83\x86\xCB\x38\xCC\x41\xC6\x3D\xBB"
              "\x08\x78\xDD\xB9\xC7\xA3\x8A\x43\x1B\x78"
              "\x37\x8D",
        .diglen    = 32,
        .ilen    = 34,
        .klen    = 100,
    },  { /* TEST VECTOR 4 */
        .key    = "\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09"
              "\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13"
              "\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d"
              "\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27"
              "\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30",
        .input    = "Sample message for keylen<blocklen,"
              " with truncated tag",
        .digest = "\x27\xA8\xB1\x57\x83\x9E\xFE\xAC\x98\xDF"
              "\x07\x0B\x33\x1D\x59\x36",
        .diglen    = 16,
        .ilen    = 54,
        .klen    = 49,
    },  { /* TEST VECTOR 5 */
        .key    = "key",
        .input    = "",
        .digest = "\x5d\x5d\x13\x95\x63\xc9\x5b\x59\x67\xb9"
              "\xbd\x9a\x8c\x9b\x23\x3a\x9d\xed\xb4\x50"
              "\x72\x79\x4c\xd2\x32\xdc\x1b\x74\x83\x26"
              "\x07\xd0",
        .diglen    = 32,
        .ilen    = 0,
        .klen    = 3,
    },
};

/*********************************************************************
**  SHA1 TEST VECTORS
*********************************************************************/
static struct test_vector sha1_tv[] = {
    { /* TEST VECTOR 1 */
        .input    = "abc",
        .ilen    = 3,
        .digest    = "\xa9\x99\x3e\x36\x47\x06\x81\x6a"
              "\xba\x3e\x25\x71\x78\x50\xc2\x6c"
              "\x9c\xd0\xd8\x9d",
        .diglen    = 20,
    }, { /* TEST VECTOR 2 */
        .input    = "abcdbcdecdefdefgefghfghighijhijkij"
              "kljklmklmnlmnomnopnopq",
        .ilen    = 56,
        .digest    = "\x84\x98\x3e\x44\x1c\x3b\xd2\x6e"
              "\xba\xae\x4a\xa1\xf9\x51\x29\xe5"
              "\xe5\x46\x70\xf1",
        .diglen    = 20,
    }, { /* TEST VECTOR 3 */
        .input    = "Test vector from febooti.com",
        .digest = "\xa7\x63\x17\x95\xf6\xd5\x9c\xd6"
              "\xd1\x4e\xbd\x00\x58\xa6\x39\x4a"
              "\x4b\x93\xd8\x68",
        .ilen    = 28,
        .diglen    = 20,
    }, { /* TEST VECTOR 4 */
        .input    = "This is an example input buffer",
        .digest = "\x18\x21\xf2\xa5\x7c\x5a\x26\x8a"
              "\x9f\xf2\x7c\x03\x8c\x6e\xb3\xf7"
              "\x37\x55\x1b\x98",
        .ilen    = 31,
        .diglen    = 20,
    },
    { /* TEST VECTOR 5 */
        .input    = "",
        .digest = "\xda\x39\xa3\xee\x5e\x6b\x4b\x0d"
              "\x32\x55\xbf\xef\x95\x60\x18\x90"
              "\xaf\xd8\x07\x09",
        .ilen    = 0,
        .diglen    = 20,
    },
};

/*********************************************************************
**  SHA256 TEST VECTORS
*********************************************************************/
static struct test_vector sha256_tv[] = {
    { /* TEST VECTOR 1 */
        .input    = "abc",
        .ilen    = 3,
        .digest    = "\xba\x78\x16\xbf\x8f\x01\xcf\xea"
              "\x41\x41\x40\xde\x5d\xae\x22\x23"
              "\xb0\x03\x61\xa3\x96\x17\x7a\x9c"
              "\xb4\x10\xff\x61\xf2\x00\x15\xad",
        .diglen    = 32,
    }, { /* TEST VECTOR 2 */
        .input    = "abcdbcdecdefdefgefghfghighijhijkij"
              "kljklmklmnlmnomnopnopq",
        .ilen    = 56,
        .digest    = "\x24\x8d\x6a\x61\xd2\x06\x38\xb8"
              "\xe5\xc0\x26\x93\x0c\x3e\x60\x39"
              "\xa3\x3c\xe4\x59\x64\xff\x21\x67"
              "\xf6\xec\xed\xd4\x19\xdb\x06\xc1",
        .diglen    = 32,
    }, { /* TEST VECTOR 3 */
        .input    = "Test vector from febooti.com",
        .digest = "\x07\x7b\x18\xfe\x29\x03\x6a\xda"
              "\x48\x90\xbd\xec\x19\x21\x86\xe1"
              "\x06\x78\x59\x7a\x67\x88\x02\x90"
              "\x52\x1d\xf7\x0d\xf4\xba\xc9\xab",
        .ilen    = 28,
        .diglen    = 32,
    }, { /* TEST VECTOR 4 */
        .input    = "",
        .digest = "\xe3\xb0\xc4\x42\x98\xfc\x1c\x14"
              "\x9a\xfb\xf4\xc8\x99\x6f\xb9\x24"
              "\x27\xae\x41\xe4\x64\x9b\x93\x4c"
              "\xa4\x95\x99\x1b\x78\x52\xb8\x55",
        .ilen    = 0,
        .diglen    = 32,
    },
};


FILE *fp;
int key_sz = 32;

static int verbose;
static int bringup;
static int internal_test_enabled;

unsigned char test_128K[_128K];
unsigned char test_128K_verify[_128K];

enum test_types {
    INTERNAL,
    NOMINAL,
    ADVERSARIAL,
    REPEAT,
    STRESS,
    PERFORMANCE,
    LAST_TEST,
};

struct option testopts[] = {
    {"Internal", required_argument, NULL,'i'},
    {"Nominal", no_argument, NULL, 'n'},
    {"Adversarial", no_argument, NULL, 'a'},
    {"Repeatability", no_argument, NULL, 'r'},
    {"Stress", no_argument, NULL, 's'},
    {"Performance", no_argument, NULL, 'p'},
    {"Verbose", no_argument, NULL, 'v'},
    {"Help", no_argument, NULL, 'h'},
    {NULL, 0, NULL, 0},
};



/*********************************************************************
**  Data types for Performance Testing
*********************************************************************/
static unsigned int enc_time_record[NUM_ITERATIONS];
static unsigned int dec_time_record[NUM_ITERATIONS];
static unsigned int sha_time_record[NUM_ITERATIONS];

/*********************************************************************
**  Driver handles (qcedev and pmem)
*********************************************************************/
static int qcedev_fd;
pthread_mutex_t plock;

/*********************************************************************
**  print_text() - print the text into the dmesg buffer.
*********************************************************************/
static void print_text(char *intro_message, unsigned char *text_addr,
                        unsigned int size)
{
    unsigned int   i;

    printf("%s @ address = 0x%p\n", intro_message, text_addr);
    for (i = 0;  i < size;  i++) {
        printf("%2x ", text_addr[i]);
        if ((i & 0xf) == 0xf)
            printf("\n");
    }
    printf("\n");
}

/*********************************************************************
** _time_diff() - Calculate the time difference in micro sec.
*********************************************************************/
static int _time_diff(struct timespec *stp, struct timespec *etp)
{
    int diff;

    if (etp->tv_sec !=  stp->tv_sec) {
        diff = (etp->tv_nsec + 1000000000 - stp->tv_nsec)/1000;
        etp->tv_sec--;
        diff += (etp->tv_sec -  stp->tv_sec) * 1000000;
    } else {
        diff = (etp->tv_nsec - stp->tv_nsec)/1000;
    }
    return diff;
};

static int qcedev_cipher_vbuf_speedtest(int fd, int packet_size,
                    enum qcedev_cipher_alg_enum alg,
                    enum qcedev_cipher_mode_enum mode,
                    uint32_t key_sz)

{

    unsigned long long total_time = 0;
    int i, j;
    struct qcedev_cipher_op_req  req;
    struct timespec stp;
    struct timespec etp;
    unsigned int  usec;
    int first_time = 1;
    static unsigned char iv[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    printf("\n\n");
    printf("---------------------------------------------------\n");
    printf("cipher_vbuf_speedtest packet_size  = 0x%x\n", packet_size);
    printf("---------------------------------------------------\n");

    for (i = 0; i < 16; i++)
        iv[i] = (1 + i) % NUM_ITERATIONS;

    for (i = 0; i < (_32K * 4); i++)
        test_128K[i] = i % 255;


    for (i = 0; i < NUM_ITERATIONS; i++) {
        enc_time_record[i] = 0;
        dec_time_record[i] = 0;
    }

    memset(req.enckey, 0, 32);
    memset(req.iv, 0, 32);
    memcpy(req.enckey, key, key_sz);
    memcpy(req.iv, iv, 16);
    req.ivlen = 16;

    req.byteoffset  = 0;
    req.data_len = packet_size;
    req.encklen  = key_sz;
    req.alg = alg;
    req.mode = mode;

    req.use_pmem = 0;

    req.vbuf.src[0].vaddr =  &test_128K[0];
    req.vbuf.src[0].len =  packet_size;
    req.vbuf.dst[0].vaddr =  &test_128K[0];
    req.vbuf.dst[0].len =  packet_size;

    req.in_place_op = 1;
    req.entries = 1;

    req.op = QCEDEV_OPER_ENC;
    printf("Encryption packet size %d\n", packet_size);

    for (i = 0; i < NUM_ITERATIONS; i++) {
        if (((i % 200) == 0) && (i > 0))
            printf("X");
        if ((verbose == 1) && (first_time == 1))
            print_text("Original data", &test_128K[0],
                    (unsigned int)(packet_size/64));


        if (clock_gettime(CLOCK_MONOTONIC, &stp) < 0) {
            printf("can't get time\n");
            return 0;
        }

        ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);

        if (clock_gettime(CLOCK_MONOTONIC, &etp) < 0) {
            printf("can't get time\n");
            return 0;
        }

        usec = _time_diff(&stp, &etp);
        enc_time_record[i] = usec;
        if ((verbose == 1) && (first_time == 1)) {
            print_text("Encrypted data", &test_128K[0],
                            packet_size/64);
            printf("\nTime for encrypting packet (0x%x) = %d usec\n"
                    , (uint32_t)packet_size
                    , (uint32_t)usec);
        }

        memcpy(req.iv, iv, 16);
        req.op = QCEDEV_OPER_DEC;
        if (clock_gettime(CLOCK_MONOTONIC, &stp) < 0) {
            printf("can't get time\n");
            return 0;
        }

        ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);

        if (clock_gettime(CLOCK_MONOTONIC, &etp) < 0) {
            printf("can't get time\n");
            return 0;
        }
        usec = _time_diff(&stp, &etp);
        dec_time_record[i] = usec;
        if ((verbose == 1) && (first_time == 1)) {
            printf("Time for decrypting packet (0x%x) = %d usec\n",
                    (uint32_t)packet_size, (uint32_t)usec);
            print_text("Decrypted data", &test_128K[0],
                            packet_size/64);
            first_time = 0;
        }

        for (j = 0; j < (_32K * 4); j++)
            test_128K[j] = j % 255;

        memcpy(req.iv, iv, 16);
        req.alg = QCEDEV_ALG_AES;
        req.mode = QCEDEV_AES_MODE_CTR;

        req.vbuf.src[0].vaddr = &test_128K[0];
        req.vbuf.src[0].len = packet_size;
        req.vbuf.dst[0].vaddr = &test_128K[0];
        req.vbuf.dst[0].len = packet_size;

        req.data_len = packet_size;
        req.op = QCEDEV_OPER_ENC;
        req.use_pmem = 0;
        req.in_place_op = 1;
        req.byteoffset = 0;
        req.entries = 1;

        /* Reset first time to 0 at the end of first iteration*/
        if (i == 0)
            first_time = 0;
    }
    total_time = 0;
    for (i = 0; i < NUM_ITERATIONS; i++)
        total_time += (unsigned long)enc_time_record[i];
    printf("\nTotal enc time for %d iteration of packet (0x%x b)= %d us\n",
        NUM_ITERATIONS, packet_size, (unsigned int)total_time);
    printf("Average enc time for %d iteration  = %d usec\n",
        NUM_ITERATIONS, (unsigned int)(total_time/NUM_ITERATIONS));

    fprintf(fp, "Enc::PacketSize = %d:: AverageTime = %d\n", packet_size,
        (unsigned int)(total_time/NUM_ITERATIONS));

    total_time = 0;
    for (i = 0; i < NUM_ITERATIONS; i++)
        total_time += (unsigned long)dec_time_record[i];
    printf("\nTotal dec time for %d iteration of packet (0x%x b)= %d us\n",
        NUM_ITERATIONS, packet_size, (unsigned int)total_time);
    printf("Average dec time for %d iteration  = %d usec\n",
        NUM_ITERATIONS, (unsigned int)(total_time/NUM_ITERATIONS));

    fprintf(fp, "Dec::PacketSize = %d:: AverageTime = %d\n", packet_size,
        (unsigned int)(total_time/NUM_ITERATIONS));
    printf("\n------------------------------------------------\n");

    return 1;
}


static int qcedev_sha_vbuf_speedtest(int fd, int packet_size,
                    enum qcedev_sha_alg_enum alg, int iuf)
{
    unsigned long long total_time = 0;
    int i, j;
    struct qcedev_sha_op_req  req;
    struct timespec stp;
    struct timespec etp;
    unsigned int  usec;
    int first_time = 1;


    printf("\n\n");
    printf("---------------------------------------------------\n");
    printf("sha1_vbuf_speedtest packet_size  = 0x%x\n", packet_size);
    printf("---------------------------------------------------\n");

    for (i = 0; i < _128K; i++)
        test_128K[i] = i % 255;

    for (i = 0; i < NUM_ITERATIONS; i++)
        sha_time_record[i] = 0;

    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = packet_size;
    req.data_len = packet_size;
    req.entries = 1;
    memset(req.digest, 0, QCEDEV_MAX_SHA_DIGEST);
    req.alg = alg;

    printf("Input packet size %d bytes\n", packet_size);
    for (i = 0; i < NUM_ITERATIONS; i++) {
        if (((i % 200) == 0) && (i > 0))
            printf("X");
        if ((verbose == 1) && (first_time == 1))
            print_text("Original data", &test_128K[0],
                    (unsigned int)(packet_size/64));

        if (clock_gettime(CLOCK_MONOTONIC, &stp) < 0) {
            printf("can't get time\n");
            return 0;
        }

        if (iuf) {
            ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req);
            ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req);
            ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req);
        } else
            ioctl(fd, QCEDEV_IOCTL_GET_SHA_REQ, &req);

        if (clock_gettime(CLOCK_MONOTONIC, &etp) < 0) {
            printf("can't get time\n");
            return 0;
        }

        usec = _time_diff(&stp, &etp);
        sha_time_record[i] = usec;
        if ((verbose == 1) && (first_time == 1)) {
            print_text("Calculated Digest", (unsigned char *)
                            &req.digest,
                            req.diglen);
            printf("\nTime for creating digest on packet"
                " (0x%x) = %d usec\n", (uint32_t)packet_size,
                (uint32_t)usec);
            printf("Digest Length is %x\n", req.diglen);
        }

        req.data[0].vaddr = &test_128K[0];
        req.data[0].len = packet_size;
        req.data_len = packet_size;
        req.entries = 1;
        memset(req.digest, 0, QCEDEV_MAX_SHA_DIGEST);
        req.alg = alg;

        /* Reset first time to 0 at the end of first iteration*/
        if (i == 0)
            first_time = 0;
    }

    total_time = 0;
    for (i = 0; i < NUM_ITERATIONS; i++)
        total_time += (unsigned long)sha_time_record[i];
    printf("\nTotal SHA time for %d iteration of packet (0x%x b)= %d us\n",
        NUM_ITERATIONS, packet_size, (unsigned int)total_time);
    printf("Average SHA time for %d iteration  = %d usec\n",
        NUM_ITERATIONS, (unsigned int)(total_time/NUM_ITERATIONS));

    fprintf(fp, "SHA::PacketSize = %d:: AverageTime = %d\n", packet_size,
        (unsigned int)(total_time/NUM_ITERATIONS));
    printf("\n------------------------------------------------\n");

    return 1;
}

static int qcedev_set_iv(struct qcedev_cipher_op_req *req, unsigned char *iv,
                unsigned int ivlen,
                enum qcedev_cipher_mode_enum mode)
{
    memset(req->iv, 0, 32);
    if ((mode == QCEDEV_AES_MODE_ECB) || (mode == QCEDEV_DES_MODE_ECB))
        req->ivlen  = 0;
    else {
        memcpy(req->iv, iv, ivlen);
        req->ivlen  = ivlen;
    }
    return 0;
}

static int qcedev_cipher_aes_basic_test(int fd)
{
    int i, num_tv;
    struct qcedev_cipher_op_req  req;
    int errors = 0;
    unsigned char *lvbuf_src;
    unsigned char *lvbuf_dst;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_cipher_aes_basic_test fd  =0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }

    lvbuf_src = malloc(0x100);
    if (!lvbuf_src) {
        printf("Failed to allocate memory for src buffer\n");
        return -1;
    }
    lvbuf_dst = malloc(0x100);
    if (!lvbuf_dst) {
        printf("Failed to allocate memory for dst buffer\n");
        free(lvbuf_src);
        return -1;
    }
    num_tv = (sizeof(cipher_aes_tv))/(sizeof(struct test_vector));

    for (i = 0; i < num_tv; i++) {
        memset(req.enckey, 0, 32);
        memcpy(req.enckey, cipher_aes_tv[i].key,
                    cipher_aes_tv[i].klen);
        qcedev_set_iv(&req, (unsigned char *)cipher_aes_tv[i].iv,
                    cipher_aes_tv[i].ivlen,
                    cipher_aes_tv[i].mode);
        req.encklen  = cipher_aes_tv[i].klen;
        req.byteoffset  = 0;
        req.data_len = cipher_aes_tv[i].ilen;
        req.alg = cipher_aes_tv[i].c_alg;
        req.mode = cipher_aes_tv[i].mode;

        memset(&lvbuf_dst[0], 0xFF, 0x100);
        memset(&lvbuf_src[0], 0xFF, 0x100);
        memcpy(&lvbuf_src[0], cipher_aes_tv[i].input,
                    cipher_aes_tv[i].ilen);

        req.vbuf.src[0].vaddr = &lvbuf_src[0];
        req.vbuf.src[0].len = cipher_aes_tv[i].ilen;
        req.vbuf.dst[0].vaddr = &lvbuf_dst[0];
        req.vbuf.dst[0].len = cipher_aes_tv[i].rlen;

        req.use_pmem = 0;
        req.in_place_op = 0;
        req.entries = 1;

        req.op = QCEDEV_OPER_ENC;
        ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);
        if (!(memcmp(&lvbuf_dst[0], cipher_aes_tv[i].result,
                    cipher_aes_tv[i].rlen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("AES mode %d Encryption Failed: tv %d\n",
                        cipher_aes_tv[i].mode, i);
                print_text("Input data",
                        (unsigned char *)
                        cipher_aes_tv[i].input,
                        cipher_aes_tv[i].ilen);
                print_text("Encrypted Data", &lvbuf_dst[0],
                        cipher_aes_tv[i].ilen);
                print_text("Expected encrypted data",
                        (unsigned char *)
                        cipher_aes_tv[i].result,
                        cipher_aes_tv[i].rlen);
            }
        }
    }

    for (i = 0; i < num_tv; i++) {
        memset(req.enckey, 0, 32);
        memcpy(req.enckey, cipher_aes_tv[i].key,
                        cipher_aes_tv[i].klen);
        qcedev_set_iv(&req, (unsigned char *)cipher_aes_tv[i].iv,
                        cipher_aes_tv[i].ivlen,
                        cipher_aes_tv[i].mode);
        req.encklen  = cipher_aes_tv[i].klen;
        req.byteoffset  = 0;
        req.data_len = cipher_aes_tv[i].rlen;
        req.alg = cipher_aes_tv[i].c_alg;
        req.mode = cipher_aes_tv[i].mode;

        memset(&lvbuf_dst[0], 0xFF, 0x100);
        memset(&lvbuf_src[0], 0xFF, 0x100);
        memcpy(&lvbuf_src[0], cipher_aes_tv[i].result,
                    cipher_aes_tv[i].rlen);

        req.vbuf.src[0].vaddr = &lvbuf_src[0];
        req.vbuf.src[0].len = cipher_aes_tv[i].rlen;
        req.vbuf.dst[0].vaddr = &lvbuf_dst[0];
        req.vbuf.dst[0].len = cipher_aes_tv[i].rlen;

        req.use_pmem = 0;
        req.in_place_op = 0;
        req.entries = 1;
        req.op = QCEDEV_OPER_DEC;
        ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);
        if (!(memcmp(&lvbuf_dst[0], cipher_aes_tv[i].input,
                    cipher_aes_tv[i].ilen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("AES mode %d Decryption Failed: tv=%d\n",
                        cipher_aes_tv[i].mode, i);
                print_text("Input data",
                        (unsigned char *)
                        cipher_aes_tv[i].result,
                        cipher_aes_tv[i].rlen);
                print_text("Decrypted Data", &lvbuf_dst[0],
                        cipher_aes_tv[i].ilen);
                print_text("Expected decrypted data",
                        (unsigned char *)
                        cipher_aes_tv[i].input,
                        cipher_aes_tv[i].ilen);
            }
        }
    }

    if (verbose || internal_test_enabled) {
        if (errors  == 0)
            printf("Basic AES Cipher Test: PASS!!!\n");
        else
            printf("Basic AES Cipher Test: FAILED %d Test(s) "
                "out of %d\n", errors, num_tv);
    }

    free(lvbuf_src);
    free(lvbuf_dst);

    return errors;
}

static int qcedev_cipher_des_basic_test(int fd)
{
    int i, num_tv;
    struct qcedev_cipher_op_req  req;
    int errors = 0;
    unsigned char *lvbuf_src;
    unsigned char *lvbuf_dst;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_cipher_des_basic_test fd  =0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    lvbuf_src = malloc(0x100);
    if (!lvbuf_src) {
        printf("Failed to allocate memory for src buffer\n");
        return -1;
    }
    lvbuf_dst = malloc(0x100);
    if (!lvbuf_dst) {
        printf("Failed to allocate memory for dst buffer\n");
        free(lvbuf_src);
        return -1;
    }

    num_tv = (sizeof(cipher_des_tv))/(sizeof(struct test_vector));

    for (i = 0; i < num_tv; i++) {
        memset(req.enckey, 0, 32);
        memcpy(req.enckey, cipher_des_tv[i].key,
                    cipher_des_tv[i].klen);
        qcedev_set_iv(&req, (unsigned char *)cipher_des_tv[i].iv,
                    cipher_des_tv[i].ivlen,
                    cipher_des_tv[i].mode);
        req.encklen  = cipher_des_tv[i].klen;
        req.byteoffset  = 0;
        req.data_len = cipher_des_tv[i].ilen;
        req.alg = cipher_des_tv[i].c_alg;
        req.mode = cipher_des_tv[i].mode;

        memset(&lvbuf_dst[0], 0xFF, 0x100);
        memset(&lvbuf_src[0], 0xFF, 0x100);
        memcpy(&lvbuf_src[0], cipher_des_tv[i].input,
                    cipher_des_tv[i].ilen);

        req.vbuf.src[0].vaddr = &lvbuf_src[0];
        req.vbuf.src[0].len = cipher_des_tv[i].ilen;
        req.vbuf.dst[0].vaddr = &lvbuf_dst[0];
        req.vbuf.dst[0].len = cipher_des_tv[i].rlen;

        req.use_pmem = 0;
        req.in_place_op = 0;
        req.entries = 1;

        req.op = QCEDEV_OPER_ENC;
        ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);
        if (!(memcmp(&lvbuf_dst[0], cipher_des_tv[i].result,
                    cipher_des_tv[i].rlen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("DES mode %d Encryption Failed: tv %d\n",
                        cipher_des_tv[i].mode, i);
                print_text("Input data",
                        (unsigned char *)
                        cipher_des_tv[i].input,
                        cipher_des_tv[i].ilen);
                print_text("Encrypted Data", &lvbuf_dst[0],
                        cipher_des_tv[i].ilen);
                print_text("Expected encrypted data",
                        (unsigned char *)
                        cipher_des_tv[i].result,
                        cipher_des_tv[i].rlen);
            }
        }
    }

    for (i = 0; i < num_tv; i++) {
        memset(req.enckey, 0, 32);
        memcpy(req.enckey, cipher_des_tv[i].key,
                        cipher_des_tv[i].klen);
        qcedev_set_iv(&req, (unsigned char *)cipher_des_tv[i].iv,
                        cipher_des_tv[i].ivlen,
                        cipher_des_tv[i].mode);
        req.encklen  = cipher_des_tv[i].klen;
        req.byteoffset  = 0;
        req.data_len = cipher_des_tv[i].rlen;
        req.alg = cipher_des_tv[i].c_alg;
        req.mode = cipher_des_tv[i].mode;

        memset(&lvbuf_dst[0], 0xFF, 0x100);
        memset(&lvbuf_src[0], 0xFF, 0x100);
        memcpy(&lvbuf_src[0], cipher_des_tv[i].result,
                    cipher_des_tv[i].rlen);

        req.vbuf.src[0].vaddr = &lvbuf_src[0];
        req.vbuf.src[0].len = cipher_des_tv[i].rlen;
        req.vbuf.dst[0].vaddr = &lvbuf_dst[0];
        req.vbuf.dst[0].len = cipher_des_tv[i].rlen;

        req.use_pmem = 0;
        req.in_place_op = 0;
        req.entries = 1;
        req.op = QCEDEV_OPER_DEC;
        ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);
        if (!(memcmp(&lvbuf_dst[0], cipher_des_tv[i].input,
                    cipher_des_tv[i].ilen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("DES mode %d Decryption Failed: tv=%d\n",
                        cipher_des_tv[i].mode, i);
                print_text("Input data",
                        (unsigned char *)
                        cipher_des_tv[i].result,
                        cipher_des_tv[i].rlen);
                print_text("Decrypted Data", &lvbuf_dst[0],
                        cipher_des_tv[i].ilen);
                print_text("Expected decrypted data",
                        (unsigned char *)
                        cipher_des_tv[i].input,
                        cipher_des_tv[i].ilen);
            }
        }
    }

    if (verbose || internal_test_enabled) {
        if (errors  == 0)
            printf("Basic DES Cipher Test: PASS!!!\n");
        else
            printf("Basic DES Cipher Test: FAILED %d Test(s) "
                "out of %d\n", errors, num_tv);
    }
    free(lvbuf_src);
    free(lvbuf_dst);
    return errors;
}

static int qcedev_cipher_basic_test(int fd)
{
    int errors = 0;
    errors += qcedev_cipher_aes_basic_test(fd);
    errors += qcedev_cipher_des_basic_test(fd);
    return errors;
}



static int qcedev_cipher_vbuf_mult_sg_large_buf_test(int fd,
                enum qcedev_cipher_alg_enum alg,
                enum qcedev_cipher_mode_enum mode,
                unsigned char *key, unsigned char * iv,
                int no_key, uint32_t key_sz)
{
    int i;
    struct qcedev_cipher_op_req  req;
    int dec_offset = 0;
    int pass = 0;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_cipher_vbuf_mult_sg_large_buf_test fd=0x%x\n",
                                    fd);
        printf("---------------------------------------------------\n");
    }
    memset(req.enckey, 0, 32);
    memcpy(req.enckey, key, key_sz);
    qcedev_set_iv(&req, iv, 16, mode);

    req.byteoffset  = 0;
    req.encklen  = key_sz;
    req.alg = alg;
    req.mode = mode;
    req.use_pmem = 0;
    req.in_place_op = 1;

    for (i = 0; i < (_32K * 4); i++) {
        test_128K[i] = i % 255;
        test_128K_verify[i] = test_128K[i];
    }
    req.vbuf.src[0].vaddr = &test_128K[0];
    req.vbuf.src[0].len = _32K * 2;
    req.vbuf.src[1].vaddr = &test_128K[_32K * 3];
    req.vbuf.src[1].len = _32K;
    req.entries = 2;

    req.vbuf.dst[0].vaddr = &test_128K[0];
    req.vbuf.dst[0].len = _32K * 2;
    req.vbuf.dst[1].vaddr = &test_128K[_32K * 3];
    req.vbuf.dst[1].len = _32K;

    req.data_len = _32K * 3;
    req.op = QCEDEV_OPER_ENC;

    ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);
    if (bringup == 1) {
        print_text("\nEncrypted data src", &test_128K[0], (_32K)/200);
        print_text("\nEncrypted data src +_32K*3", &test_128K[_32K*3],
                                (_32K)/200);
    }
    if (no_key) {
        req.op = QCEDEV_OPER_DEC_NO_KEY;
        req.encklen = 0;
        memset(req.enckey, 0, 32);
    }

    qcedev_set_iv(&req, iv, 16, mode);
    req.byteoffset  = dec_offset;
    req.vbuf.src[0].vaddr = &test_128K[dec_offset];
    req.vbuf.src[0].len = (_32K * 2) - dec_offset;
    req.vbuf.src[1].vaddr = &test_128K[_32K * 3];
    req.vbuf.src[1].len = _32K;
    req.entries = 2;

    req.vbuf.dst[0].vaddr = &test_128K[dec_offset];
    req.vbuf.dst[0].len = (_32K * 2) - dec_offset;
    req.vbuf.dst[1].vaddr = &test_128K[_32K * 3];
    req.vbuf.dst[1].len = _32K;
    req.op = QCEDEV_OPER_DEC;
    req.data_len = (_32K * 3) - dec_offset;
    ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);

    pass = memcmp(&test_128K[0], &test_128K_verify[0], (_32K * 4));
    if (((pass != 0) || (bringup == 1)) && (verbose == 1)) {
        print_text("\nOriginal data src", &test_128K_verify[0],
                                (_32K)/200);
        print_text("\nOriginal data src +_32K*3",
                &test_128K_verify[(_32K * 3)], (_32K)/200);
        print_text("\nDecrypted data src", &test_128K[0], (_32K)/200);
        print_text("\nDecrypted data src +_32K*3", &test_128K[_32K*3],
                                (_32K)/200);
    }

    if (pass == 0) {
        if (verbose)
            printf("Decryption of 32KB_plus packet (alg %x, mode"
                "%x) PASSED\n", (uint32_t)alg, (uint32_t)mode);
        return 1;
    } else {
        printf("Decryption of 32KB_plus packet (alg %x, mode %x) "
                "FAILED\n", (uint32_t)alg, (uint32_t)mode);
        return 0;
    }
}

static int qcedev_cipher_vbuf_mult_sg_test(int fd,
            enum qcedev_cipher_alg_enum alg,
            enum qcedev_cipher_mode_enum mode, unsigned char *key,
            unsigned char *iv, int no_key, uint32_t key_sz)
{
    int i;
    struct qcedev_cipher_op_req  req;
    int sg_size0, sg_size1, sg_size2;
    int pass = 0;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_cipher_vbuf_mult_sg_test fd =0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    memset(req.enckey, 0, 32);
    memcpy(req.enckey, key, key_sz);
    qcedev_set_iv(&req, iv, 16, mode);

    if ((alg == QCEDEV_ALG_AES) &&
        (mode == QCEDEV_AES_MODE_CTR)) {
        sg_size0 = 58;
        sg_size1 = 42;
        sg_size2 = 28;
    } else {
        sg_size0 = 0x40;
        sg_size1 = 0x30;
        sg_size2 = 0x10;
    }
    req.byteoffset  = 0;
    req.data_len = sg_size0 + sg_size1 + sg_size2;
    req.encklen = key_sz;
    req.alg = alg;
    req.mode = mode;
    req.use_pmem = 0;
    req.in_place_op = 1;

    memset(&vbuf_src[0], 0xFF , 0x1000);
    memcpy(&vbuf_src[0], &PlainText_90[0], sg_size0);
    memcpy(&vbuf_src[0] + 100, &PlainText_60[0], sg_size1);
    memcpy(&vbuf_src[0] + 200, &PlainText_90[0], sg_size2);

    req.vbuf.src[0].vaddr = &vbuf_src[0];
    req.vbuf.src[0].len = sg_size0;
    req.vbuf.src[1].vaddr = &vbuf_src[100];
    req.vbuf.src[1].len = sg_size1;
    req.vbuf.src[2].vaddr = &vbuf_src[200];
    req.vbuf.src[2].len = sg_size2;
    req.entries = 3;

    req.vbuf.dst[0].vaddr = &vbuf_src[0];
    req.vbuf.dst[0].len = sg_size0;
    req.vbuf.dst[1].vaddr = &vbuf_src[100];
    req.vbuf.dst[1].len = sg_size1;
    req.vbuf.dst[2].vaddr = &vbuf_src[200];
    req.vbuf.dst[2].len = sg_size2;

    /* copy original data for verification */
    memset(&test_128K[0], 0xFF, 0x1000);
    memcpy(&test_128K[0], &vbuf_src[0], 200 + sg_size2);

    req.op = QCEDEV_OPER_ENC;
    ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);

    /* copy encrypted data for verification */
    memset(&test_128K_verify[0], 0xFF, 0x1000);
    memcpy(&test_128K_verify[0], &vbuf_src[0], 200 + sg_size2);


    qcedev_set_iv(&req, iv, 16, mode);
    req.byteoffset = 0;
    req.op = QCEDEV_OPER_DEC;
    ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);

    pass = memcmp(&test_128K[0], &vbuf_src[0], 200 + sg_size2);

    if (((pass != 0) || (bringup == 1)) && (verbose == 1)) {
        print_text("Original data", &test_128K[0], 200 + sg_size2);
        print_text("Encrypted data", &test_128K_verify[0],
                            200 + sg_size2);
        print_text("Decrypted data", &vbuf_src[0], 400);
    }

    if (pass == 0) {
        if (verbose)
            printf("Encryption/Decryption multi-sg (alg %x, mode "
                "%x) PASSED\n", (uint32_t)alg, (uint32_t)mode);
        return 1;
    } else {
            printf("Encryption/Decryption multi-sg(alg %x, mode "
                "%x) FAILED\n", (uint32_t)alg, (uint32_t)mode);
        return 0;
    }

}

static int qcedev_cipher_vbuf_single_sg_test(int fd,
            enum qcedev_cipher_alg_enum alg,
            enum qcedev_cipher_mode_enum mode,
            unsigned char *data_src,
            unsigned char *data_dst, unsigned int len,
            unsigned char *key, unsigned char* iv, int no_key,
            uint32_t offset,
            uint32_t key_sz)
{
    int i;
    struct qcedev_cipher_op_req  req;
    int pmem_fd;
    void *data;
    int use_pmem = 0;
    offset = 0;
    int pass = 0;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_cipher_vbuf_single_sg_test fd  =0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    memset(req.enckey, 0, 32);
    memcpy(req.enckey, key, key_sz);
    qcedev_set_iv(&req, iv, 16, mode);

    req.byteoffset  = 0;
    req.data_len = len;
    req.encklen  = key_sz;

    req.alg = alg;
    req.mode = mode;

    req.vbuf.src[0].vaddr =  data_src;
    req.vbuf.src[0].len =  len;
    req.vbuf.dst[0].vaddr =  data_dst;
    req.vbuf.dst[0].len =  len;

    req.use_pmem = 0;
    req.in_place_op = 1;
    req.entries = 1;

    req.op = QCEDEV_OPER_ENC;

    /* copy original data for verification of enc/dec */
    memset(&vbuf_src[0], 0xFF, 0x1000);
    memcpy(&vbuf_src[0], data_src, len);

    ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req);

    /* copy encrypted data for verification of enc/dec */
    memset(&vbuf_dst[0], 0xFF, 0x1000);
    memcpy(&vbuf_dst[0], data_dst, len);

    if (no_key) {
        req.op = QCEDEV_OPER_DEC_NO_KEY;
        req.encklen = 0;
        memset(req.enckey, 0, 32);
    }

    req.data_len -= offset;
    req.byteoffset = offset;

    req.vbuf.src[0].vaddr = data_dst + offset;
    req.vbuf.src[0].len = req.data_len;
    req.vbuf.dst[0].vaddr = req.vbuf.src[0].vaddr ;
    req.vbuf.dst[0].len = req.data_len;

    if (offset) {
        if (verbose) {
            printf("Encrypted data starts at offset 0x%x\n",
                                offset);
            print_text("Modified Encrypted data\n",
                (unsigned char *)data_dst, req.data_len);
        }
    }
    req.op = QCEDEV_OPER_DEC;
    qcedev_set_iv(&req, iv, 16, mode);
    ioctl(fd, QCEDEV_IOCTL_DEC_REQ, &req);

    pass = memcmp((data_dst + offset), (uint8_t *)&vbuf_src[0] + offset,
                            req.data_len);

    if (((pass != 0) || (bringup == 1)) && (verbose == 1)) {
        print_text("\nOriginal data src", &vbuf_src[0] + offset,
                        req.data_len);
        print_text("\nEncrypted data ", &vbuf_dst[0] +
                        offset,    req.data_len);
        print_text("\nDecrypted data ",
                (unsigned char *)(data_dst + offset),
                req.data_len);
    }

    if (pass == 0) {
        if (verbose)
            printf("Encryption single-sg(alg %x, mode %x) PASSED\n",
                        (uint32_t)alg, (uint32_t)mode);
        return 1;
    } else {
        printf("Decryption single-sg (alg %x, mode %x) FAILED\n",
                        (uint32_t)alg, (uint32_t)mode);
        return 0;
    }
}

static int qcedev_get_SHA(struct qcedev_sha_op_req *req, int fd, int alg)
{
    uint8_t digest[QCEDEV_MAX_SHA_DIGEST];
    memset(&digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    memset(&req->digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req->alg = alg;

    ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, req);
    ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, req);
    ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, req);

    memcpy(&digest[0], &req->digest[0], QCEDEV_MAX_SHA_DIGEST);
    memset(&req->digest[0], 0, QCEDEV_MAX_SHA_DIGEST);

    ioctl(fd, QCEDEV_IOCTL_GET_SHA_REQ, req);
    if (verbose || (internal_test_enabled == 1))
        print_text("Calculated Digest", (unsigned char *)&req->digest,
                        req->diglen);

    if (memcmp(&req->digest[0], &digest[0], QCEDEV_MAX_SHA_DIGEST) == 0) {
        return 1;
    } else {
        printf("FAILED to calculate hash (qcedev_get_SHA)alg = %d\n",
                                    alg);
        return 0;
    }
}

static int qcedev_sha_basic_test(int fd, int alg)
{
    struct qcedev_sha_op_req  req;
    struct test_vector *curr_test_vector;
    int errors = 0;
    unsigned int i, num_tv;

    if (verbose == 1) {
        printf("\n\n");
        printf("----------------------------------------------\n");
        printf(" sha_basic_test: fd = 0x%x\n", fd);
    }

    req.alg = alg;
    switch (req.alg) {
    case QCEDEV_ALG_SHA1:
        if (verbose == 1)
            printf(" SHA1 Algorithm test\n");
        num_tv = (sizeof(sha1_tv))/(sizeof(struct test_vector));
        curr_test_vector = &sha1_tv[0];
        break;

    case QCEDEV_ALG_SHA256:
        if (verbose == 1)
            printf(" SHA256 Algorithm test\n");
        num_tv = (sizeof(sha256_tv))/(sizeof(struct test_vector));
        curr_test_vector = &sha256_tv[0];
        break;

    case QCEDEV_ALG_SHA1_HMAC:
        if (verbose == 1)
            printf(" SHA1-HMAC Algorithm test\n");
        num_tv = (sizeof(hmac_sha1_tv))/(sizeof(struct test_vector));
        curr_test_vector = &hmac_sha1_tv[0];
        break;

    case QCEDEV_ALG_SHA256_HMAC:
        if (verbose == 1)
            printf(" SHA256-HMAC Algorithm test\n");
        num_tv = (sizeof(hmac_sha256_tv))/(sizeof(struct test_vector));
        curr_test_vector = &hmac_sha256_tv[0];
        break;
    default:
        printf("Invalid algorithm = %d\n", alg);
        return 1;
    }

    if (verbose)
        printf("----------------------------------------------\n");

    for (i = 0; i < num_tv; i++) {
        req.data[0].vaddr = (uint8_t *)curr_test_vector[i].input;
        req.data[0].len = curr_test_vector[i].ilen;
        req.data_len = curr_test_vector[i].ilen;
        req.diglen = curr_test_vector[i].diglen;
        req.entries = 1;

        if (req.alg == QCEDEV_ALG_SHA1_HMAC ||
                req.alg == QCEDEV_ALG_SHA256_HMAC) {
            req.authkey = (uint8_t *)curr_test_vector[i].key;
            req.authklen =    curr_test_vector[i].klen;
            req.diglen = curr_test_vector[i].diglen;
        }

        memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
        qcedev_get_SHA(&req, fd, req.alg);

        if (!(memcmp(&req.digest[0], curr_test_vector[i].digest,
            curr_test_vector[i].diglen) == 0)) {
            errors += 1;
            if (verbose == 1) {
                printf("SHA test failed\n");
                print_text("input data",
                        (unsigned char *)
                        curr_test_vector[i].input,
                        curr_test_vector[i].ilen);
                print_text("Calculated Digest",
                        (unsigned char *)&req.digest[0],
                        curr_test_vector[i].diglen);
                print_text("Expected Digest",
                        (unsigned char *)
                        curr_test_vector[i].digest,
                        curr_test_vector[i].diglen);
            }
        }
    }

    if (verbose || internal_test_enabled) {
        if (errors == 0)
            printf("SHA TEST: PASS !!!\n");
        else
            printf("SHA TEST: FAILED %d TESTS OUT OF %d\n",
                    errors, num_tv);
    }

    return errors;
}

static int qcedev_SHA1_SHA256_basic_test(int fd)
{
    int errors = 0;

    if (verbose == 1) {
        printf("\n\n");
        printf("----------------------------------------------\n");
        printf(" qcedev_SHA1_SHA256_basic_test: fd = 0x%x\n", fd);
        printf("----------------------------------------------\n");
    }

    errors = qcedev_sha_basic_test(fd, QCEDEV_ALG_SHA1);
    errors += qcedev_sha_basic_test(fd, QCEDEV_ALG_SHA256);
#ifdef CRYPTO_HMAC_SUPPORT
    errors += qcedev_sha_basic_test(fd, QCEDEV_ALG_SHA1_HMAC);
    errors += qcedev_sha_basic_test(fd, QCEDEV_ALG_SHA256_HMAC);
#endif

    if ((errors == 0) && (verbose == 1))
#ifdef CRYPTO_HMAC_SUPPORT
        printf("SHA(sha1,sha256,sha1-hmac,sha1-256)Basic Test: PASS\n");
#else
        printf("SHA(sha1,sha256)Basic Test: PASS\n");
#endif
    if (errors > 0)
        return 0;
    else
        return 1;
}

#ifdef CRYPTO_CMAC_SUPPORT
static int qcedev_CMAC_basic_test(int fd)
{
    struct qcedev_sha_op_req  req;
    int errors = 0;
    int i, num_tv;
    if (verbose == 1) {
        printf("\n\n");
        printf("----------------------------------------------\n");
        printf(" qcedev_CMAC_basic_test: fd = 0x%x\n", fd);
        printf("----------------------------------------------\n");
    }
    num_tv = (sizeof(aes_cmac_tv))/(sizeof(struct test_vector));

    for (i = 0; i < num_tv; i++) {
        req.alg = QCEDEV_ALG_AES_CMAC ;
        req.data[0].vaddr = (uint8_t *)aes_cmac_tv[i].input;
        req.data[0].len = aes_cmac_tv[i].ilen;
        req.data_len = aes_cmac_tv[i].ilen;
        req.authkey = (uint8_t *)aes_cmac_tv[i].key;
        req.authklen = aes_cmac_tv[i].klen;
        req.diglen = 16;
        req.entries = 1;

        memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
        ioctl(fd, QCEDEV_IOCTL_GET_CMAC_REQ, &req);

        if (!(memcmp(&req.digest[0], aes_cmac_tv[i].digest,
                    aes_cmac_tv[i].diglen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("CMAC test failed\n");
                print_text("Calculated CMAC :",    &req.digest[0],
                            aes_cmac_tv[i].diglen);
                print_text("Expected CMAC :",
                            (unsigned char *)
                            aes_cmac_tv[i].digest,
                            aes_cmac_tv[i].diglen);
            }
        }
    }
    if (verbose || internal_test_enabled) {
        if (errors  == 0)
            printf("CMAC Basic Test: PASS !!!\n");
        else
            printf("CMAC BASIC TEST: FAILED %d TESTS"
                "OUT OF %d\n", errors, num_tv);
    }
    return errors;
}
#else
static int qcedev_CMAC_basic_test(int fd)
{
    printf("CMAC is not supported on this device\n");
    return 0;
}
#endif

static int qcedev_SHA1_SHA256_multiple_of_64b(int fd)
{
    struct qcedev_sha_op_req  req;

    if (verbose) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf(" qcedev_SHA1_SHA256_x64: fd = 0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    req.data[0].vaddr = &PlainText_90[0];
    req.data[0].len = 0x80;
    req.data_len = 0x80;
    req.diglen = 0;
    req.entries = 1;

    qcedev_get_SHA(&req, fd, QCEDEV_ALG_SHA1);
    qcedev_get_SHA(&req, fd, QCEDEV_ALG_SHA256);

    return 1;
}

static int qcedev_SHA1_SHA256_multi_updates(int fd)
{
    struct qcedev_sha_op_req  req;
    struct qcedev_sha_op_req  req0;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf("qcedev_SHA1_SHA256_xupdates: fd = 0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &PlainText_90[0];
    req.data[0].len = 0x50;
    req.data_len = 0x50;
    req.diglen = 0;
    req.entries = 1;
    req.alg = QCEDEV_ALG_SHA1;

    ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req);

    ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req);
    req.data[0].vaddr = &PlainText_90[0x50];
    req.data[0].len = 0x40;
    req.data_len = 0x40;

    ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req);

    ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req);

    memset(&req0.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req0.data[0].vaddr = &PlainText_90[0];
    req0.data[0].len = 0x90;
    req0.data_len = 0x90;
    req0.diglen = 0;
    req0.entries = 1;
    req0.alg = QCEDEV_ALG_SHA1;

    ioctl(fd, QCEDEV_IOCTL_GET_SHA_REQ, &req0);

    if (bringup == 1) {
        print_text("\nOriginal data", &PlainText_90[0], 0x90);
        print_text("\nSHA1 Digest ", &req.digest[0], 0x14);
    }
    if (memcmp(&req.digest[0], &req0.digest[0], 0x14) == 0) {
        if (verbose || internal_test_enabled)
            printf("SHA multi updates PASS!!!\n");
        return 1;
    } else {
        if (verbose || internal_test_enabled) {
            printf("SHA multi updates FAILED!!!\n");
            print_text("\nExpected SHA1 Digest", &req0.digest[0],
                                0x14);
        }
        return 0;
    }
}

static int qcedev_SHA_test_32KB_plus(int fd, int alg)
{
    struct qcedev_sha_op_req  req;
    struct qcedev_sha_op_req  req0;
    int i = 0;
    int pass = 1;
    char digest[QCEDEV_MAX_SHA_DIGEST];

    if (verbose) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf(" qcedev_SHA_test_32KB_plus: fd = 0x%x, algo = %d\n",
                                fd, alg);
        printf("---------------------------------------------------\n");
        printf("Max Data PAcket Size = 0x%x\n", (uint32_t)_32K * 4);
    }
    /* Testing large buffer >32K*/
    for (i = 0; i < (_32K * 4); i++)
        test_128K[i] = i % 255;

    /*********************************************************************/
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = 38;
    req.data[1].vaddr = &test_128K[38];
    req.data[1].len = 34525;
    req.data_len = 38 + 34525;
    req.alg = alg;
    req.diglen = 0;
    req.entries = 2;

    qcedev_get_SHA(&req, fd, alg);

    memcpy(&digest[0], &req.digest[0], QCEDEV_MAX_SHA_DIGEST);

    /*********************************************************************/
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = 34525 ;
    req.data[1].vaddr = &test_128K[34525];
    req.data[1].len = 38;
    req.data_len  = 38 + 34525;
    req.diglen = 0;
    req.entries = 2;

    qcedev_get_SHA(&req, fd, alg);

    if (memcmp(&digest[0], &req.digest[0], QCEDEV_MAX_SHA_DIGEST) == 0)
        pass = 1;
    else {
        pass = 0;
        printf("SHA1 of large buffer failed\n");
    }

    /*********************************************************************/
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = _32K + 30 ;
    req.data[1].vaddr = &test_128K[_32K + 30];
    req.data[1].len = _32K + 90;
    req.data[2].vaddr = &test_128K[(2 * _32K) + 120];
    req.data[2].len = (2 * _32K) - 120;
    req.data_len = _32K * 4;
    req.diglen = 0;
    req.entries = 3;
    qcedev_get_SHA(&req, fd, alg);

    memcpy(&digest[0], &req.digest[0], QCEDEV_MAX_SHA_DIGEST);

    /*********************************************************************/
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = _32K + 30 ;
    req.data[1].vaddr = &test_128K[_32K + 30];
    req.data[1].len = 90;
    req.data[2].vaddr = &test_128K[_32K + 120];
    req.data[2].len = 50;
    req.data[3].vaddr = &test_128K[_32K + 170];
    req.data[3].len = (2 * _32K) + 90;
    req.data[4].vaddr = &test_128K[(3 * _32K) + 260];
    req.data[4].len = _32K - 260;
    req.data_len = _32K * 4;
    req.diglen = 0;
    req.entries = 5;

    qcedev_get_SHA(&req, fd, alg);

    if (memcmp(&digest[0], &req.digest[0], QCEDEV_MAX_SHA_DIGEST) == 0)
        pass = 1;
    else {
        pass = 0;
        if (verbose) {
            printf("SHA of 128 KB buffer in 5 sg entries failed\n");
            print_text("SHA Digest w/3 entries\n",
                        (unsigned char *)&digest[0],
                        QCEDEV_MAX_SHA_DIGEST);
            print_text("SHA Digest w/5 entries\n",
                        (unsigned char *)&req.digest[0],
                        QCEDEV_MAX_SHA_DIGEST);
        }
    }

    /*********************************************************************/
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr = &test_128K[0];
    req.data[0].len = _32K * 4 ;
    req.data_len = _32K * 4;
    req.diglen = 0;
    req.entries = 1;
    qcedev_get_SHA(&req, fd, alg);

    if (memcmp(&digest[0], &req.digest[0], QCEDEV_MAX_SHA_DIGEST) == 0)
        pass = 1;
    else {
        pass = 0;
        if (verbose) {
            printf("SHA of 128 KB buffer (in one entry) failed\n");
            print_text("SHA Digest\n", &req.digest[0],
                        QCEDEV_MAX_SHA_DIGEST);
        }
    }

    /*********************************************************************/
    req0.data[0].vaddr = &test_128K[0];
    req0.data[0].len = _16K;
    req0.data_len = _16K;
    req.diglen = 0;
    req0.entries = 1;
    req0.alg = alg;

    memset(&req0.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req0);

    for (i = 0; i < 8; i++) {
        req0.data[0].vaddr = &test_128K[_16K * i];
        req0.data[0].len = _16K;
        req0.data_len = _16K;
        req.diglen = 0;
        req0.entries = 1;
        req0.alg = alg;

        ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req0);
    }

    ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req0);

    if (memcmp(&digest[0], &req0.digest[0], QCEDEV_MAX_SHA_DIGEST) == 0)
        pass = 1;
    else {
        pass = 0;
        printf("SHA of 128 KB buffer failed (_16KB chunks)\n");
    }
    /*********************************************************************/

    if (pass == 1) {
        if (verbose || internal_test_enabled)
            printf("SHA Digest (128 KB) as expected: PASS!!!!\n");
    } else {
        if (verbose)
            printf("SHA Digest (128 KB ) comparison FAILED !!!!\n");
    }
    return pass;
}

static int qcedev_sha_adversarial_test(int fd)
{
    struct qcedev_sha_op_req  req;
    struct qcedev_sha_op_req  req0;
    int errors = 0;
    int ret;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf(" qcedev_sha_adverserial_test: fd = 0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }
    memset(&req.digest[0], 0, QCEDEV_MAX_SHA_DIGEST);
    req.data[0].vaddr =  &PlainText_90[0];
    req.data[0].len = 0x90;
    req.data_len = 0x00;
    req.diglen = 0;
    req.entries = 1;
    req.alg = QCEDEV_ALG_SHA1;

    /* Check for data-len set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req))
        errors++;

    req.data_len = 0x90;
    req.entries = 0;
    req.alg = QCEDEV_ALG_SHA1;

    /* Check for entries set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req))
        errors++;

    req.entries = 1;
    req.alg = 6;

    /* Check for alg set incorrectly to an invalid value */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req))
        errors++;

    req.alg = QCEDEV_ALG_SHA1;

    if (ioctl(fd, QCEDEV_IOCTL_SHA_INIT_REQ, &req))
        errors++;

    /**********************************************************/
    req.data_len = 0x00;

    /* Check for data-len set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req))
        errors++;

    req.data_len = 0x90;
    req.entries = 0;
    req.alg = QCEDEV_ALG_SHA1;

    /* Check for entries set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req))
        errors++;

    req.entries = 1;
    req.alg = 6;

    /* Check for alg set incorrectly to an invalid value */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req))
        errors++;

    req.alg = QCEDEV_ALG_SHA1;

    if (ioctl(fd, QCEDEV_IOCTL_SHA_UPDATE_REQ, &req))
        errors++;
    /***********************************************************/
    req.data_len = 0x00;

    /* Check for data-len set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req))
        errors++;

    req.data_len = 0x90;
    req.entries = 0;
    req.alg = QCEDEV_ALG_SHA1;

    /* Check for entries set incorrectly to 0 */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req))
        errors++;

    req.entries = 1;
    req.alg = 6;

    /* Check for alg set incorrectly to an invalid value */
    if (!ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req))
        errors++;

    req.alg = QCEDEV_ALG_SHA1;

    if (ioctl(fd, QCEDEV_IOCTL_SHA_FINAL_REQ, &req))
        errors++;
    /***********************************************************/

    return errors;
}

static int qcedev_sha_test_suite(int sha_fd)
{
    int errors = 0;

    if ((verbose == 1) && (internal_test_enabled == 1)) {
        printf("\n\n");
        printf("===================================================\n");
        printf(" SHA TEST for sha_fd = 0x%x\n", sha_fd);
        printf("===================================================\n");
    }
    if (!qcedev_SHA1_SHA256_basic_test(sha_fd))
        errors++;
    if (!qcedev_SHA_test_32KB_plus(sha_fd, QCEDEV_ALG_SHA1))
        errors++;
    if (!qcedev_SHA_test_32KB_plus(sha_fd, QCEDEV_ALG_SHA256))
        errors++;
    if (!qcedev_SHA1_SHA256_multi_updates(sha_fd))
        errors++;
    if (!qcedev_SHA1_SHA256_multiple_of_64b(sha_fd))
        errors++;
    if (errors)
        printf("NUMBER OF FAILURES IN SHA TEST SUITE: 0x%x\n", errors);
    if (verbose || internal_test_enabled)
        printf("\n=============SHA TEST COMPLETE=============\n\n\n");
    return errors;
}

static int qcedev_cipher_adversarial_test(int fd)
{
    int errors = 0;
    struct qcedev_cipher_op_req  req;

    if (verbose == 1) {
        printf("\n\n");
        printf("---------------------------------------------------\n");
        printf(" qcedev_cipher_adversarial_test: fd = 0x%x\n", fd);
        printf("---------------------------------------------------\n");
    }

    memset(req.enckey, 0, 32);
    memset(req.iv, 0, 32);
    memcpy(req.enckey, key, key_sz);
    memcpy(req.iv, iv, 16);
    req.ivlen  = 16;

    req.byteoffset  = 0;
    req.data_len = 0x90;
    req.encklen  = key_sz;

    req.alg = QCEDEV_ALG_AES;
    req.mode = QCEDEV_AES_MODE_CTR;

    req.vbuf.src[0].vaddr = &PlainText_90[0];
    req.vbuf.src[0].len = 0x90;
    req.vbuf.dst[0].vaddr = &PlainText_90[0];
    req.vbuf.dst[0].len = 0x90;

    req.use_pmem = 0;
    req.in_place_op = 1;
    req.entries = 1;

    req.op = QCEDEV_OPER_ENC;

    /* Case 1: keylength is 0 but the key is a non zero array*/
    req.encklen  = 0;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;
    req.encklen  = key_sz;

    /* Case 2: ivlen = 0  for ctr mode*/
    req.ivlen  = 0;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;
    req.ivlen  = 16;

    /* Case 3: ivlen > 0 and mode == _ECB*/
    req.mode = QCEDEV_AES_MODE_ECB;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;

    /* Case 4: byteoffset > 0 and mode ==  _ECB*/
    req.byteoffset  = 16;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;
    req.mode = QCEDEV_AES_MODE_CTR;

    /* Case 5: byteoffset > 0 and use_pmem == 1*/
    req.byteoffset  = 16;
    req.use_pmem  = 1;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;
    req.mode = QCEDEV_AES_MODE_CTR;
    req.byteoffset  = 0;

    /* Case 6:  use_pmem == 1 and  in_palce_op == 0*/
    req.in_place_op  = 0;
    if (!ioctl(fd, QCEDEV_IOCTL_ENC_REQ, &req))
        errors++;
    req.use_pmem  = 0;
    req.in_place_op  = 1;

    return errors;
}


static int qcedev_cipher_vbuf_test(int cipher_fd,
                    enum qcedev_cipher_alg_enum alg,
                    enum qcedev_cipher_mode_enum  mode,
                    uint32_t key_sz)
{
    int errors = 0;
    if (verbose == 1) {
        printf("\n\n\n");
        printf("***************************************************\n");
        printf("       CIPHER TEST CASES WITH VBUF ALLOCATED MEMORY\n");
        printf("qcedev_cipher_vbuf_test: alg=0x%x, mode=0x%x\n",
                                alg, mode);
        printf("***************************************************\n");
    }
    if (alg != QCEDEV_ALG_AES) {
        if (!(qcedev_cipher_vbuf_single_sg_test(cipher_fd, alg, mode,
                &PlainText_90[0], &PlainText_90[0], 0x10, key,
                iv, 0, 0, key_sz)))
            errors++;
    } else {
        if (mode == QCEDEV_AES_MODE_CTR) {
            /* test the byteoffset case */
            if (!(qcedev_cipher_vbuf_single_sg_test(cipher_fd, alg,
                mode, &PlainText_90[0], &PlainText_90[0],
                0x80, key, &iv[0], 0, 10, key_sz)))
                errors++;
        } else {
            if (!(qcedev_cipher_vbuf_single_sg_test(cipher_fd, alg,
                mode, &PlainText_90[0], &PlainText_90[0],
                0x80, key, &iv[0], 0, 0, key_sz)))
                errors++;
        }
    }
    if (!(qcedev_cipher_vbuf_mult_sg_test(cipher_fd, alg,
                        mode, key, &iv[0], 0, key_sz)))
            errors++;
    if (!(qcedev_cipher_vbuf_mult_sg_large_buf_test(cipher_fd, alg,
                        mode, key, &iv[0], 0, key_sz)))
            errors++;
    return errors;
}


static int qcedev_cipher_vbuf_test_suite(int cipher_fd)
{
    int errors = 0;
    if (verbose == 1) {
        printf("\n\n");
        printf("==================================================\n");
        printf(" CIPHER TEST for  cipher_fd = 0x%x\n", cipher_fd);
        printf("==================================================\n");
    }

    if (verbose == 1)
        printf("\n AES-128 (USE VBUF)\n");
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_CBC, KEY_SIZE_16);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_ECB, KEY_SIZE_16);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_CTR, KEY_SIZE_16);

    if (verbose == 1)
        printf("\n AES-256 (USE VBUF)\n");
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_CBC, KEY_SIZE_32);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_ECB, KEY_SIZE_32);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_CTR, KEY_SIZE_32);

#ifdef CRYPTO_AES_XTS_SUPPORT
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_XTS, 2*KEY_SIZE_32);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_AES,
                    QCEDEV_AES_MODE_XTS, KEY_SIZE_32);
#endif

    if (verbose == 1)
        printf("\n DES (USE VBUF)\n");
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_DES,
                    QCEDEV_DES_MODE_CBC, KEY_SIZE_16);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_DES,
                    QCEDEV_DES_MODE_ECB, KEY_SIZE_16);

    if (verbose == 1)
        printf("\n 3DES (USE VBUF)\n");
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_3DES,
                    QCEDEV_DES_MODE_CBC, KEY_SIZE_32);
    errors += qcedev_cipher_vbuf_test(cipher_fd, QCEDEV_ALG_3DES,
                    QCEDEV_DES_MODE_ECB, KEY_SIZE_32);

    if (errors)
        printf("NUMBER OF FAILURES IN VBUF CIPHER TEST SUITE: 0x%x\n",
                                errors);
    if ((verbose == 1) || internal_test_enabled)
        printf("\n==============CIPHER TEST COMPLETE==============\n");
    return errors;
}

static int qcedev_cipher_test_suite(int cipher_fd)
{
    int errors = 0;
    if (verbose == 1) {
        printf("\n\n");
        printf("==================================================\n");
        printf(" CIPHER TEST for  cipher_fd = 0x%x\n", cipher_fd);
        printf("==================================================\n");
    }


    errors += qcedev_cipher_vbuf_test_suite(cipher_fd);

    if (errors)
        printf("NUMBER OF FAILURES IN CIPHER TEST SUITE: 0x%x\n",
                                errors);
    if (verbose || internal_test_enabled)
        printf("\n==============CIPHER TEST COMPLETE==============\n");
    return errors;
}

static int qcedev_test_suite(int qcedev_fd)
{
    int errors = 0;

    if (verbose == 1) {
        printf("\n\n\n\n\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                                 X\n");
        printf("X      QCEDEV TEST SUITE: qcedev_fd = 0x%x        X\n",
                                qcedev_fd);
        printf("X                                                 X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    }
    errors = qcedev_sha_test_suite(qcedev_fd);

    errors += qcedev_cipher_test_suite(qcedev_fd);

    if (verbose == 1) {
        if (errors) {
            printf("\n\n");
            printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
            printf("X                                         X\n");
            printf("X        QCEDEV TEST SUITE FAILED      X\n");
            printf("X                                         X\n");
            printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
            printf("NUMBER OF FAILURES IN TEST SUITE: 0x%x\n",
                errors);
        } else {
            printf("\n\n");
            printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
            printf("X                                         X\n");
            printf("X QCEDEV TEST SUITE PASSED ALL TEST CASES X\n");
            printf("X                                         X\n");
            printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        }
    }

    return 1;
}

static int nominal(void)
{
    int errors = 0;
    errors = qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA1);
    errors += qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA256);
    errors += qcedev_cipher_basic_test(qcedev_fd);
#ifdef CRYPTO_HMAC_SUPPORT
    errors += qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA1_HMAC);
    errors += qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA256_HMAC);
#endif
    errors += qcedev_CMAC_basic_test(qcedev_fd);
    return errors;
}

static int nominal_test(void)
{
    int errors;

    printf("\n\nRunning Nominal Test......\n");

    errors = nominal();

    printf("\n\n");
    if (errors) {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X    QCEDEV FAILED NOMINAL TEST CASES     X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("Number of failures in %s test: 0x%x\n",
                    testopts[NOMINAL].name, errors);
    } else {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X   QCEDEV PASSED NOMINAL TEST CASE(S)      X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    }
    return errors;
}

static int adverse(void)
{
    int sha_errors = 0;
    int cipher_errors = 0;

    sha_errors = qcedev_sha_adversarial_test(qcedev_fd);
    if ((sha_errors) && (verbose == 1))
        printf("SHA ADVERSARIAL TESTS ERRORS = 0x%x\n",
                    (uint32_t)sha_errors);

    cipher_errors += qcedev_cipher_adversarial_test(qcedev_fd);
    if ((cipher_errors) && (verbose == 1))
        printf("CIPHER ADVERSARIAL TESTS ERRORS = 0x%x\n",
                        (uint32_t)cipher_errors);

    return cipher_errors + sha_errors;
}

static int adversarial_test(void)
{
    int errors;

    printf("\n\nRunning Adversarial Test......\n");

    errors = adverse();
    printf("\n\n");
    if (errors) {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X   QCEDEV FAILED ADVERSE TEST CASE(S)      X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("Number of failures in %s test: 0x%x\n",
                testopts[ADVERSARIAL].name, errors);
    } else {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X   QCEDEV PASSED ADVERSE TEST CASE(S)      X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    }
    return errors;
}

static int repeat_test(void)
{
    int errors = 0;
    int i;

    printf("\n\nRunning Repeatabilty Test......\n");
    for (i = 0; i < 100; i++) {
        errors += nominal();
        errors += adverse();
    }
    printf("\n\n");
    if (errors) {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X  QCEDEV FAILED REPEAT TEST CASE(S)      X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("Number of failures in %s test: 0x%x\n",
                    testopts[REPEAT].name, errors);
    } else {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X    QCEDEV PASSED REPEAT TEST CASE(S)    X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    }
    return errors;
}

static int qcedev_sha1_pthread_test(int fd)
{
    struct qcedev_sha_op_req  *req;
    int errors = 0;
    unsigned int i, num_tv;
    if (verbose == 1) {
        printf("\n\n");
        printf("----------------------------------------------\n");
        printf(" sha1_basic_test: fd = 0x%x\n", fd);
        printf("----------------------------------------------\n");
    }
    req = malloc(sizeof(struct qcedev_sha_op_req));
    if (!req) {
        printf("Failed to allocate memory for req buffer\n");
        return -1;
    }

    num_tv = (sizeof(sha1_tv))/(sizeof(struct test_vector));
    for (i = 0; i < num_tv; i++) {

        req->alg = QCEDEV_ALG_SHA1;
        req->data[0].vaddr = (uint8_t *)sha1_tv[i].input;
        req->data[0].len = sha1_tv[i].ilen;
        req->data_len = sha1_tv[i].ilen;
        req->entries = 1;

        memset(&req->digest[0], 0, QCEDEV_MAX_SHA_DIGEST);

        ioctl(fd, QCEDEV_IOCTL_GET_SHA_REQ, req);
        if (!(memcmp(&req->digest[0], sha1_tv[i].digest,
                    sha1_tv[i].diglen) == 0)) {
            errors += 1;
            if (verbose) {
                printf("SHA1 FAILED\n");
                print_text("SHA1 input data",
                        (unsigned char *)
                        sha1_tv[i].input,
                        sha1_tv[i].ilen);
                print_text("Calculated SHA1 Digest",
                        (unsigned char *)
                        &req->digest[0],
                        sha1_tv[i].diglen);
                print_text("Expected SHA1 Digest",
                        (unsigned char *)
                        sha1_tv[i].digest,
                        sha1_tv[i].diglen);
            }
        }
    }

    if (verbose) {
        if (errors  == 0)
            printf("SHA1 Basic Test: PASS !!!\n");
        else
            printf("SHA1 BASIC TEST: FAILED %d TESTS"
                "OUT OF %d\n", errors, num_tv);
    }
    free(req);
    return errors;
}

static int basic_thread_test(int fd, int thread, uint32_t *iteration)
{
    int errors = 0;
    int i;

    for (i = 0; i < 500; i++)
        if (thread % 2)
            errors += qcedev_sha1_pthread_test(fd);
        else
            errors += qcedev_cipher_basic_test(fd);

    if (errors) {
        printf("\nNumber of failures in thread %d stress test: 0x%x\n",
                    thread, errors);
    } else {
        printf("    Thread %d stress test: PASSED\n", thread);
    }
    *iteration = i;
    return errors;
}

static void *test_thread(void *arg)
{
    uint32_t thread_num = *(uint32_t *)arg;
    int fd;
    uint32_t iteration, errors = 0;

    printf("    Thread %d starting...\n", thread_num);
    fd = open("/dev/qce", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("Fail to Open QCEDEV Device fd (%d)\n", fd);
        pthread_exit((char *)1);
    } else {
        if (verbose == 1)
            printf("     Thread %d fd %d\n", thread_num, fd);
    }

    errors =  basic_thread_test(fd, thread_num, &iteration);
    if (errors)
        pthread_exit(&errors);

    close(fd);

    if (errors)
        printf("Thread %d failed at iteration %d\n",
                thread_num, iteration);

    pthread_exit(&errors);

    return NULL;
}

static int stress_test_thread(char *name)
{
    int i, rc, thread_errs, errors = 0;
    static pthread_t threads[8];

    if (verbose == 1) {
        printf("\n\n");
        printf("----------------------------------------------\n");
        printf(" Running Multi-Thread Stress Test...\n");
        printf("----------------------------------------------\n");
    }
    pthread_mutex_init(&plock, 0);

    for (i = 0; i < 8; i++) {
        rc = pthread_create(&threads[i], NULL, test_thread,
                            (void *) &i);
        if (rc) {
            printf("Couldn't create thread %d\n", i + 1);
            return 1;
        }
    }

    for (i = 0; i < 8; i++) {
        rc = pthread_join(threads[i], (void **)&thread_errs);
        if (rc) {
            printf("Couldn't join thread %d\n", i + 1);
            return 1;
        }
        errors += thread_errs;
    }

    return errors;
}

static int stress_test(void)
{
    int errors = 0;
    int i;

    printf("\n\nRunning Stress Test......\n");

    printf("  Running Single Thread Stress Test...\n");
    for (i = 1; i < 501; i++) {
        errors += qcedev_sha_test_suite(qcedev_fd);
        errors += qcedev_cipher_vbuf_test_suite(qcedev_fd);
        if (!(i % 50))
            printf("\n  *   Completed %d runs...\n", i);
    }
    printf("\n\n");
    if (errors) {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X  QCEDEV FAILED STRESS TEST          X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("Number of failures in %s test: 0x%x\n",
                    testopts[STRESS].name, errors);
    } else {
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
        printf("X                                         X\n");
        printf("X    QCEDEV PASSED STRESS TEST            X\n");
        printf("X                                         X\n");
        printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    }
    return errors;
}

static int performance_test(void)
{
    int i;

    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    printf("X                                         X\n");
    printf("X  QCEDEV PERFORMANCE TEST RESULTS      X\n");
    printf("X                                         X\n");
    printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
    fp = fopen("test.txt", "w+");
    if (!fp) {
        printf("Could not open test.txt file, errno: %d\n", errno);
        return -errno;
    }
    printf("\n\nRunning Performance Test......\n");
    printf("\nCipher Performance Test");
    printf("\n\talg= AES, mode = CBC, key size = 128bit\n");
    /* Performance test is run with the following parameters
     * packet sizes = 8K, 16K, 32K, 64K
     * alg = aes
     * mode = cbc
     * key size = 128 bit
    */
    for (i = 8; i < MAX_PACKET_DATA_KB; i *= 2)
        qcedev_cipher_vbuf_speedtest(qcedev_fd, 1024 * i,
                        QCEDEV_ALG_AES,
                        QCEDEV_AES_MODE_CBC,
                        16);

    /* Performance test is run with the following parameters
     * packet sizes = 8K, 16K, 32K, 64K.
     * alg = SHA256.
     * iuf = false.
    */
    printf("\nSHA Performance Test");
    printf("\n\talg= SHA256, DIGEST\n");
    for (i = 8; i < MAX_PACKET_DATA_KB; i *= 2)
        qcedev_sha_vbuf_speedtest(qcedev_fd, 1024 * i,
                        QCEDEV_ALG_SHA256, 0);
    fclose(fp);
    return 0;
}

void internal_test_menu(void)
{
    printf("\n\n");
    printf("The currently implemented tests are:\n");
    printf("\n");
    printf("    Input    1    TEST CASE\n");
    printf("   -----------    ----------\n");
    printf("    0 ------->  Dispaly Menu\n");
    printf("    1 ------->  SHA1/SHA256 Test Suite\n");
    printf("    2 ------->  CIPHER Test Suite\n");
    printf("    3 ------->  Basic SHA1 Test\n");
    printf("    4 ------->  Basic SHA256 Test\n");
    printf("    5 ------->  Basic CMAC Test\n");
    printf("    6 ------->  Basic Cipher AES test\n");
    printf("    7 ------->  Basic Cipher DES test\n");
    printf("    8 ------->  SHA1/SHA256 block size multiple pkt\n");
    printf("    9 ------->  SHA1/SHA256 32 KB Plus data\n");
    printf("   10 ------->  CIPHER (PMEM) Test Suite\n");
    printf("   11 ------->  CIPHER (VBUF) Test Suite\n");
    printf("   12 ------->  Cipher using VBUF (PERFORMANCE)\n");
    printf("   13 ------->  Cipher using PMEM (PERFORMANCE)\n");
    printf("   14 ------->  SHA using VBUF (PERFORMANCE)\n");
    printf("   15 ------->  Multi Thread Stress Test\n");
    printf("   16 ------->  hmac-sha1 basic test\n");
    printf("   17 ------->  hmac-sha256 basic test\n");

    printf("\n  Input   2    CIPHER/SHA ALGORITHM\n");
    printf("   -----------    -----------------\n");
    printf("    0 ------->  QCEDEV_ALG_DES/SHA1\n");
    printf("    1 ------->  QCEDEV_ALG_3DES/SHA256\n");
    printf("    2 ------->  QCEDEV_ALG_AES\n\n");

    printf("\n  Input   3    AES/DES MODE\n");
    printf("   -----------    -----------------\n");
    printf("    0 ------->  QCEDEV_AES_MODE_CBC (AES/DES/DES3)\n");
    printf("    1 ------->  QCEDEV_AES_MODE_ECB (AES/DES/DES3)\n");
    printf("    2 ------->  QCEDEV_AES_MODE_CTR\n");
    printf("    3 ------->  QCEDEV_AES_MODE_XTS\n");

    printf("\n  Input   4    KEY SIZE 16/32\n");
    printf("   -----------    -----------------\n");
    printf("    16 ------->   128 bit\n");
    printf("    32 ------->   256 bit\n");
    printf("    64 ------->   512 bit\n");

    printf("\n  Input   5    SHA1/SHA256 MODE\n");
    printf("   -----------    -----------------\n");
    printf("    0 ------->   using SHA Digest\n");
    printf("    1 ------->   using init/update/final\n");


    printf("    Test Cases  Input1 Input2 Input3 Input4 Input5\n");
    printf("    ----------  ------ ------ ------ ------ -------\n");
    printf("     0 - 8        x      -       -      -    -\n");
    printf("       9          x      x       -      -    -\n");
    printf("     10 - 13      x      x       x      x    -\n");
    printf("      14          x      x       -      x    x\n");
    printf("      15          x      -       -      -    -\n");
}

int internal_test(int argc, char *argv[])
{
    int test_number;
    int i = 0;
    int iuf = 0;
    enum qcedev_cipher_alg_enum alg = 0;
    enum qcedev_cipher_alg_enum sha_alg = 0;
    enum qcedev_cipher_mode_enum mode = 0;


    if (!argv[0]) {
        internal_test_menu();
        return 0;
    }
    fp = fopen("test.txt", "w+");
    if (!fp) {
        printf("Could not open test.txt file\n");
        return -errno;
    }

    printf("test---number:%s", argv[0]);
    test_number = atoi(argv[0]);

    /*All internal tests > 9 and < 14 need algo and mode options*/
    if (test_number == 9) {
        alg = atoi(argv[1]);
    } else    if ((test_number > 9) && (test_number < 13)) {
        alg = atoi(argv[1]);
        mode = atoi(argv[2]);
        key_sz = atoi(argv[3]);
        if (!((key_sz == KEY_SIZE_16) || (key_sz == KEY_SIZE_32) ||
            (key_sz == KEY_SIZE_64))) {
                printf("Wrong key size\n");
                exit(-1);
        }
    } else if (test_number == 14) {
        sha_alg = atoi(argv[1]);
        iuf = atoi(argv[2]);
    }

    if (verbose)
        printf("Running test %d\n", test_number);

    switch (test_number) {
    case 0:
        internal_test_menu();
        break;

    case 1:
        qcedev_sha_test_suite(qcedev_fd);
        break;

    case 2:
        qcedev_cipher_test_suite(qcedev_fd);
        break;

    case 3:
        qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA1);
        break;

    case 4:
        qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA256);
        break;
    case 5:
        qcedev_CMAC_basic_test(qcedev_fd);
        break;
    case 6:
        qcedev_cipher_aes_basic_test(qcedev_fd);
        break;

    case 7:
        qcedev_cipher_des_basic_test(qcedev_fd);
        break;

    case 8:
        qcedev_SHA1_SHA256_multiple_of_64b(qcedev_fd);
        break;

    case 9:
        qcedev_SHA_test_32KB_plus(qcedev_fd, alg);
        break;

    case 10:
        printf("PMEM is not enabled on this device\n");
        break;

    case 11:
        qcedev_cipher_vbuf_test(qcedev_fd, alg, mode, key_sz);
        break;

    case 12:
        for (i = 1; i < MAX_PACKET_DATA_KB; i *= 2)
            qcedev_cipher_vbuf_speedtest(qcedev_fd, 1024 * i, alg,
                            mode, key_sz);
        break;

    case 13:
        printf("PMEM is not enabled on this device\n");
        break;

    case 14:
        for (i = 1; i < MAX_PACKET_DATA_KB; i *= 2)
            qcedev_sha_vbuf_speedtest(qcedev_fd, 1024 * i, sha_alg,
                            iuf);
        break;

    case 15:
        stress_test_thread("MultiThread");
        break;

    case 16:
#ifdef CRYPTO_HMAC_SUPPORT
        qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA1_HMAC);
#else
        printf("HMAC not supported on this target\n");
#endif
        break;

    case 17:
#ifdef CRYPTO_HMAC_SUPPORT
        qcedev_sha_basic_test(qcedev_fd, QCEDEV_ALG_SHA256_HMAC);
#else
        printf("HMAC not supported on this target\n");
#endif
        break;

    default:
        internal_test_menu();
        break;
    }

    close(qcedev_fd);
    fclose(fp);
    exit(0);
}

static int (*test_func[]) (void) = {
    [NOMINAL] = nominal_test,
    [ADVERSARIAL] = adversarial_test,
    [STRESS] = stress_test,
    [REPEAT] = repeat_test,
    [PERFORMANCE] = performance_test,
};

static void usage(int ret)
{
    printf("\n\n---------------------------------------------------------\n"
        "Usage: qcedev_test -[OPTION] -[TEST_TYPE0]..-[TEST_TYPEn]\n\n"
        "Runs the user space tests specified by the TEST_TYPE\n"
        "parameter(s).\n"
        "\n\n"
        "OPTION can be:\n"
        "  -v, --verbose         run with debug messages on\n\n"
        "TEST_TYPE can be:\n"
        "  -i, --internal        run internal functionality tests\n"
        "  -n, --nominal         run standard functionality tests\n"
        "  -a, --adversarial     run tests that try to break the\n"
        "                          driver\n"
        "  -r, --repeatability   run 200 iterations of both the\n"
        "                          nominal and adversarial tests\n"
        "  -s, --stress          run tests that try to maximize the\n"
        "                          capacity of the driver\n"
        "  -p, --performance     run cipher and sha256 performance\n"
        "               tests\n"
        "  -h, --help            print this help message and exit\n\n\n"
        "   do 'chmod 777 qcedev_test'\n"
        " - Run qcedev_test:\n"
        "      do './qcedev_test -<OPTION> -<TEST_TYPE0> -<TEST_TYPE1>"
        " ......-<TEST_TYPEn>'\n"
        "         [see OPTION & TEST_TYPE described above]\n"
        "---------------------------------------------------------\n");

    exit(ret);
}

static unsigned int comand_argc = 0;
static char *comand_argv[16];

static int parse_comand_arg(char *const argv[], char *save_argv[])
{
    int i = 0;

    while (argv[i] != NULL && argv[i][0] != '-') {
        //strcpy(save_argv[i], argv[i]);
        save_argv[i] = argv[i];
        printf("cmd:%s", save_argv[i]);
        ++i;
    }
    
    return i;
}


static unsigned int parse_command(int argc, char *const argv[])
{
    int command = 0;
    unsigned int ret = 0;
    int option_index;

    verbose = 0;
    bringup = 0;

    while ((command = getopt_long(argc, argv, "bvnasrpi:h", testopts,
                      NULL)) != -1) {
        
        printf("next optind is %d\n", optind);
        switch (command) {
        case 'b':
            bringup = 1;
            verbose = 1;
            break;
        case 'v':
            verbose = 1;
            break;
        case 'n':
            ret |= 1 << NOMINAL;
            break;
        case 'a':
            ret |= 1 << ADVERSARIAL;
            break;
        case 'r':
            ret |= 1 << REPEAT;
            break;
        case 's':
            ret |= 1 << STRESS;
            break;
        case 'p':
            ret |= 1 << PERFORMANCE;
            break;
        case 'i':
            ret = 1 << INTERNAL;
            comand_argc = parse_comand_arg(&argv[optind - 1], comand_argv);
            break;
        case 'h':
            usage(0);
        default:
            usage(-1);
        }
    }

    /* When only verbose option is given*/
    if (ret == 0)
        usage(-1);
    return ret;
}

int main(int argc, char **argv)
{
    int rc = 0, num_tests_failed = 0, i;
    internal_test_enabled = 0;
    int error = -1;
    unsigned int test_mask = parse_command(argc, argv);

    qcedev_fd = open("/dev/qce", O_RDWR | O_SYNC);

    if (qcedev_fd < 0) {
        printf("Fail to Open QCEDEV Device (dev/qce) qcedev_fd (%d)\n",
                                qcedev_fd);
        printf("Test failed\n");
        return error;
    }

    if (verbose)
        printf("QCEDEV Device Opened SUCCESSFUL qcedev_fd (%d)\n",
                                qcedev_fd);

    /* Condition to check if it is a internal test or ABAIT test */
    if ((test_mask & (1 << INTERNAL)) == (1U << INTERNAL)) {
        internal_test(comand_argc , comand_argv);
    } else {
        for (i = 0; i < LAST_TEST; i++) {
            /* Look for the test that was selected */
            if (!(test_mask & (1U << i)))
                continue;

            /* This test was selected, so run it */
            rc = test_func[i]();

            if (rc) {
                printf("%s test case FAILED! rc:%d\n",
                    testopts[i].name, rc);
                num_tests_failed++;
            }
        }
    }

    if (num_tests_failed)
        printf("NUMBER OF TESTS FAILED: %d\n\r", num_tests_failed);

    close(qcedev_fd);

    return -num_tests_failed;
}
