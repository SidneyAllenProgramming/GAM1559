#pragma once

#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"

class Server
{
public:
	Server();
	~Server();

	bool Initialize();
	bool StartServer(SOCKET listenSocket);
	bool AcceptConnections(SOCKET listenSocket);

	bool InitializeWindowsSockets();

private:
	int m_iResult;
};