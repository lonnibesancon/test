#include "tcp_server.h"
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <sstream>
#include <string>
#include "../SCProtocol.h"

using namespace std ;
tcp_server::tcp_server( int port, Drawing* d)
{
    tcp_server::port = port;
	tcp_server::drawing = d ;
	tcp_server::hasToClose = false ;
	tcp_server::nbOfClients = 0 ;
	initializeWinsock();
}

tcp_server::~tcp_server()
{
	closesocket(ListenSocket);
    WSACleanup();
}

void tcp_server::setNbOfClients(int n){
	nbOfClients = n ;
}

int tcp_server::getNbOfClients(){
	return nbOfClients;
};

 void tcp_server::initializeWinsock(){
	  /* WSAStartup() initializes
		 the program to call winsock2.dll */
	WORD wVersionRequested;
    WSADATA wsaData;
    int wsaerr;
    wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0)
    {
        printf("Server: ERROR FINDING WINSOCK2 dll!\n");
        WSACleanup();
        return ;
    }

    else
    {
        printf("Server: The Winsock2 dll was found \n");
    }
 }

void tcp_server::ProcessMessage(float data[]){
	cout << data[0] << ";" << data[1] << endl ;
	if(data[0] != CLIENTID ){
		cerr << "Not a valid ID" << endl ;
	}
	else{
		switch((int)data[1]){
			case TOUCHROTATION:
				drawing->rotateCamera(data[2],data[3],data[4]);
				break;
			case TOUCHTRANSLATION:
				drawing->translate(data[2],data[3],data[4]);
				break;
			case TOUCHSCALE:
				drawing->scale(data[2]);
				break;
			case TANGIBLEROTATION:

				break;
			case TANGIBLETRANSLATION:

				break;
			case TANGIBLESCALE:

				break;
			case PARTICLESDATASET:
				tcp_server::drawing->setFileName(PARTICLES);
				tcp_server::drawing->read();
				break;
			case JETLINESDATASET:
				tcp_server::drawing->setFileName(JETLINES);
				tcp_server::drawing->read();
				break;
			case ENDPROGRAM:
				tcp_server::hasToClose = true ;
				break ;
			default:
				cerr << "Not a valid message type" << endl ;
		}
	}
}



int tcp_server::start_listening(){
    sockaddr_in server ;

    //Initialise winsock
/*  printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialised.\n");*/
     
    //Create a socket
    if((ListenSocket = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }
    printf("Socket created.\n");


	//Make non blocking
	//ListenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//unsigned long mode = 1;
	//ioctlsocket(ListenSocket, FIONBIO, &mode);
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
     
    //Bind
    if( bind(ListenSocket ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");
 
   
}

void tcp_server::waitForMessage(){

	int slen , recv_len;
    char buf[MESSAGESIZE];
	sockaddr_in si_other;
	slen = sizeof(si_other) ;	//Get error 10014 winsock if not done

	//keep listening for data
    //while(1)
    //{
        //printf("Waiting for data...");
        fflush(stdout);
         
        //clear the buffer by filling null, it might have previously received data
        memset(buf,'\0', MESSAGESIZE);
         
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(ListenSocket, buf, MESSAGESIZE, 0, (struct sockaddr *) &si_other, &slen)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code : %d" , WSAGetLastError());
			getchar();
            exit(EXIT_FAILURE);
        }

        //print details of the client/peer and the data received
        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        //printf("Data: %s\n" , buf);
		string msg = buf ;
		std::stringstream ss(msg);
		int i = 0 ;
		string tok;
		double matrix[16] ;
		int msgtype ;
		//cout << "Line : " << msg << endl ;
		while(getline(ss, tok, ';') && i < 17 ){
			if(i==0){
				msgtype = atoi(tok.c_str());
				
			}
			else{
				//matrix[i] = static_cast<float>(::atof(tok.c_str()));
				matrix[i-1] = stod(tok.c_str());
				//cout << matrix[i] << ";" ;
				
			}
			i++ ;
        }
		drawing->setTransformationMatrix(matrix);
        
		
    //}
 
    /*closesocket(s);
    WSACleanup();
     
    return 0;*/

}


void tcp_server::sendMessage(){
	int slen , send_len;
    char buf[MESSAGESIZE];
	sockaddr_in si_other;
	slen = sizeof(si_other) ;	//Get error 10014 winsock if not done

	send_len = MESSAGESIZE ;

	if (sendto(ListenSocket, buf, send_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
	{
		printf("sendto() failed with error code : %d" , WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}


