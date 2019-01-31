/**
 *@file     example_crypto_tz_test.c
 *@date     2017-12-27
 *@author   running.qian
 *@brief    Quecte Openssl-TZ Demo
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/engine.h>
#include "ql_openssl_tz.h"
#define QL_TZ_LOG(format, ...)      fprintf(stderr, "[QL_TZ][%d]:"format, __LINE__, ##__VA_ARGS__)

typedef struct {
    int magic;
    unsigned int tag_num;
    unsigned int tag_size;
} aes_enc_hdr_t;

typedef unsigned char uchar;


static int ql_test_gen_RSAkey(int bits, char *pem_key_name)
{
    int ret;
    
    QL_TZ_LOG("Gen Rsa Keypair...\n");
    EVP_PKEY_CTX *ctx;
    EVP_PKEY *pkey = NULL;
    ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!ctx) {
       /* Error occurred */
        QL_TZ_LOG("error1\n");
        return -1;
    }
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
       /* Error */
        QL_TZ_LOG("error2\n");
        return -1;
    }
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        /* Error */
        QL_TZ_LOG("error3\n");
        return -1;
    }

    /* Generate key */
    ret = EVP_PKEY_keygen(ctx, &pkey);// RSA_generate_key(bits, 65537, NULL, NULL);  

    /* Save Keypair */
    BIO *bio = BIO_new_file(pem_key_name, "wb");
    if (pkey != NULL) {
         ret = PEM_write_bio_RSAPrivateKey(bio, pkey->pkey.rsa, NULL, NULL, 0, NULL, NULL);
        //ret = PEM_write_bio_RSA_PUBKEY(bio, pkey->pkey.rsa);
    }
    QL_TZ_LOG("ret = %d, done\n", ret);
    BIO_free_all(bio);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(ctx);
    
    return ret;
}


static int ql_test_read_RSAkey(char *key_file)
{
    int ret, i;
    BIO *bio;
    EVP_PKEY *pkey = NULL;
    bio = BIO_new(BIO_s_file());

    BIO_read_filename(bio, key_file);
    PEM_read_bio_PrivateKey(bio, &pkey, NULL, NULL);
    if (pkey == NULL) {
        QL_TZ_LOG("read prikey error \n");
        return -1;
    }

    BIO_free_all(bio);
    EVP_PKEY_free(pkey);
    return 0;
}


