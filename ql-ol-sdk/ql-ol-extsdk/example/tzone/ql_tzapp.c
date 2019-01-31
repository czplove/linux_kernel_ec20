/* 
 * Copyright (c) 2017-2018, The Quectel Wireless Solutions Co., Ltd.
 *
 * Authors
 *  carl.yin,       carl.yin@quectel.com
 *  running.qian    running.qian@quectel.com
 *
 * Fixs:
 *  2017-07-14      carl        initial
 *  2017-11-14      running     redisgned
 */

#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

#include "ql_tzone.h" //<linux/quectel/ql_tzone.h>

typedef struct {
    uint32  modulus_size;
    uint8   modulus[2048];
    uint32  exponent_size;
    uint8   exponent[64];
} ql_public_key_export_t;

//openssl generate next modules ,private Expoent, and private Exponent
static uint8 g_modulus[] = {
        0x00,0xd5,0x6e,0x89,0x6a,0xdb,0x76,0xe5,0xe4,0xf6,0x2d,0xdd,0xa3,0xaf,0x1b,0x15,
        0xfe,0x18,0xff,0x58,0xb4,0x14,0xc2,0xdd,0x05,0xe6,0xa3,0x74,0x51,0xee,0x46,
        0x1e,0xaf,0xcf,0x0b,0x16,0x02,0xbb,0xa5,0x14,0xcf,0x89,0x6d,0x01,0xd1,0x07,
        0x06,0x72,0x53,0x62,0x89,0xf8,0x63,0x85,0xa7,0xe9,0xfe,0xfa,0x64,0xe6,0xc1,
        0x10,0x03,0xeb,0xa8,0xf2,0xf3,0xc2,0xde,0xa3,0x9e,0x2c,0xae,0x84,0x74,0x3b,
        0x02,0x31,0xb8,0x99,0x28,0x3e,0x46,0x91,0xc7,0x15,0xc8,0x30,0xab,0x63,0x26,
        0x30,0xc6,0x76,0x77,0x22,0xc3,0x43,0xc3,0x45,0xbf,0xb7,0x28,0xa4,0x2b,0x63,
        0xb4,0xa2,0x4e,0x06,0xcf,0xfc,0x0b,0x34,0x12,0xc9,0xbd,0x0d,0x3f,0x48,0x44,
        0x61,0xb6,0xc7,0x17,0x52,0x66,0xcf,0xa7,0x86,0x92,0xe1,0xf0,0x4c,0x94,0x77,
        0xa8,0x61,0x40,0x00,0x9d,0x9c,0xd7,0x99,0xf1,0xdb,0x43,0x1a,0xaf,0x39,0xc6,
        0x3e,0x93,0xdc,0x19,0x28,0x38,0x91,0x86,0x85,0x75,0x99,0xd3,0xdc,0x9d,0xef,
        0x57,0x68,0x29,0x03,0xde,0x60,0x36,0xc9,0x0a,0x36,0xbd,0x8f,0x4f,0x90,0x21,
        0x91,0x8b,0x8f,0x38,0xee,0xbe,0xba,0x18,0x19,0xf4,0x21,0xc2,0x8d,0x46,0x27,
        0x18,0x75,0x13,0x44,0x95,0x75,0x1b,0x8d,0x78,0xb6,0x01,0x80,0x45,0xd5,0x50,
        0x0c,0x59,0xd8,0xf0,0x6d,0x65,0xaa,0xe2,0x36,0x59,0x8c,0x64,0x37,0x92,0xdb,
        0x65,0x8c,0x77,0x8a,0x6b,0x43,0x62,0x72,0x73,0x82,0x88,0xa2,0xe2,0xfb,0x5f,
        0xae,0xda,0x3d,0x1e,0x68,0x13,0x1a,0x8d,0xa2,0x36,0x8f,0x11,0x4b,0xb5,0xfb,0x79
    };
        
static uint8 g_publicExponent[] = {0x00, 0x01, 0x00, 0x01};//65537

