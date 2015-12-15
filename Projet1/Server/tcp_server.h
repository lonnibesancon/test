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
	void ProcessMessage(float data[]);

};

#endif