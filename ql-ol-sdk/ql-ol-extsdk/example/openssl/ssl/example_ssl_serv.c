#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


/* define HOME to be dir for key and cert files... */
#define HOME "./"
/* Make these what you want for cert & key files */
#define CERTF       HOME "server_cert.pem"
#define KEYF        HOME "server_key.pem"
#define CACERTF     HOME "rootca_cert.pem"


#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

int main (int argc, char *argv[])
{
    int err;
    int listen_sd;
    int sd;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    size_t client_len;
    SSL_CTX* ctx;
    SSL*     ssl;
    X509*    client_cert;
    char*    str;
    char     buf [4096];
    const SSL_METHOD *meth;
    int mutual_auth = 0; //mutual authentication

    if (argc == 2 && strcmp(argv[1], "mutual") == 0) {
        printf("Enable mutual authentication\n");
        mutual_auth = 1;
    }
    else {
        printf("You are using one-way authentication mode\n");
        printf("If you want to use two-way authentication mode, please input:\n"
               "./example_ssl_serv mutual\n"
              );
    }
    
    /* SSL preliminaries. We keep the certificate and key with the context. */
    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();
    meth = TLSv1_2_server_method();
    ctx = SSL_CTX_new (meth);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(2);
    }
    
    if (mutual_auth) {
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER  | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0);//pay attention to sync system time
        if (SSL_CTX_load_verify_locations(ctx, CACERTF, 0) != 1) {
            SSL_CTX_free(ctx);
            printf("Failed to load CA file %s", CACERTF);
            exit(1);
        }
        
        if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
            SSL_CTX_free(ctx);
            printf("Call to SSL_CTX_set_default_verify_paths failed");
            exit(2);
        }
    }
    
    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(3);
    }
    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(4);
    }

    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr,"Private key does not match the certificate public key\n");
        exit(5);
    }
    
    
    SSL_CTX_set_cipher_list(ctx,"RC4-MD5");
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);

    /* ----------------------------------------------- */
    /* Prepare TCP socket for receiving connections */

    listen_sd = socket (AF_INET, SOCK_STREAM, 0);   CHK_ERR(listen_sd, "socket");
    
    memset (&sa_serv, '\0', sizeof(sa_serv));
    sa_serv.sin_family      = AF_INET;
    sa_serv.sin_addr.s_addr = INADDR_ANY;
    sa_serv.sin_port        = htons (1111);          /* Server Port number */
    
    err = bind(listen_sd, (struct sockaddr*) &sa_serv,
            sizeof (sa_serv));                   CHK_ERR(err, "bind");
            
    /* Receive a TCP connection. */
            
    err = listen (listen_sd, 5);                    CHK_ERR(err, "listen");
    
    client_len = sizeof(sa_cli);
    sd = accept (listen_sd, (struct sockaddr*) &sa_cli, &client_len);
    CHK_ERR(sd, "accept");
    close (listen_sd);

    printf ("Connection from %lx, port %x\n",
        sa_cli.sin_addr.s_addr, sa_cli.sin_port);
    
    /* ----------------------------------------------- */
    /* TCP connection is ready. Do server side SSL. */

    ssl = SSL_new (ctx);                           CHK_NULL(ssl);
    SSL_set_fd (ssl, sd);
    err = SSL_accept (ssl);                        CHK_SSL(err);
    
    /* Get the cipher - opt */
    
    printf ("SEVR SSL connection using %s\n", SSL_get_cipher (ssl));
    
    /* Get client's certificate (note: beware of dynamic allocation) - opt */

    client_cert = SSL_get_peer_certificate (ssl);
    if (client_cert != NULL) {
        printf ("Client certificate:\n");
        
        str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t subject: %s\n", str);
        OPENSSL_free (str);
        
        str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t issuer: %s\n", str);
        OPENSSL_free (str);
        
        /* We could do all sorts of certificate verification stuff here before
        deallocating the certificate. */
        
        X509_free (client_cert);
    } else
        printf ("Client does not have certificate.\n");

    /* DATA EXCHANGE - Receive message and send reply. */

    err = SSL_read (ssl, buf, sizeof(buf) - 1);                   CHK_SSL(err);
    buf[err] = '\0';
    printf ("Got %d chars:'%s'\n", err, buf);
    
    err = SSL_write (ssl, "I hear you.", strlen("I hear you."));  CHK_SSL(err);

    /* Clean up. */

    close (sd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);
}
/* EOF - serv.cpp */