static uint8 g_privateExponent[] = {
        0x4d,0x2b,0x7a,0x64,0x38,0x95,0xf4,0xe9,0xa4,0x59,0x51,0x1a,0x2d,0xe6,0x0d,0x3d,
        0xa2,0xac,0x30,0xd6,0x6b,0xeb,0x49,0x1f,0x29,0x15,0x50,0xa9,0x8f,0x0d,0xab,
        0x6a,0xc5,0xe1,0xac,0x43,0xb0,0xb5,0x2f,0xa1,0x1a,0x7f,0x5f,0x78,0xc4,0xa1,
        0x80,0x14,0xc6,0x3c,0x3c,0xa3,0x3f,0x60,0x47,0x2a,0xfa,0x3e,0x21,0x71,0x0c,
        0xe9,0x5d,0xfb,0xe8,0x5c,0x5a,0xc9,0x45,0x3f,0x9b,0xc8,0x3d,0xfc,0x08,0x99,
        0xf2,0x80,0x5c,0x60,0x40,0xa4,0x23,0x71,0x68,0x5e,0xc9,0xba,0x2e,0x4f,0x50,
        0xb3,0x71,0x82,0x01,0xff,0xb8,0x30,0x92,0xb7,0x8e,0xf2,0x12,0xe9,0xdd,0x53,
        0x22,0x9c,0x33,0xba,0x5f,0xd7,0x6d,0x2c,0x9d,0xe4,0xcd,0x35,0x64,0x5b,0xd5,
        0x38,0xd1,0x23,0x8d,0x97,0xbe,0xdc,0x9a,0x7a,0xe2,0xe7,0x90,0x53,0x2c,0x17,
        0x91,0x43,0x6d,0xf7,0x53,0x2d,0x2c,0x40,0x68,0x77,0x3e,0x19,0x53,0xf9,0x2b,
        0x03,0xa3,0x20,0x6e,0x35,0x30,0xb6,0xd5,0x40,0x86,0x99,0x81,0x45,0x9a,0xdc,
        0xe1,0x70,0x07,0x81,0xdc,0xdf,0xcb,0x13,0x1e,0xf6,0xbb,0x8d,0x08,0x4c,0xea,
        0x02,0x1f,0xcf,0x5b,0x8b,0x0e,0x50,0x0f,0xe5,0x0a,0x06,0x32,0xb1,0x51,0x80,
        0x7b,0x25,0x44,0x4c,0x72,0xba,0x0d,0x19,0x03,0x0f,0x97,0x86,0x75,0x67,0x22,
        0xe3,0x9d,0x2b,0x5b,0xef,0x25,0xf0,0x8d,0x79,0x9d,0x32,0x95,0x6b,0x6c,0xf0,
        0x19,0x54,0xd2,0xed,0xf9,0x14,0x2a,0xb9,0x26,0x94,0xbe,0x89,0x1e,0x3b,0x30,
        0xb9,0x94,0xb7,0x2a,0xf1,0x6b,0x5c,0xc7,0xe1,0x32,0x09,0x3f,0xab,0xfd,0x01
    };


static int ql_tzapp_write_file(const char *filename, void *pbuf, uint32 size)
{
    FILE *fp = fopen(filename, "wb");
    size_t ret = 0;

    if (fp) {
        ret = fwrite(pbuf, 1, size, fp);
        fsync(fileno(fp));
        fclose(fp);
    }

    return (ret == size);
}

static int ql_tzapp_read_file(const char *filename, void **ppbuf, uint32 *psize)
{
    FILE *fp = fopen(filename, "rb");
    size_t ret = 0;

    *ppbuf = NULL;
    *psize = 0;
    
    if (fp) {
        fseek(fp, 0, SEEK_END);
        ret = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        printf("file size %d\n\r", ret);
        if (ret) {
            *ppbuf = malloc(ret);
            if (*ppbuf) {
                *psize = fread(*ppbuf, 1, ret, fp);
            }
        }
        
        fclose(fp);
    }

    return (ret == *psize);
}


static int ql_tzapp_aes_gen_key(int tz_fd, const char *keyfile)
{
    int ret = -1;
    ql_tz_aes_generate_key_t keyblob = {256, NULL};

    printf("%s(%s)\n", __func__, keyfile);

    keyblob.key_ptr = malloc(keyblob.key_size);

    if (keyblob.key_ptr) {
        ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_AES_KEY_GEN, &keyblob);

        printf("TZ_AES_GEN_KEY ret = %d, key_ptr = %p, key_size = %d\n",
                ret, keyblob.key_ptr, keyblob.key_size);
        if (!ret && keyblob.key_size) {
            ql_tzapp_write_file(keyfile, keyblob.key_ptr, keyblob.key_size);
        }

        free(keyblob.key_ptr);
    }

    return ret;    
}


