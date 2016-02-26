#ifndef TCPSERVER
#define TCPSERVER
#include <string.h>
#include "../System.h"
#include "../Drawing.h"
using namespace std;

class tcp_server
{

public:
	bool hasToClose ;
    tcp_server(int port, Drawing* d);

    virtual ~tcp_server();
    //int start_listening1();
	int start_listening();
	void initializeWinsock();
	//void ProcessIncomingMessage();
	void waitForMessage();
	int getNbOfClients();
	void sendMessage();
	void setNbOfClients(int n);

protected:
    int port;
	int nbOfClients ;
    SOCKET ListenSocket; // Server Socket
    SOCKET SendSocket; // Client Socket
	Drawing* drawing ;

private:
    //int acceptConns();
	void ProcessMessage(int msgType, int interactionMode = 0, int mapper=0, double* mat = 0, double* origin=0, double* normal=0, double* seedPoint = 0);

};

#endif