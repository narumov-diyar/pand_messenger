#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <cstring>
#include <cstdlib>

#include <iostream>


#define SRV_PORT        "55556"
#define SRV_DOMAIN      "jacobeaen.chickenkiller.com"
#define SRV_IP          "91.103.253.65"

/*
Так как сервер имеет только A запись в dns, то клиенты работают только с AF_INET.
Можно использовать ооп в с++ - не вызывать connect(), send() и тд, а делать socket.connect()
*/

typedef enum {
    netfunc_code_check,
    getfunc_code_check,
    socket_code_check,
} err_type;

// inet_pton и др. возвращает 1 при успехе, ф-ции сокетов -1 при ошибке, getaddrinfo 0 при успехе
void print_error(int code, const char *message, err_type type) {
    switch (type) {
        case netfunc_code_check:
            if (code != 1) {
                perror(message);
                exit(EXIT_FAILURE);
            }   
            break;
        case getfunc_code_check:
            if (code != 0) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        case socket_code_check:
            if (code == -1) {
                perror(message);
                exit(EXIT_FAILURE);
            }
            break;
        }
}

// Создание сокета для подключения к серверу
int create_socket_struct(struct addrinfo **res, int family, int socktype, int flags) {
    struct addrinfo hints = {0};
    hints.ai_family = family;
    hints.ai_socktype = socktype;
    hints.ai_flags = flags;

    int status = getaddrinfo(SRV_DOMAIN, SRV_PORT, &hints, res);

    return status;
}

bool check_ip(struct sockaddr_in *ai_addr) {
    char ipv4_addres[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ai_addr->sin_addr), ipv4_addres, INET_ADDRSTRLEN);

    if (strcmp(ipv4_addres, SRV_IP) == 0)
        return true;

    return false;
}

bool check_port(struct sockaddr_in *ai_addr) {
    in_port_t port = ntohs(ai_addr->sin_port);

    return (int)port == atoi(SRV_PORT);
}

void choose_addrinfo_struct(struct addrinfo *res, struct addrinfo **hints) {
    while (res != nullptr) {
        struct sockaddr_in *ipv4 = (sockaddr_in *)(res->ai_addr);
        if ( check_ip(ipv4) && check_port(ipv4) ) {
            *hints = res;
            break;
        }
            
        res = res->ai_next;
    }
}

int main(void) {
    struct addrinfo *res;
    struct addrinfo *hints = nullptr;

    int status = create_socket_struct(&res, AF_UNSPEC, SOCK_STREAM, AI_NUMERICSERV);
    print_error(status, "creating user socket", getfunc_code_check);

    choose_addrinfo_struct(res, &hints);
    if (hints == nullptr)
        exit(EXIT_FAILURE);
    
    int sock = socket(hints->ai_family, hints->ai_socktype, hints->ai_protocol);
    print_error(sock, "socket", socket_code_check);

    // чет устал я, здесь пахнет обычным си, а не С++ с его классами

    freeaddrinfo(res);
    return 0;
}