static int ql_tzapp_aes_import_key(int tz_fd, const char *keyfile)
{
    int ret = -1;
    ql_tz_aes_import_key_t ikeyblob;

    uint8 g_AESKey[] = {
        0x00,0xd5,0x6e,0x89,0x6a,0xdb,0x76,0xe5,
        0xe4,0xf6,0x2d,0xdd,0xa3,0xaf,0x1b,0x15,
        0x00,0xd5,0x6e,0x89,0x6a,0xdb,0x76,0xe5,
        0xe4,0xf6,0x2d,0xdd,0xa3,0xaf,0x1b,0x15
    };//any data 

    printf("%s(%s)\n", __func__, keyfile);

    ikeyblob.key_size = 56;//>=56
    ikeyblob.key_ptr = malloc(ikeyblob.key_size);
    ikeyblob.input_aes_size= sizeof(g_AESKey);
    ikeyblob.input_aes_ptr = g_AESKey;

    if (ikeyblob.key_ptr) {
        ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_AES_KEY_IMPORT, &ikeyblob);

        printf("TZ_AES_IMPORT_KEY ret = %d, key_ptr = %p, key_size = %d\n",
                ret, ikeyblob.key_ptr, ikeyblob.key_size);

        if (!ret && ikeyblob.key_size) {
            ql_tzapp_write_file(keyfile, ikeyblob.key_ptr, ikeyblob.key_size);
        }
    }

    free(ikeyblob.key_ptr);
    return ret;    
}


static int ql_tzapp_aes_encrypt_data(int tz_fd, const char *keyfile,
    const char *infile, const char *outfile)
{
    int     ret;
    uint32  input_data_size;
    void    *input_data_ptr;
    uint32  output_data_size;
    uint8   *output_data_ptr;
    ql_tz_aes_generate_key_t keyblob = {256, NULL};
    ql_tz_aes_endecrypt_data_t enc_data;

    printf("%s(%s, %s, %s)\n", __func__, keyfile, infile, outfile);

    ql_tzapp_read_file(keyfile, &keyblob.key_ptr, &keyblob.key_size);
    ql_tzapp_read_file(infile, &input_data_ptr, &input_data_size);

    if (input_data_size > 2048) {
        input_data_size = 2048;
    }
    output_data_size = input_data_size+24;
    output_data_ptr = malloc(output_data_size);

    enc_data.key_size = keyblob.key_size;
    enc_data.key_ptr = keyblob.key_ptr;
    enc_data.input_data_size = input_data_size;
    enc_data.input_data_ptr = input_data_ptr;
    enc_data.output_data_size = output_data_size;
    enc_data.output_data_ptr = output_data_ptr;
    printf("TZ ENC DATA key_size = %d, input data size = %d\n",
            keyblob.key_size, input_data_size);
    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_AES_DATA_ENCRYPT, &enc_data);

    printf("TZ_AES_ENCRYPT_DATA ret = %d, output_data_size = %d\n",
        ret, enc_data.output_data_size);

    if (!ret && enc_data.output_data_size) {
        ql_tzapp_write_file(outfile, enc_data.output_data_ptr,
                            enc_data.output_data_size);
    }

    free(enc_data.key_ptr);
    free(enc_data.input_data_ptr);
    free(enc_data.output_data_ptr);
    return 0;
}

