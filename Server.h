//
// Created by george on 7/11/2017.
//

#ifndef AUTH_THESIS_FHE_SI_SERVER_H
#define AUTH_THESIS_FHE_SI_SERVER_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

using namespace std;

class Server {

private:
    string serverIp;
    int serverPort;
    int masterSocket;

    void SocketCreate();

    void SocketBind();

    void SocketListen();

    static void *SocketAccept(void *);

    static void *SocketRead(void *);

    void RequestDispatch(int, string);

public:
    Server(string, int);

    static void Send(int, string);

    static void Log(int, string);


};


#endif //AUTH_THESIS_FHE_SI_SERVER_H
