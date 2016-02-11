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

void tcp_server::ProcessMessage(int msgType, int conditionalInfo, int mapper, double* mat, double* position, double* orientation){
	/*cout << mat[0] << ";" << data[1] << endl ;
	if(data[0] != CLIENTID ){
		cerr << "Not a valid ID" << endl ;
	}*/
	cout << "Modification" << endl ;
	switch(msgType){
		case NONEWEVENT:
			cerr << "Error in the event type" << endl ;
			break ;
		case MATRIXCHANGED:
			//cout << "Matrix" << endl ;
			drawing->setMapper(mapper);
			drawing->setTransformationMatrix(conditionalInfo, mat);
			//cout << origin[0] << origin[1] << origin[2] << endl ;
			//cout << " Coucou" << endl ;
			drawing->setPositionAndOrientation(position, orientation);
			break;
		case ENDPROGRAM:
			this->hasToClose = true ;
			break;
		default:
			cerr << "Error: unknown even type" << endl ;
			break ;
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
		double allvalues[NUMBEROFITEMSINMESSAGE];
		double matrix[16] ;
		int interactionMode = OBJECTINTERACTION ; // Default case
		int messageType = NONEWEVENT ;
		int mapper = 0 ;
		//cout << "Line : " << msg << endl ;

		//First what kind of message is that
		getline(ss, tok, ';');
		messageType = atoi(tok.c_str());
		getline(ss, tok, ';');
		interactionMode = atoi(tok.c_str());
		getline(ss, tok, ';');
		mapper = atoi(tok.c_str());

		//THEN POSITION AND ORIENTATION INFO
		if(messageType != NONEWEVENT && messageType != ENDPROGRAM && messageType != CHANGEDATASET){
			while(getline(ss, tok, ';') && i < NUMBEROFITEMSINMESSAGE ){
				//cout << " new boucle " << endl ;
				//matrix[i] = static_cast<float>(::atof(tok.c_str()));
				allvalues[i] = stod(tok.c_str());
				//cout << matrix[i] << ";" ;
				//cout << i << " : "<< allvalues[i-1]<< endl ;
				i++ ;
			}
		}
		//cout << "fin" << endl ;
		
		for(int i = 0 ; i < 16 ; i++ ){
			matrix[i] = allvalues[i] ;
		}
		double position[3] ;
		position[0] = allvalues[16];
		position[1] = allvalues[17];
		position[2] = allvalues[18];
		double orientation[4];
		orientation[0] = allvalues[19];
		orientation[1] = allvalues[19];
		orientation[2] = allvalues[19];
		orientation[3] = allvalues[19];
		//cout << "Set origin to " << origin[0] << ", " << origin[1] << ", " << origin[2] << endl ;
		//cout << "coucou" << endl ;
		this->ProcessMessage(messageType, interactionMode, mapper, matrix, position,orientation);	
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


