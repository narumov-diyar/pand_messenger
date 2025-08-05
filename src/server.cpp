#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "errors.hpp"
#include "general_funcs.hpp"

int main() {
    struct addrinfo* hints = nullptr;
    struct addrinfo* res = nullptr;

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    SocketAddress host = {
        .ip = "0.0.0.0",
        .domain = nullptr,
        .port = "55556",
    };
    SocketConfig conf = {
        .ai_family = AF_INET,
        .ai_flags = AI_PASSIVE,
        .ai_socktype = SOCK_STREAM,
    };

    int status = CreateSocketStruct(&res, host, conf);
    PrintError(status, "Creating user socket structure", GetFuncCode);

    ChooseAddrinfoStruct(res, &hints, host);
    if (hints == nullptr)
        exit(EXIT_FAILURE);

    int server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    PrintError(server_fd, "Socket fd creating", SocketCode);

    status = bind(server_fd, (struct sockaddr*)&(res->ai_addr), sizeof(res->ai_addr));
    PrintError(status, "Binding socket", SocketCode);

    status = listen(server_fd, 5);
    PrintError(status, "Binding socket", SocketCode);
    if (status == -1)
        close(server_fd);

    std::cout << "Server listening on port " << host.port << "...\n";

    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    PrintError(status, "Accepting client", SocketCode);
    if (status == -1)
        close(server_fd);

    std::cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << std::endl;

    char buffer[1024];
    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        std::cout << "Client says: " << buffer << std::endl;
        send(client_fd, "Hello from server!", 18, 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}