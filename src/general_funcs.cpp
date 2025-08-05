#include "general_funcs.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>

// Srv: domain - nullptr, flag - AI_PASSIVE. Func for client and server.
int CreateSocketStruct(struct addrinfo** res, SocketAddress host, SocketConfig conf) {
    struct addrinfo hints = {0};
    hints.ai_family = conf.ai_family;
    hints.ai_flags = conf.ai_flags;
    hints.ai_socktype = conf.ai_socktype;

    return getaddrinfo(host.domain, host.port, &hints, res);
}

void ChooseAddrinfoStruct(struct addrinfo* res, struct addrinfo** hints, SocketAddress host) {
    while (res != nullptr) {
        struct sockaddr_in* ipv4 = (sockaddr_in*)(res->ai_addr);
        if (CheckIp(ipv4, host) && CheckPort(ipv4, host)) {
            *hints = res;
            break;
        }

        res = res->ai_next;
    }
}

bool CheckIp(struct sockaddr_in* ai_addr, SocketAddress host) {
    char ipv4_addres[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ai_addr->sin_addr), ipv4_addres, INET_ADDRSTRLEN);

    return strcmp(ipv4_addres, host.ip) == 0;
}

bool CheckPort(struct sockaddr_in* ai_addr, SocketAddress host) {
    in_port_t port = ntohs(ai_addr->sin_port);

    return (int)port == atoi(host.port);
}

void InputMessage(std::string& message) {
    std::cout << "Your message: ";
    std::getline(std::cin, message);
    message.push_back('\n');
}

// Debug function
void PrintIp(struct addrinfo* hints) {
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)(hints->ai_addr);

    char address[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), address, INET_ADDRSTRLEN);

    std::cout << address << '\n';
}