static int ql_test_RsaSign(char *pkey_file, char *in_file, char *out_file)
{
    int ret, i;
    BIO *bio;
    EVP_PKEY *pkey = NULL;
    int sig_len;
    EVP_MD_CTX  md_ctx, md2_ctx;
    
    FILE *in_fp, *out_fp;
    int inlen, outlen;
    uchar inbuf[QL_TZ_RSA_DATA_MAX_SIZE];
    uchar sig_buf[128];
    
    QL_TZ_LOG("Get PEM KEY\n");
    bio = BIO_new(BIO_s_file());
    BIO_read_filename(bio, pkey_file);
    PEM_read_bio_PrivateKey(bio, &pkey, NULL, NULL);
    if (pkey == NULL) {
        QL_TZ_LOG("read prikey error \n");
        return -1;
    }
    QL_TZ_LOG("Get PEM KEY Done.\n");
    
    in_fp = fopen(in_file, "r");
    out_fp = fopen(out_file, "wb");
    
    inlen = fread(inbuf, 1, QL_TZ_RSA_DATA_MAX_SIZE, in_fp);
    if (inlen <= 0) {
        return 0;
    }
    
#if 0
    EVP_PKEY_CTX *ctx;
    /* md is a SHA-256 digest in this example. */
    char md[32] = {
                    0x49, 0x97, 0xb6, 0x94, 0xe3, 0x0c, 0xb3, 0x7f,
                    0x17, 0x56, 0xaf, 0x19, 0xe6, 0x1e, 0x6f, 0xfb,
                    0x7f, 0xb0, 0x76, 0x63, 0xab, 0x60, 0x80, 0xc0,
                    0x49, 0x2d, 0xcf, 0xc4, 0xdf, 0x48, 0x35, 0x0a
                  };
    
    uchar *sig;
    size_t mdlen = 32, siglen;
    /*
    * NB: assumes signing_key and md are set up before the next
    * step. signing_key must be an RSA private key and md must
    * point to the SHA-256 digest to be signed.
    */
    ctx = EVP_PKEY_CTX_new(pkey, NULL /* no engine */);
    if (!ctx) {
        /* Error occurred */
        QL_TZ_LOG("new evp pkey ctx error\n");
    }
    if (EVP_PKEY_sign_init(ctx) <= 0) {
        /* Error */
        QL_TZ_LOG("sign init error\n");
    }
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_PADDING) <= 0) {
        /* Error */
        QL_TZ_LOG("set rsa padding error\n");
    }
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
        /* Error */
        QL_TZ_LOG("set signatrue md failed\n");
    }
    
    /* Determine buffer length */
    //if (EVP_PKEY_sign(ctx, NULL, &siglen, md, mdlen) <= 0) {
        /* Error */
    //    QL_TZ_LOG("sign data ERROR\n");
   //}

    sig = OPENSSL_malloc(256);

    if (!sig) {
        QL_TZ_LOG("malloc sigbuf error\n");
        /* malloc failure */
    }
    QL_TZ_LOG("sign data ...\n");
    if (EVP_PKEY_sign(ctx, sig, &siglen, md, mdlen) <= 0) {
        QL_TZ_LOG("sign md error\n");
    }
#endif

    QL_TZ_LOG("Sign data...\n");
    /* Signature is siglen bytes written to buffer sig */
    EVP_SignInit(&md_ctx, EVP_sha256());
    EVP_SignUpdate(&md_ctx, inbuf, inlen);
    sig_len = sizeof(sig_buf);
    ret = EVP_SignFinal(&md_ctx, sig_buf, &sig_len,  pkey);
    
    if (ret > 0) {
        fwrite(sig_buf, 1, sig_len, out_fp);
        QL_TZ_LOG ("Ok.\n");
    }
    else {
        QL_TZ_LOG("Failed.\n");
    }

    EVP_PKEY_free (pkey);
}


static int ql_test_RsaVerify(char *pkey_file, char *in_file, char *sign_file)
{
    int ret, i;
    BIO *bio;
    EVP_PKEY    *pkey = NULL;
    EVP_MD_CTX  md_ctx, md2_ctx;
    
    FILE  *in_fp, *sign_fp;
    int   inlen, sign_len;
    uchar inbuf[QL_TZ_RSA_DATA_MAX_SIZE];
    uchar sign_buf[256];
    
    QL_TZ_LOG("Get PEM KEY\n");
    bio = BIO_new(BIO_s_file());
    BIO_read_filename(bio, pkey_file);
    PEM_read_bio_PrivateKey(bio, &pkey, NULL, NULL);
    if (pkey == NULL) {
        QL_TZ_LOG("read prikey error \n");
        return -1;
    }
    QL_TZ_LOG("Get PEM KEY Done.\n");
    
    in_fp = fopen(in_file, "r");
    sign_fp = fopen(sign_file, "r");
    
    inlen = fread(inbuf, 1, QL_TZ_RSA_DATA_MAX_SIZE, in_fp);
    if (inlen <= 0) {
        QL_TZ_LOG("Get Plain data failed\n");
        return 0;
    }
    
    sign_len = fread(sign_buf, 1, QL_TZ_RSA_DATA_MAX_SIZE, sign_fp);
    if (sign_len <= 0) {
        QL_TZ_LOG("Get Sign data failed\n");
        return 0;
    }

    QL_TZ_LOG("Verify data...\n");
    EVP_VerifyInit(&md2_ctx, EVP_sha256());
    EVP_VerifyUpdate(&md2_ctx, inbuf, inlen);
    ret = EVP_VerifyFinal(&md2_ctx, sign_buf, sign_len, pkey);
    if (ret > 0) {
        QL_TZ_LOG ("Ok.\n");
    } else {
        QL_TZ_LOG ("Failed.\n");
        //ERR_print_errors_fp(stdout);
    }
    EVP_PKEY_free (pkey);
}


