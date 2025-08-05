#pragma once

#include <cstring>
#include <string>

typedef struct {
    const char* ip;
    const char* domain;
    const char* port;
} SocketAddress;

typedef struct {
    int ai_family;
    int ai_flags;
    int ai_socktype;
} SocketConfig;

int CreateSocketStruct(struct addrinfo** res, SocketAddress host, SocketConfig conf);
void ChooseAddrinfoStruct(struct addrinfo* res, struct addrinfo** hints, SocketAddress host);
bool CheckIp(struct sockaddr_in* ai_addr, SocketAddress address);
bool CheckPort(struct sockaddr_in* ai_addr, SocketAddress address);
void InputMessage(std::string& message);
void PrintIp(struct addrinfo* hints);