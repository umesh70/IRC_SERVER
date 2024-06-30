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
#include "Library/StringUtil.h"
#include "Library/Process.h"

#define DEFAULT_PORT "3000"
#define MaxDataSize 4096
void *connection_handler(void *socket_desc);

class IRCServer {
private:
    int serverSocket, clientSocket;
    struct addrinfo hints, *serverInfo;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize;
    int connections;


public:
    IRCServer(const std::string& port = DEFAULT_PORT):connections(0) {
        setupHints();
        getServerInfo(port);
        createSocket();
        bindSocket();
        listenOnSocket();
    }


    void setupHints() {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;
    }

    void getServerInfo(const std::string& port) {
        int status;
        if ((status = getaddrinfo(NULL, port.c_str(), &hints, &serverInfo)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
            exit(1);
        }
    }

    void createSocket() {
        for (auto p = serverInfo; p != NULL; p = p->ai_next) {
            serverSocket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (serverSocket == -1) {
                perror("server: socket");
                continue;
            }

            if (bind(serverSocket, p->ai_addr, p->ai_addrlen) == -1) {
                close(serverSocket);
                perror("server: bind");
                continue;
            }
            break; 
        }
    }

    void bindSocket() {
        int yes = 1;
        if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        if (listen(serverSocket, SOMAXCONN) == -1) {
            perror("listen");
            exit(1);
        }
        std::cout << "SERVER: Waiting for connections..." << std::endl;
    }

    void listenOnSocket() {
        clientAddrSize = sizeof(clientAddr);
        while ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize)) != -1) {
            std::cout << "Connection accepted" << std::endl;
            pthread_t threadID;
            int* newSock = new int(clientSocket); 

            if (pthread_create(&threadID, NULL, connection_handler, (void*) newSock) < 0) {
                perror("could not create thread");
                continue;
            }
            pthread_detach(threadID);
        }
        if (clientSocket == -1) {
            perror("accept failed");
        }
    }
    ~IRCServer() {
        freeaddrinfo(serverInfo);
        close(serverSocket);
    }

};


void *connection_handler(void *socket_desc) {

    int Sockdesc = *(int*)socket_desc;
    std::string ServerMessage,ClientMessage;
    char  buffer[MaxDataSize];
    int read_size;
    
    ServerMessage = "N for new User, E for Existing User\n >>";
    write(Sockdesc,ServerMessage.c_str(),ServerMessage.size());
    
    const char * ClientMsg = ClientMessage.c_str();
    
    while((read_size = recv(Sockdesc,buffer,MaxDataSize-1,0))>0){
        buffer[read_size] = '\0';
        ClientMessage = buffer;
        write(Sockdesc,ClientMessage.c_str(),ClientMessage.size());
    }

    return NULL;

}

int main(int argc ,char* argv[]) {
    // // Print each command line argument
    // for (int i = 0; i < argc; i++) {
    //     std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    //     auto word = Util::RemoveWhitespace(argv[i]);
    //     auto check = Util::IsUpperCase(word);
    //     std::cout<<argv[i]<<"  "<<check<<std::endl;
    // }

    std::string Parsed = Process::ArgProcess(argc , argv);
    Process::PostProcess(Parsed);
    

    return 0;
}
