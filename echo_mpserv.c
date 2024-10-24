#include "echo_mpserv.h"
#include <openssl/provider.h>

int main(int argc, char *argv[]){
    init_openssl();

    SSL_CTX *ctx = create_context();
    //OSSL_PROVIDER_load(OSSL_LIB_CTX_new(), "oqsprovider");

    //set_context(ctx);


    if(argc != 3){
        printf("Usage : %s <port>\n", argv[0]);
    }

    char cert_file[100];
    char key_file[100];
    char kex[50];
    if (strcmp(argv[2], "dil2") == 0) {
        strcpy(cert_file, "dns/new_cert/dil2_crt.pem");
        strcpy(key_file, "dns/new_cert/dil2_priv.key");
        strcpy(kex, "kyber512");
    } else if (strcmp(argv[2], "dil3") == 0) {
        strcpy(cert_file, "dns/new_cert/dil3_crt.pem");
        strcpy(key_file, "dns/new_cert/dil3_priv.key");
        strcpy(kex, "kyber768");
    } else if (strcmp(argv[2], "dil5") == 0) {
        strcpy(cert_file, "dns/new_cert/dil5_crt.pem");
        strcpy(key_file, "dns/new_cert/dil5_priv.key");
        strcpy(kex, "kyber1024");
    }
    else if (strcmp(argv[2], "fal512") == 0) {
        strcpy(cert_file, "dns/new_cert/fal512_crt.pem");
        strcpy(key_file, "dns/new_cert/fal512_priv.key");
        strcpy(kex, "kyber512");
    }
    else if (strcmp(argv[2], "fal1024") == 0) {
        strcpy(cert_file, "dns/new_cert/fal1024_crt.pem");
        strcpy(key_file, "dns/new_cert/fal1024_priv.key");
        strcpy(kex, "kyber1024");
    }

    if(!SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM))
        error_handling("fail to load cert");
    if(!SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM))
        error_handling("fail to load cert's private key");

    SSL_CTX_set_keylog_callback(ctx, keylog_callback);


    SSL_CTX_set_num_tickets(ctx, 0);
 
    int serv_sock = create_listen(atoi(argv[1]));

    pid_t pid;
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    int str_len;
    char buf[BUF_SIZE];

    while(1){
        socklen_t adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_adr, &adr_sz);
        if(clnt_sock < 0){
            printf("continue~\n");
            continue;
        }else{
            puts("new client connected ...");
        }

        SSL* ssl = SSL_new(ctx);
        if(!SSL_set1_groups_list(ssl, kex))
            error_handling("fail to set kex algorithm");
        SSL_set_fd(ssl, clnt_sock);

        if(SSL_accept(ssl) <= 0){
            ERR_print_errors_fp(stderr);
            error_handling("fail to accept TLS connection");
        }

        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        // child process
        if(pid == 0){
            close(serv_sock);
            while((str_len = SSL_read(ssl,buf, BUF_SIZE)) != 0){

                printf("buf : %s", buf);
                if(!strncmp(buf, "hello\n",str_len)){
                    strcpy(buf, "Nice to have\n");
                    SSL_write(ssl, buf, strlen(buf));
                }else{
                    SSL_write(ssl, buf, str_len);
                }
                memset(buf, 0, sizeof(char)*BUF_SIZE);
            }
            SSL_shutdown(ssl);
            SSL_free(ssl);
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    SSL_CTX_free(ctx);
    EVP_cleanup();
    return 0;
}
/*
 * error messages
 */
void init_openssl(){
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}
SSL_CTX *create_context(){
    SSL_CTX* ctx = SSL_CTX_new(SSLv23_server_method());
    if(!ctx) error_handling("fail to create ssl context");

    //SSL_CTX_set_max_proto_version(ctx, TLS1_3_VERSION);
    return ctx;
}
/*
 * set ecdh curves automatically
 * set cert and its private key
 */

void keylog_callback(const SSL* ssl, const char *line){
//    printf("==============================================\n");
//    printf("%s\n", line);
}
/*
 * create socket fd to listen
 * return : server socket
 */
int create_listen(int port){
    int serv_sock;
    struct sockaddr_in serv_adr;
    struct sigaction act;

    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGCHLD, &act, 0);

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(port);

    if(serv_sock < 0)
        error_handling("fail to create socket");

    int enable = 1;
    if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        error_handling("SO_REUSEADDR failed");
    }

    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) < 0)
        error_handling("bind() error");

    if(listen(serv_sock, 5) < 0) 
        error_handling("listen() error");

    printf("Listening on port %d\n", port);

    return serv_sock;
}

void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed process id : %d \n", pid);
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