void ql_test_aes_key_gen(char *key_file)
{
    FILE *out;
    int ret;
    EVP_CIPHER_CTX ctx;
    uchar tz_key[QL_TZ_AES_KEY_SIZE];

    /* Don't set key or IV right away; we want to check lengths */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, EVP_aes_256_ccm(), NULL, NULL, NULL, 0);

    /*Gen New TZ AES Encrypted KEY */
    ret = EVP_CIPHER_CTX_ctrl(&ctx, EVP_CTRL_RAND_KEY, 0, tz_key);
    QL_TZ_LOG("gen tz_key ret=%d, EKey:\n", ret);
    BIO_dump_fp(stdout, tz_key, QL_TZ_AES_KEY_SIZE);
    
    out = fopen(key_file, "wb");
    fwrite(tz_key, 1, QL_TZ_AES_KEY_SIZE, out);
    fclose(out);
}


int ql_test_aes_256_ccm_tzcrypt(char *key_file, char *in_file, char *out_file, int do_encrypt)
{
    int ret;
    int i=0;
    EVP_CIPHER_CTX ctx;
    
    /* file variables */
    uchar tz_key[QL_TZ_AES_KEY_SIZE] = "";
    FILE *in_fp, *out_fp, *key_fp;
    int inlen, outlen;
    uchar inbuf[QL_TZ_AES_DATA_MAX_SIZE], outbuf[QL_TZ_AES_DATA_MAX_SIZE + EVP_MAX_BLOCK_LENGTH];
    int file_size;
    
    /* tag variables */
    aes_enc_hdr_t enc_hdr;
    int tag_size;
    int tag_num;
    int tag_buff_size;
    unsigned char *tag_buff;


    key_fp = fopen(key_file, "r");
    inlen = fread(tz_key, 1, QL_TZ_AES_KEY_SIZE, key_fp);
    if(inlen != QL_TZ_AES_KEY_SIZE) {
        fclose(key_fp);
        return -1;
    }
    QL_TZ_LOG("tz_key:\n");
    BIO_dump_fp(stdout, tz_key, QL_TZ_AES_KEY_SIZE);

    in_fp  = fopen(in_file, "r");
    out_fp = fopen(out_file, "wb");

    if (do_encrypt) {
        fseek (in_fp, 0, SEEK_END);
        file_size = ftell(in_fp);
        fseek(in_fp, 0, SEEK_SET);
        
        tag_num = file_size / QL_TZ_AES_DATA_MAX_SIZE;
        if (file_size % QL_TZ_AES_DATA_MAX_SIZE) {
            ++tag_num;
        }
        tag_size = QL_TZ_AES_TAG_SIZE;
        tag_buff_size = tag_num * QL_TZ_AES_TAG_SIZE;
        tag_buff = (uchar *)malloc(tag_buff_size);
        QL_TZ_LOG("tag_num = %d, tag_size = %d\n", tag_num, tag_size);
        memset(tag_buff, 0x00, tag_buff_size);
        memset(&enc_hdr, 0x00, sizeof(aes_enc_hdr_t));
        
        enc_hdr.magic = 0x514C545A;//'Q''L''T''Z'
        enc_hdr.tag_num = tag_num;
        enc_hdr.tag_size = QL_TZ_AES_TAG_SIZE;
        fwrite(&enc_hdr, 1, sizeof(aes_enc_hdr_t), out_fp);
        fwrite(tag_buff, 1, tag_buff_size, out_fp);
    }
    else {
        inlen = fread(&enc_hdr, 1, sizeof(aes_enc_hdr_t), in_fp);
        if (inlen != sizeof(aes_enc_hdr_t)) {
            QL_TZ_LOG("Get ENC Header Failed\n");
            goto CIPHER_END;
        }
        
        tag_num = enc_hdr.tag_num;
        tag_size = enc_hdr.tag_size;
        tag_buff_size = tag_num * tag_size;
        QL_TZ_LOG("tag_num = %d, tag_size = %d\n", tag_num, tag_size);
        tag_buff = (uchar *)malloc(tag_buff_size);

        inlen = fread(tag_buff, 1, tag_buff_size, in_fp);
        if (inlen != tag_buff_size) {
            QL_TZ_LOG("Get ENC Tag Failed\n");
            goto CIPHER_END;
        }
    }

    /* Don't set key or IV right away; we want to check lengths */
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, EVP_aes_256_ccm(), NULL, NULL, NULL, do_encrypt);
    /* Now we can set tz_key and IV */
    EVP_CipherInit_ex(&ctx, NULL, NULL, tz_key, NULL, do_encrypt);
    
    QL_TZ_LOG("encrypt%d data...\n", do_encrypt);
    for(;;) {
        inlen = fread(inbuf, 1, QL_TZ_AES_DATA_MAX_SIZE, in_fp);
        if(inlen <= 0) break;
   
        if (!do_encrypt) {
            EVP_CIPHER_CTX_ctrl(&ctx, EVP_CTRL_CCM_SET_TAG, QL_TZ_AES_TAG_SIZE, (void *)&tag_buff[i * tag_size]);
           // EVP_CipherInit_ex(&ctx, NULL, NULL, tz_key, NULL, do_encrypt);
        }

        EVP_CipherUpdate(&ctx, NULL, &outlen, NULL, inlen);//Set L
        //EVP_CipherUpdate(&ctx, NULL, &outlen, aad, sizeof(aad));//Set Additional data, not support
        
        if (!EVP_CipherUpdate(&ctx, outbuf, &outlen, inbuf, inlen)) {//first CBC-MAC, second CTR Encrypt
            QL_TZ_LOG("Cipher Update failed\n");
            goto CIPHER_END;
        }
        QL_TZ_LOG("Once Cipher outlen %d\n", outlen);
        fwrite(outbuf, 1, outlen, out_fp);
        outlen = 0;
        
        if (do_encrypt) {
            //get tag
            EVP_CIPHER_CTX_ctrl(&ctx, EVP_CTRL_CCM_GET_TAG, QL_TZ_AES_TAG_SIZE, &tag_buff[i * tag_size]);
            //QL_TZ_LOG("Tag:\n");
            //BIO_dump_fp(stdout, &tag_buff[i * tag_size], QL_TZ_AES_TAG_SIZE);
        }
        
        i++;
    }

    if((ret = EVP_CipherFinal_ex(&ctx, outbuf, &outlen)) < 0) {
        QL_TZ_LOG("Cipher Final failed\n");
        goto CIPHER_END;
    }
    QL_TZ_LOG("Final outlen %d\n", outlen);
    fwrite(outbuf, 1, outlen, out_fp);
    
    if (do_encrypt) {
        QL_TZ_LOG("Save TAG info\n");
        fseek(out_fp, sizeof(aes_enc_hdr_t), SEEK_SET);
        fwrite(tag_buff, 1, tag_buff_size, out_fp);
    }
    QL_TZ_LOG("ret =%d, encrypt done.\n", ret);
    