static int ql_tzapp_aes_decrypt_data(int tz_fd, const char *keyfile,
    const char *infile, const char *outfile)
{
    int     ret = -1;
    uint32  input_data_size;
    void    *input_data_ptr;
    uint32  output_data_size;
    uint8   *output_data_ptr;
    ql_tz_aes_generate_key_t keyblob = {256, NULL};
    ql_tz_aes_endecrypt_data_t dec_data;

    printf("%s(%s, %s, %s)\n", __func__, keyfile, infile, outfile);

    ql_tzapp_read_file(keyfile, &keyblob.key_ptr, &keyblob.key_size);
    ql_tzapp_read_file(infile, &input_data_ptr, &input_data_size);
    if (input_data_size > (2048+24))
        input_data_size = (2048+24);

    output_data_size = input_data_size + 24;
    output_data_ptr = malloc(output_data_size);

    dec_data.key_size = keyblob.key_size;
    dec_data.key_ptr = keyblob.key_ptr;
    dec_data.input_data_size = input_data_size;
    dec_data.input_data_ptr = input_data_ptr;
    dec_data.output_data_size = output_data_size;
    dec_data.output_data_ptr = output_data_ptr;
    
    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_AES_DATA_DECRYPT, &dec_data);

    printf("TZ_AES_DECRYPT_DATA ret = %d, output_data_size = %d\n",
        ret, dec_data.output_data_size);

    if (!ret && dec_data.output_data_size) {
        ql_tzapp_write_file(outfile, dec_data.output_data_ptr,
            dec_data.output_data_size);
    }
    
    free(dec_data.key_ptr);
    free(dec_data.input_data_ptr);
    free(dec_data.output_data_ptr);

    return 0;
}


static int ql_tzapp_rsa_generate_key(int tz_fd, const char *keyfile)
{
    int ret = -1;
    ql_tz_rsa_generate_key_t rsa_key_blob_req;

    printf("%s(%s)\n", __func__, keyfile);

    rsa_key_blob_req.modulus_size = 2048;
    rsa_key_blob_req.public_exponent = 0x010001;//65537
    rsa_key_blob_req.digest_pad_type = QL_TZ_RSA_PKCS115_SHA2_256;

    rsa_key_blob_req.rsa_key_blob = malloc(sizeof(ql_crypto_rsa_key_blob_t));
    if (rsa_key_blob_req.rsa_key_blob) {
        ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_KEYPAIR_GEN, &rsa_key_blob_req);

        printf("TZ_AES_GENERATE_KEY ret = %d\n", ret);

        if (!ret)
            ql_tzapp_write_file(keyfile, rsa_key_blob_req.rsa_key_blob,
                sizeof(ql_crypto_rsa_key_blob_t));
    }
    
    free(rsa_key_blob_req.rsa_key_blob);

    return ret;
}


static int ql_tzapp_rsa_sign_data(int tz_fd, const char *keyfile,
        const char *infile, const char *signaturefile)
{
    int    ret = -1;
    uint32 input_data_size;
    void   *input_data_ptr;
    uint32 signature_length;
    void   *signature_ptr;
    uint32 key_material_len;
    
    ql_tz_rsa_sign_verify_data_t rsa_sign_data;

    printf("%s(%s, %s, %s)\n", __func__, keyfile, infile, signaturefile);

    ql_tzapp_read_file(keyfile, (void *)&rsa_sign_data.rsa_key_blob, &key_material_len);
    ql_tzapp_read_file(infile, &input_data_ptr, &input_data_size);
    if (input_data_size > 2048)
        input_data_size = 2048;

    signature_length = 256;
    signature_ptr = malloc(signature_length);

    rsa_sign_data.input_data_size = input_data_size;
    rsa_sign_data.input_data_ptr = input_data_ptr;
    rsa_sign_data.signature_length = signature_length;
    rsa_sign_data.signature_ptr = signature_ptr;
    
    if (input_data_ptr == NULL || signature_ptr == NULL) {
        printf("TZ_RSZ_SIGN malloc failed\n");
        return -1;
    }

    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_DATA_SIGN, &rsa_sign_data);

    printf("TZ_RSA_SIGN ret = %d, sign size = %d\n",
            ret, rsa_sign_data.signature_length);

    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_SINDATA_VERIFY, &rsa_sign_data);

    printf("TZ_RSA_VERIFY ret = %d, key_size = %d\n", ret, rsa_sign_data.signature_length);
    
    if (!ret && rsa_sign_data.signature_length) {
        ql_tzapp_write_file(signaturefile, rsa_sign_data.signature_ptr,
                            rsa_sign_data.signature_length);
    }
    
    free(rsa_sign_data.rsa_key_blob);
    free(input_data_ptr);
    free(signature_ptr);

    return 0;
}


