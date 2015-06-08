#pragma once
#include <string.h>
#include "../System.h"
using namespace std;

class tcp_server
{

public:
    tcp_server(int);
    virtual ~tcp_server();
    int start_listening();

protected:
    int port;
    SOCKET sSock; // Server Socket
    SOCKET cSock; // Client Socket

private:
    int acceptConns();

};