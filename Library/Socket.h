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


// namespace baseSocket{
//     constexpr char DEFAULT_PORT[] = "3000";
//     constexpr int MaxDataSize = 4096;

//     class baseSocker {
//         private:        
//         struct addrinfo hints;
//         struct addrinfo* serverInfo;
//         struct sockaddr_in clientAddr;
//         socklen_t clientAddrSize;
//         int connections;

//     }
// }
namespace Socket {
    constexpr char DEFAULT_PORT[] = "3000";
    constexpr int MaxDataSize = 4096;
    void *connection_handler(void *socket_desc);
    
    class Socket {
    
    public:
        Socket(const std::string&);
        ~Socket();
        void SetupHints();
        void GetServerInfo(const std::string& port);
        void CreateSocket();
        void BindSocket();
        void ListenSocket();
    private:
        int serverSocket, clientSocket;
        struct addrinfo hints;
        struct addrinfo* serverInfo;
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize;
        int connections;
        //static void HandleError(const std::string& message);

    };

}