#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 4433
#define MESSAGE "Hello"

// /home/bhoopesh/OPENSSL_DIR/include/openssl -- headers (ssl.h, err.h)
// /home/bhoopesh/OPENSSL_DIR -- binaries (libcrypto.a, libssl.a)
// /home/bhoopesh/OPENSSL_DIR/oqs/include/oqs - liboqs

// gcc -o server server.c -I/path/to/custom_openssl/include -L/path/to/custom_openssl/lib -lssl -lcrypto

// gcc -o server server.c -I/home/bhoopesh/OPENSSL_DIR/include/openssl -L/home/bhoopesh/OPENSSL_DIR -L/home/bhoopesh/OPENSSL_DIR/oqs/lib -lssl -lcrypto -loqs

// gcc -o server server.c -I/usr/local/include/openssl -L/usr/local/lib -L/usr/local/lib -lssl -lcrypto -loqs


int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int sockfd;

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_client_method());

    if (!ctx) {
        printf("Error creating SSL context\n");
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_ADDR, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        return 1;
    }

    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    if (SSL_connect(ssl) <= 0) {
        printf("Error connecting SSL\n");
        return 1;
    }

    printf("Connected to server\n");

    if (SSL_write(ssl, MESSAGE, strlen(MESSAGE)) < 0) {
        printf("Error sending data\n");
        return 1;
    }

    printf("Data sent: %s\n", MESSAGE);

    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    close(sockfd);

    return 0;
}
