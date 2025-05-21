#pragma once
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdexcept>
#include <string>

#include<memory>

namespace baseSocket{
    constexpr char DEFAULT_PORT[] = "3000";
    constexpr int MaxDataSize = 4096;

    class baseSocket {
        private:
            int baseSocketDes;        
            struct addrinfo hints;
            struct addrinfo* serverInfo;
            struct sockaddr_in clientAddr;
            socklen_t clientAddrSize;
            int connections;
        public:
            baseSocket(const std::string&);
            ~baseSocket();
            void SetupHints();
            void GetServerInfo(const std::string& port);
            void CreateSocket();
            void BindSocket();
            void ListenSocket();
    };
}