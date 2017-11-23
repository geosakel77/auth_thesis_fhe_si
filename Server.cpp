//
// Created by george on 7/11/2017.
//

#include "Server.h"

struct ClientRequest {
    int socketFD;
    string request;
};

struct ServerAndSocket {
    Server *socketServer;
    int socketFD;
};


Server::Server(string serverIp, int serverPort) {
    this->serverIp = serverIp;
    this->serverPort = serverPort;

    this->SocketCreate();
    this->SocketBind();
    this->SocketListen();

    pthread_create(new pthread_t(), NULL, Server::SocketAccept, this);
};

void Server::SocketCreate() {
    this->masterSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->masterSocket < 0) {
        perror("socket");
        exit(0);
    } else {
        int opt = 1;
        setsockopt(this->masterSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        cout << "Socket created successfully with file descriptor " << this->masterSocket << "\n";
    };
};

void Server::SocketBind() {
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(this->serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(this->serverIp.data());

    if (bind(this->masterSocket, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("bind");
        exit(0);
    } else {
        cout << "Socket bound successfully to the " << this->serverIp << ":" << this->serverPort << " address\n";
    };
};

void Server::SocketListen() {
    listen(this->masterSocket, 5);
    cout << "Socket is beeing listened now\n";
};

void *Server::SocketAccept(void *serverPointer) {
    int socketFD;
    pthread_t *clientThread;
    ClientRequest clientRequest;
    Server *this_;
    ServerAndSocket serverAndSocketPtr;

    this_ = (Server *) serverPointer;

    while (1) {
        socketFD = accept(this_->masterSocket, NULL, NULL);
        if (socketFD < 0) {
            perror("accept");
        } else {
            this_->RequestDispatch(socketFD, "CLIENT_CONNECTED");

            serverAndSocketPtr.socketServer = this_;
            serverAndSocketPtr.socketFD = socketFD;

            pthread_create(new pthread_t(), NULL, Server::SocketRead, &serverAndSocketPtr);
        };
    };
};

void *Server::SocketRead(void *serverAndSocketPointer) {
    /*
    char input[256];
    int inputLength;
    Server *socketServerPtr;
    int socketFD;
    ServerAndSocket *serverAndSocketPtr;

    serverAndSocketPtr = (ServerAndSocket *) serverAndSocketPointer;
    socketServerPtr = serverAndSocketPtr->socketServer;
    socketFD = serverAndSocketPtr->socketFD;

    while (1) {
        memset((void *) &input, '\0', sizeof(input));
        inputLength = read(socketFD, (void *) &input, 255);
        if (inputLength < 0) {
            perror("read");
        } else if (inputLength == 0 || input[0] == '\0') {
            socketServerPtr->RequestDispatch(socketFD, "CLIENT_DISCONNECTED");
            pthread_exit(NULL);
        } else {
            socketServerPtr->RequestDispatch(socketFD, input);
        };
    };
     */
};




void Server::Send(int socketFD, string message) {

    int bytesWritten;

    bytesWritten = send(socketFD, message.c_str(), message.size() + 1);

    if (bytesWritten < 0) {
        perror("write");
    } else {
        Server::Log(socketFD, "<--- " + message);
    };
};

void Server::Log(int socketFD, string message) {
    sockaddr address;
    socklen_t addressLength;
    sockaddr_in *addressInternet;
    string ip;
    int port;

    getpeername(socketFD, &address, &addressLength);
    addressInternet = (struct sockaddr_in *) &address;

    ip = inet_ntoa(addressInternet->sin_addr);
    port = addressInternet->sin_port;

    cout << ip << ":" << port << " " << message << "\n";
};