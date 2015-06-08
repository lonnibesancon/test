#include "tcp_server.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std ;
tcp_server::tcp_server( int port)
{
    tcp_server::port = port;
}

tcp_server::~tcp_server()
{
}

int tcp_server::start_listening()
{
    /* WSAStartup() initializes
 the program to call winsock2.dll */
    WORD wVersionRequested;
    WSADATA wsaData;
    int wsaerr;
    wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        printf("Server: The Winsock dll not found!n");
        WSACleanup();
        return 0;
    }

    else
    {
        printf("Server: The Winsock2 dll foundn");
    }
    /* SOCKET is simply a UINT, created because
 on Unix sockets are file descriptors(UINT) but not in windows
 so new type SOCKET was created */
    sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sSock == INVALID_SOCKET)
    {
        printf("Server: Error initializing socket!n");
        WSACleanup();
        return 0;
    }

    /* The SOCKADDR_IN structure is used by
 Win Sockets to specify an endpoint address
 to which the socket connects */
    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
    service.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* bind just links the socket
 with the sockaddr_in struct we initialized */
    if(bind(sSock,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR)
    {
        printf("Server: Error binding socket to portn");
        WSACleanup();
        return 0;
    }

    /* wait for incoming connections */
    if(listen(sSock,10)==SOCKET_ERROR)
        printf("listen(): Error listening on socket %d.n", WSAGetLastError());
    else
    {
        printf("Server: Waiting for connections...n");
    }
    printf("Server: Waiting for a client to connect...n");
    /* accept connections */
    acceptConns();
    return 0;
}

/* TODO: To change functionality
 of the server change the following function... */
int tcp_server::acceptConns()
{
    sockaddr_in from;
	bool infinite = true ;
    int fromlen=sizeof(from);
    /* Infinte loop to echo
 the IP address of the client */

	int readsize = 0 ;
	char* message;
	char* clientmessage = "\0";
	string smatrix ;
	int ind ;
	string tok;
	int i = 0 ;
	int bytesSent ;

	float matrix[16] ;

	while( (readsize = recv(sSock, clientmessage, 2000, 0))> 0 ){
		message = "ack";
		bytesSent = send(sSock, message, strlen(message),MSG_DONTROUTE);
		if(bytesSent = 0){
			std::cerr << "Error sending ACK" << endl ;
		}
		smatrix = clientmessage ;
		std::stringstream ss(smatrix);

		cout << smatrix << endl ;
		while(getline(ss, tok, ',') && i < 16 ){
			matrix[i] = static_cast<float>(::atof(tok.c_str()));
			i++ ;
		}
		i = 0 ;
	}
	if(readsize == 0 ){
		cerr << "client disconnected" << endl ;
	}

    while (infinite)
    {
        char temp[512];
        cSock = accept(sSock,(SOCKADDR*)&from,&fromlen);
        std::cout<<"Your IP address is : " << inet_ntoa(from.sin_addr) << std::endl ;
        send(cSock,temp,strlen(temp),0);
        std::cout << "Connection from " << inet_ntoa(from.sin_addr) <<"n";
        closesocket(cSock);
    }
    closesocket(sSock);
    WSACleanup();
    return 0;
}
