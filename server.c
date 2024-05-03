#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define SERVER_PORT 4433
#define SERVER_CERT "falcon512_srv.crt"
#define SERVER_KEY "falcon512_srv.key"

void handle_client(SSL *ssl) {
    char buffer[1024];
    int bytes;

    while (1) {
        bytes = SSL_read(ssl, buffer, sizeof(buffer));
        if (bytes <= 0) {
            printf("Connection closed or error occurred\n");
            break;
        }
        buffer[bytes] = '\0';
        printf("Received: %s", buffer);
        SSL_write(ssl, buffer, strlen(buffer));
    }

    SSL_shutdown(ssl);
    SSL_free(ssl);
}

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int sockfd, client_sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(TLS_server_method());

    if (!ctx) {
        printf("Error creating SSL context\n");
        return 1;
    }

    if (SSL_CTX_use_certificate_file(ctx, SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        perror("Error listening on socket");
        return 1;
    }

    while (1) {
        client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &client_len);
        if (client_sockfd < 0) {
            perror("Error accepting connection");
            continue;
        }

        printf("Client connected\n");

        ssl = SSL_new(ctx);
        SSL_set_fd(ssl, client_sockfd);

        if (SSL_accept(ssl) <= 0) {
            ERR_print_errors_fp(stderr);
            close(client_sockfd);
            continue;
        }

        printf("SSL connection established\n");

        handle_client(ssl);
    }

    close(sockfd);
    SSL_CTX_free(ctx);

    return 0;
}

