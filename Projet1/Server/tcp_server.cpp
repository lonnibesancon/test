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
        printf("Server: The Winsock dll not found!\n");
        WSACleanup();
        return 0;
    }

    else
    {
        printf("Server: The Winsock2 dll found \n");
    }
    /* SOCKET is simply a UINT, created because
 on Unix sockets are file descriptors(UINT) but not in windows
 so new type SOCKET was created */
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(ListenSocket == INVALID_SOCKET)
    {
        cerr << "Server: Error initializing socket!n" << endl;
        WSACleanup();
        return 0;
    }

    /* The SOCKADDR_IN structure is used by
 Win Sockets to specify an endpoint address
 to which the socket connects */
    sockaddr_in service,client ;
    service.sin_family = AF_INET;
    service.sin_port = htons(port);
	service.sin_addr.s_addr = INADDR_ANY;

    /* bind just links the socket
 with the sockaddr_in struct we initialized */
    if(bind(ListenSocket,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR)
    {
        printf("Server: Error binding socket to port \n");
        WSACleanup();
        return 0;
    }

    /* wait for incoming connections */
    if(listen(ListenSocket,10)==SOCKET_ERROR)
        printf("listen(): Error listening on socket %d\n", WSAGetLastError());
    else
    {
        printf("Server: Waiting for connections...\n");
    }
    
    /* accept connections */
	
	printf("Server: Waiting for a client to connect...\n");
	ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    } else {
        wprintf(L"Client connected.\n");
		acceptConns();

	}

    // No longer need server socket
    closesocket(ListenSocket);
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

    int readsize;
    char* message;
    char* clientmessage = (char*) malloc(2000*sizeof(char));
    string smatrix ;
    int ind ;
    string tok;
    int i = 0 ;
    int bytesSent ;

    float matrix[16] ;
    while( (readsize = recv(ClientSocket, clientmessage, 2000, 0))> 0 ){
        message = "ack";
        bytesSent = send(ClientSocket, message, strlen(message),0);
        if(bytesSent == 0){
            std::cerr << "Error sending ACK" << endl ;
        }
		cout <<"BytesSent" << bytesSent << endl;
        smatrix = clientmessage ;
        std::stringstream ss(smatrix);

        
        while(getline(ss, tok, ',') && i < 16 ){
            matrix[i] = static_cast<float>(::atof(tok.c_str()));
            i++ ;
        }
		cout << matrix << endl ;
        i = 0 ;
		message ="ok";
		bytesSent = send(ClientSocket, message, strlen(message),0);
        if(bytesSent == 0){
            std::cerr << "Error sending ACK" << endl ;
        }
    }
    if(readsize == 0 ){
        cerr << "client disconnected" << endl ;
    }
    cout << "Socket closed" << endl ;
    closesocket(ListenSocket);
    WSACleanup();
	return 0 ;
}