static int ql_tzapp_rsa_verify_sign(int tz_fd, const char *keyfile,
    const char *infile, const char *signaturefile)
{
    int     ret = -1;
    uint32  input_data_size;
    void    *input_data_ptr;
    uint32  signature_length;
    void    *signature_ptr = NULL;
    uint32  key_material_len;
    
    ql_tz_rsa_sign_verify_data_t verify_data;    

    printf("%s(%s, %s, %s)\n", __func__, keyfile, infile, signaturefile);

    ql_tzapp_read_file(keyfile, (void *)&verify_data.rsa_key_blob, &key_material_len);
    ql_tzapp_read_file(infile, &input_data_ptr, &input_data_size);
    if (input_data_size > 2048)
        input_data_size = 2048;
    ql_tzapp_read_file(signaturefile, &signature_ptr, &signature_length);

    verify_data.input_data_size = input_data_size;
    verify_data.input_data_ptr = input_data_ptr;
    verify_data.signature_length = signature_length;
    verify_data.signature_ptr = signature_ptr;

    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_SINDATA_VERIFY, &verify_data);

    printf("TZ_RSA_VERIFY ret = %d, key_size = %d\n", ret, verify_data.signature_length);

    if (!ret && verify_data.signature_length) {
        
    }

    free(verify_data.rsa_key_blob);
    free(input_data_ptr);
    free(signature_ptr);
    return 0;
}


static int ql_tzapp_rsa_import_keypair(int tz_fd, const char *outfile)
{
    int ret = -1;
    uint32 key_material_len;
    ql_tz_rsa_keypair_import_t import_keypair;
    ql_crypto_rsa_key_blob_t  tz_key_blob;    

    printf("%s(%s)\n", __func__, outfile);

    import_keypair.in_modulus = g_modulus;
    import_keypair.in_modulusSize = sizeof(g_modulus);
    import_keypair.in_pubExp = g_publicExponent;
    import_keypair.in_pubExpSize = sizeof(g_publicExponent);
    import_keypair.in_privExp = g_privateExponent;
    import_keypair.in_privExpSize = sizeof(g_privateExponent);
    import_keypair.in_padding = QL_TZ_RSA_PKCS115_SHA2_256;    
    import_keypair.rsa_key_blob = malloc(sizeof(ql_crypto_rsa_key_blob_t));

    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_KEYPAIR_IMPORT, &import_keypair);

    printf("TZ_RSA_VERIFY ret = %d, mod_size = %d, pubexp_size=%d\n",
            ret, import_keypair.rsa_key_blob->modulus_size,
            import_keypair.rsa_key_blob->public_exponent_size);

    if (!ret) {
        key_material_len = sizeof(ql_crypto_rsa_key_blob_t);
         ql_tzapp_write_file(outfile, import_keypair.rsa_key_blob, key_material_len);
    }

    free(import_keypair.rsa_key_blob);
    return 0;
}


static int ql_tzapp_rsa_export_pubkey(int tz_fd, const char *keyfile, const char *pubkeyfile)
{
    int ret = -1;
    ql_public_key_export_t pubkey;
    uint32 key_material_len;
    ql_tz_rsa_public_key_export_t export_pubkey;    

    printf("%s(%s, %s)\n", __func__, keyfile, pubkeyfile);

    ql_tzapp_read_file(keyfile, (void *)&export_pubkey.rsa_key_blob, &key_material_len);

    export_pubkey.modulus_data_size = 2048;
    export_pubkey.modulus_data_ptr = malloc(export_pubkey.modulus_data_size);
    export_pubkey.public_exponent_size = 32;
    export_pubkey.public_exponent_ptr = malloc(export_pubkey.public_exponent_size);

    ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RSA_PUBKEY_EXPORT, &export_pubkey);

    printf("TZ_RSA_VERIFY ret = %d, modulus_size = %d, public_exponent_size=%d\n",
        ret, export_pubkey.modulus_data_size, export_pubkey.public_exponent_size);

    if (!ret && export_pubkey.modulus_data_size
        && export_pubkey.public_exponent_size) {
        memset(&pubkey, 0x00, sizeof(pubkey));
        
        pubkey.modulus_size = export_pubkey.modulus_data_size;
        memcpy(pubkey.modulus, export_pubkey.modulus_data_ptr, export_pubkey.modulus_data_size);

        pubkey.exponent_size = export_pubkey.public_exponent_size;
        memcpy(pubkey.exponent, export_pubkey.public_exponent_ptr, export_pubkey.public_exponent_size);

        ql_tzapp_write_file(pubkeyfile, &pubkey, sizeof(pubkey));
    }

    free(export_pubkey.modulus_data_ptr);
    free(export_pubkey.public_exponent_ptr);
    return 0;
}