CIPHER_END:
    fclose(key_fp);
    fclose(in_fp);
    fclose(out_fp);
    EVP_CIPHER_CTX_cleanup(&ctx);
    return 1;
}


void ql_test_rand_gen(void)
{
    int ret;
    uchar rand_data[512];
    
    memset(rand_data, 0x00, sizeof(rand_data));
    
    ret = RAND_bytes(rand_data, sizeof(rand_data));
    if (ret > 0) {
        QL_TZ_LOG("New TZ BIG RANDOM DATA:\n");
        BIO_dump_fp(stdout, rand_data, sizeof(rand_data));
    }
    
    memset(rand_data, 0x00, sizeof(rand_data));
    ret = RAND_bytes(rand_data, 18);
    if (ret > 0) {
        QL_TZ_LOG("New TZ SMALL RANDOM DATA:\n");
        BIO_dump_fp(stdout, rand_data, 18);
    }
}


void printHelp(void)
{
    printf(
           "./example_crypto_tz_test gen_rand\n"
           "./example_crypto_tz_test aes_gen_key <key_file>\n"
           "./example_crypto_tz_test aes_enc     <key_file> <plain_file> <enc_file>\n"
           "./example_crypto_tz_test aes_dec     <key_file> <enc_file>   <dec_file>\n"
           "./example_crypto_tz_test rsa_gen_key <key_file>\n"
           "./example_crypto_tz_test rsa_sign    <key_file> <plain_file> <sign_file>\n"
           "./example_crypto_tz_test rsa_verify  <key_file> <plain_file> <sign_file>\n"
          );
}


