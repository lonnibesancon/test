#ifndef TCPSERVER
#define TCPSERVER
#include <string.h>
#include "../System.h"
#include "../Drawing.h"
using namespace std;

class tcp_server
{

public:
    tcp_server(int port, Drawing* d);
    virtual ~tcp_server();
    int start_listening();

protected:
    int port;
    SOCKET ListenSocket; // Server Socket
    SOCKET ClientSocket; // Client Socket
	Drawing* drawing ;

private:
    int acceptConns();
	void ProcessMessage(float data[]);

};

#endif