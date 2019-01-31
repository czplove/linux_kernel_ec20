#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/* define HOME to be dir for key and cert files... */
#define HOME "./"
/* Make these what you want for cert & key files */
#define CERTF       HOME "client_cert.pem"
#define KEYF        HOME "client_key.pem"
#define CACERTF     HOME "rootca_cert.pem"


#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

int main (int argc, char *argv[])
{
    int err;
    int sd;
    struct sockaddr_in sa;
    SSL_CTX* ctx;
    SSL*     ssl;
    X509*    server_cert;
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
               "./example_ssl_cli mutual\n"
              );
    }
    
    SSLeay_add_ssl_algorithms();
    meth = TLSv1_2_client_method();
    SSL_load_error_strings();
    ctx = SSL_CTX_new (meth);                        CHK_NULL(ctx);

    CHK_SSL(err);
    
    if (mutual_auth) {
        SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, 0);//pay attention to sync system time
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
    }
    
    SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY); 
    
    /* ----------------------------------------------- */
    /* Create a socket and connect to server using normal socket calls. */
    
    sd = socket (AF_INET, SOCK_STREAM, 0);       CHK_ERR(sd, "socket");
    
    memset (&sa, '\0', sizeof(sa));
    sa.sin_family      = AF_INET;
    sa.sin_addr.s_addr = inet_addr ("127.0.0.1");   /* Server IP */
    sa.sin_port        = htons     (1111);          /* Server Port number */
    
    err = connect(sd, (struct sockaddr*) &sa,
            sizeof(sa));                   CHK_ERR(err, "connect");

    /* ----------------------------------------------- */
    /* Now we have TCP conncetion. Start SSL negotiation. */
    
    ssl = SSL_new (ctx);                         CHK_NULL(ssl);    
    SSL_set_fd (ssl, sd);
    err = SSL_connect (ssl);                     CHK_SSL(err);
        
    /* Following two steps are optional and not required for
        data exchange to be successful. */
    
    /* Get the cipher - opt */

    printf ("CLI SSL connection using %s\n", SSL_get_cipher (ssl));
    
    /* Get server's certificate (note: beware of dynamic allocation) - opt */

    server_cert = SSL_get_peer_certificate (ssl);       CHK_NULL(server_cert);
    printf ("Server certificate:\n");
    
    str = X509_NAME_oneline (X509_get_subject_name (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t subject: %s\n", str);
    OPENSSL_free (str);

    str = X509_NAME_oneline (X509_get_issuer_name  (server_cert),0,0);
    CHK_NULL(str);
    printf ("\t issuer: %s\n", str);
    OPENSSL_free (str);

    /* We could do all sorts of certificate verification stuff here before
        deallocating the certificate. */

    X509_free (server_cert);
    
    /* --------------------------------------------------- */
    /* DATA EXCHANGE - Send a message and receive a reply. */

    err = SSL_write (ssl, "Hello World!", strlen("Hello World!"));  CHK_SSL(err);
    
    err = SSL_read (ssl, buf, sizeof(buf) - 1);                     CHK_SSL(err);
    buf[err] = '\0';
    printf ("Got %d chars:'%s'\n", err, buf);
    SSL_shutdown (ssl);  /* send SSL/TLS close_notify */

    /* Clean up. */

    close (sd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);
}
/* EOF - cli.cpp */
