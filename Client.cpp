//
// Created by george on 7/11/2017.
//

#include "Client.h"

/**
    Connect to a host on a certain port number
*/

using namespace std;
int Client::conn( const string &address, int port)
{
    //create socket if it is not already created
    if(sock == -1)
    {
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }

        cout<<"Socket created\n";
    }

    server.sin_addr.s_addr = inet_addr( address.c_str() );


    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server

    if (connect(sock,(struct sockaddr *)&server,sizeof(server)) < 0)
    {
        perror("Connection Failed. Error");
        return 1;
    }

    cout<<"Connected\n";
    return 0;
}

/**
    Send data to the connected host
*/
bool Client::send_data(string data)
{
    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        perror("Send failed : ");
        return false;
    }
    cout<<"Data send\n";

    return true;
}

/**
    Receive data from the connected host
*/
string Client::receive(int size=512)
{
    char buffer[size];
    string reply;

    //Receive a reply from the server
    if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    {
        puts("recv failed");
    }

    reply = buffer;
    return reply;
}