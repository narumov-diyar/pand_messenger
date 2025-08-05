#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include "errors.hpp"
#include "general_funcs.hpp"

int main(void) {
    struct addrinfo* res = nullptr;
    struct addrinfo* hints = nullptr;

    SocketAddress host = {
        .ip = "91.103.253.65",
        .domain = "jacobeaen.chickenkiller.com",
        .port = "55556",
    };
    SocketConfig conf = {
        .ai_family = AF_INET,
        .ai_flags = AI_NUMERICSERV,
        .ai_socktype = SOCK_STREAM,
    };

    int status = CreateSocketStruct(&res, host, conf);
    PrintError(status, "Creating user socket structure", GetFuncCode);

    ChooseAddrinfoStruct(res, &hints, host);
    if (hints == nullptr)
        exit(EXIT_FAILURE);

    int server_fd = socket(hints->ai_family, hints->ai_socktype, hints->ai_protocol);
    PrintError(server_fd, "Creating user socket fd", SocketCode);

    std::cout << "IP to conect: ";
    PrintIp(hints);

    status = connect(server_fd, hints->ai_addr, hints->ai_addrlen);
    PrintError(status, "Connecting to server", SocketCode);
    if (status == -1)
        close(server_fd);

    while (true) {
        std::string message;
        InputMessage(message);

        std::cout << "Sending data to ";
        PrintIp(hints);

        status = send(server_fd, message.c_str(), message.length(), 0);
        PrintError(status, "Sending sata to server", SocketCode);
        if (status == -1)
            close(server_fd);
    }

    close(server_fd);
    freeaddrinfo(res);
    return 0;
}