static int ql_tzapp_random_gen(int tz_fd, const char *keyfile) 
{
    int ret = -1;
    ql_tz_random_data_t random_data = {512, NULL};

    printf("%s(%s)\n", __func__, keyfile);

    random_data.data_ptr = malloc(random_data.data_size);

    if (random_data.data_ptr) {
        ret = ioctl(tz_fd, QL_TZ_CRYPTO_SVC_RANDOM_DATA_GEN, &random_data);

        printf("TZ_AES_GENERATE_KEY ret = %d, data_ptr = %p, data_size = %d\n",
            ret, random_data.data_ptr, random_data.data_size);

        if (!ret && random_data.data_size)
            ql_tzapp_write_file(keyfile, random_data.data_ptr, random_data.data_size);

        free(random_data.data_ptr);
    }

    return ret;    
}


int main(int argc, char *argv[])
{
    int ret = -1;
    int fd = open("/dev/tzone", O_RDWR);

    if (fd <= 0)
        return 0;

    printf("dev tzone fd = %d\n\r", fd);
    if (argc >= 3) {
        const char *op = argv[1];
        const char *keyfile = argv[2];
        const char *infile = argv[3];
        const char *outfile = argv[4];

        if (!strcasecmp(op, "aes_gen_key")) {
            ret = ql_tzapp_aes_gen_key(fd, keyfile);
        }
        else if (!strcasecmp(op, "aes_import_key")) {
            ret = ql_tzapp_aes_import_key(fd, keyfile);
        }
        else if (!strcasecmp(op, "aes_enc")) {
            ret = ql_tzapp_aes_encrypt_data(fd, keyfile, infile, outfile);
        }        
        else if (!strcasecmp(op, "aes_dec")) {
            ret = ql_tzapp_aes_decrypt_data(fd, keyfile, infile, outfile);
        }   
        else if (!strcasecmp(op, "rsa_gen_key")) {
            ret = ql_tzapp_rsa_generate_key(fd, keyfile);
        }        
        else if (!strcasecmp(op, "rsa_sign")) {
            ret = ql_tzapp_rsa_sign_data(fd, keyfile, infile, outfile);
        } 
        else if (!strcasecmp(op, "rsa_verify")) {
            ret = ql_tzapp_rsa_verify_sign(fd, keyfile, infile, outfile);
        }
        else if (!strcasecmp(op, "rsa_import_keypair")) {
            ret = ql_tzapp_rsa_import_keypair(fd, keyfile);
        }
        else if (!strcasecmp(op, "rsa_export_pubkey")) {
            ret = ql_tzapp_rsa_export_pubkey(fd, keyfile, infile);
        }
         else if (!strcasecmp(op, "random_gen")) {
            ret = ql_tzapp_random_gen(fd, keyfile);
        }
        else {
            printf("unknow op=%s\n", op);
            return -1;
        }
        
        close(fd);
    }
    else {
        printf("tzapp aes_gen_key key.blob\n");
        printf("tzapp aes_import_key key.blob\n");
        printf("tzapp aes_enc key.blob plain_file enc_file\n");
        printf("tzapp aes_dec key.blob enc_file dec_file\n");
        printf("tzapp rsa_gen_key key.blob\n");
        printf("tzapp rsa_sign key.blob plain_file sign_file\n");
        printf("tzapp rsa_verify key.blob plain_file sign_file\n");
        printf("tzapp rsa_import_keypair key.blob\n");
        printf("tzapp rsa_export_pubkey key.blob pubkey_file\n");
        printf("tzapp random_gen rnd.blob\n");
    }

    return ret;
}

