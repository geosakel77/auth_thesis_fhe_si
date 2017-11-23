//
// Created by george on 7/11/2017.
//

#ifndef AUTH_THESIS_FHE_SI_CLIENT_H
#define AUTH_THESIS_FHE_SI_CLIENT_H

#include<iostream>    //cout
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr


using namespace std;

class Client {

private:
    int sock;
    string address;
    int port;
    struct sockaddr_in server;
public:

    int conn(const string &addrr, int);
    bool send_data(string data);
    string receive(int);

};

#endif //AUTH_THESIS_FHE_SI_CLIENT_H
