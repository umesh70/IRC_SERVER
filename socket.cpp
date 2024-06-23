#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

#define DEFAULT_PORT "3000"
void *connection_handler(void *socket_desc);

class IRCServer {
private:
    int serverSocket, clientSocket;
    struct addrinfo hints, *serverInfo;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize;

public:
    IRCServer(const std::string& port = DEFAULT_PORT) {
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

// Dummy function to act as a thread handler
void *connection_handler(void *socket_desc) {
return NULL;
}

int main() {
    IRCServer server;
    server.listenOnSocket(); 
    return 0;
}
