#include "Socket.h"
#include "StringUtil.h"
namespace Socket{
    
    Socket::Socket(const std::string& port):connections(0) {
        SetupHints();
        GetServerInfo(port);
        CreateSocket();
        BindSocket();
        ListenSocket();
    }

    void Socket::SetupHints(){
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;   
    }

    void Socket::GetServerInfo(const std::string& port){
        int status;
        if ((status = getaddrinfo(NULL, port.c_str(), &hints, &serverInfo)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
            exit(1);
        }
    }

    void Socket::CreateSocket(){
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
    void Socket::BindSocket(){
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

    void Socket::ListenSocket(){
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

    Socket::~Socket(){
        freeaddrinfo(serverInfo);
        close(serverSocket);      
    }
    
    
    void* connection_handler(void* socket_desc) {
    int Sockdesc = *(int*)socket_desc;
    std::string ServerMessage, ClientMessage;
    char buffer[MaxDataSize];
    int read_size;

    ServerMessage = "N for new User, E for Existing User\n >>";
    if (send(Sockdesc, ServerMessage.c_str(), ServerMessage.size(), 0) == -1) {
        perror("send");
        return NULL;
    }

    while (true) {
        // Receive response from client
        read_size = recv(Sockdesc, buffer, MaxDataSize - 1, 0);
        if (read_size == -1) {
            perror("recv");
            return NULL;
        }
        
        buffer[read_size] = '\0';  // Null-terminate the received data
        ClientMessage = buffer;
        ClientMessage = Util::RemoveWhitespace(ClientMessage.c_str());

        // Check if the client's message is a single character
        if (ClientMessage.size() == 1) {
            char ClChar = std::toupper(ClientMessage.back());

            if (ClChar == 'N') {
                ServerMessage = "Register with USER command\n>>";
                if (send(Sockdesc, ServerMessage.c_str(), ServerMessage.size(), 0) == -1) {
                    perror("send");
                    return NULL;
                }
                break;  // Exit the loop after processing valid 'N' input
            } else if (ClChar == 'E') {
                ServerMessage = "Proceed with existing user login\n>>";
                if (send(Sockdesc, ServerMessage.c_str(), ServerMessage.size(), 0) == -1) {
                    perror("send");
                    return NULL;
                }
                break;  // Exit the loop after processing valid 'E' input
            }
        }

        // If we reach here, the input was invalid
        ServerMessage = "Invalid input. Please enter 'N' for new User or 'E' for Existing User\n >>";
        if (send(Sockdesc, ServerMessage.c_str(), ServerMessage.size(), 0) == -1) {
            perror("send");
            return NULL;
        }
    }
    
    return NULL;
}

}