ENGINE* ql_test_openssl_tz_init(void)
{
    ENGINE *qltz_e = NULL;
    const char *engine_id = QL_TZ_ENGINE_ID;
    /**
     * Initial openssl libs
     */
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();
    ENGINE_load_builtin_engines();
    
    /**
     * Inital QuecTZ Engine
     */
    qltz_e = ENGINE_by_id("dynamic");
    ENGINE_ctrl_cmd_string(qltz_e, "SO_PATH", "/usr/lib/libql_openssl_tz.so.1", 0);
    ENGINE_ctrl_cmd_string(qltz_e, "ID", engine_id, 0);
    ENGINE_ctrl_cmd_string(qltz_e, "LOAD", NULL, 0);

    if (!qltz_e) {
        /* the engine isn't available */
        printf("can't get engine\n");
        exit(-1);
    }
    
    return qltz_e;
}


int main(int argc, char *argv[])
{
    int ret;
    ENGINE *qltz_e = NULL;
     
    if (argc < 2) {
        printHelp();
        return 0;
    }


    if (!strcmp(argv[1], "gen_rand")) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_RAND(qltz_e);
        ql_test_rand_gen();
    }
    else if (!strcmp(argv[1], "aes_gen_key") && argc == 3) {
         qltz_e = ql_test_openssl_tz_init();
         ENGINE_set_default_ciphers(qltz_e);
         ql_test_aes_key_gen(argv[2]);
    }
    else if (!strcmp(argv[1], "aes_enc") && argc == 5) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_ciphers(qltz_e);
        ql_test_aes_256_ccm_tzcrypt(argv[2], argv[3], argv[4], 1);
    }
    else if (!strcmp(argv[1], "aes_dec") && argc == 5) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_ciphers(qltz_e);
        ql_test_aes_256_ccm_tzcrypt(argv[2], argv[3], argv[4], 0);
    }
    else if (!strcmp(argv[1], "rsa_gen_key") && argc == 3) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_RSA(qltz_e);
        ql_test_gen_RSAkey(1024, argv[2]);
        ql_test_read_RSAkey(argv[2]);
    }
    else if (!strcmp(argv[1], "rsa_sign") && argc == 5) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_RSA(qltz_e);
        ql_test_RsaSign(argv[2], argv[3], argv[4]);
    }
    else if (!strcmp(argv[1], "rsa_verify") && argc == 5) {
        qltz_e = ql_test_openssl_tz_init();
        ENGINE_set_default_RSA(qltz_e);
        ql_test_RsaVerify(argv[2], argv[3], argv[4]);
    }
    else {
        printHelp();
        return 0;
    }
    
    ENGINE_finish(qltz_e);
    ENGINE_free(qltz_e);
    return 0;
}