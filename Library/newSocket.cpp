#include "newSocket.h"
#include "StringUtil.h"

namespace baseSocket {
    
    baseSocket::baseSocket(const std::string& port) : connections(0) {
        SetupHints();
        GetServerInfo(port);
        CreateSocket();
        ListenSocket();
    }

    // setup a basic TCP socket with IP4 addressing
    void baseSocket::SetupHints(){
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;   
    }

    //resolve the address info for the server in machine IP:port format
    void baseSocket::GetServerInfo(const std::string& port){
        int status;
        if ((status = getaddrinfo(NULL, port.c_str(), &hints, &serverInfo)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
            exit(1);
        }
    }
    //create a socket 
    void baseSocket::CreateSocket(){
        for (auto p = serverInfo; p != NULL; p = p->ai_next) {
            baseSocketDes = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (baseSocketDes == -1) {
                perror("server: socket");
                continue;
            }
            int yes = 1;
            if (setsockopt(baseSocketDes, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
                perror("setsockopt");
                exit(1);
            }


            if (bind(baseSocketDes, p->ai_addr, p->ai_addrlen) == -1) {
                close(baseSocketDes);
                perror("server: bind");
                continue;
            }
        
            break; 
        }
        if (baseSocketDes == -1) {
            std::cerr << "server: failed to bind" << std::endl;
            exit(2);
        }   
    }
    void baseSocket::ListenSocket(){
        if (listen(baseSocketDes,SOMAXCONN) == -1) {
            perror("listen");
            exit(1);
        }
        std::cout << "server: waiting for connections..." << std::endl;
        
    }

    //listen for incoming connections
    void baseSocket::acceptConnection(){
        clientAddrSize= sizeof(clientAddr);
        int clientSocket = accept(baseSocketDes, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "accept failed" << std::endl;
            return;
        }
        std::cout << "Connection accepted" << std::endl;
        connections++;
        std::cout << "Number of connections: " << connections << std::endl;
        std::cout << "Client IP: " << inet_ntoa(clientAddr.sin_addr) << std::endl;
        std::cout << "Client port: " << ntohs(clientAddr.sin_port) << std::endl;
    }
    
    baseSocket::~baseSocket() {
        freeaddrinfo(serverInfo);
        close(baseSocketDes);
    }  

}


int main(){
    baseSocket::baseSocket server("3000");

    while (true) {
        server.acceptConnection();
    }

    return 0;
}
