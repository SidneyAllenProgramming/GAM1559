#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 12345

class Client
{
public:
	Client();
	~Client();

	bool Initialize();
	bool Connect(const char* message, SOCKET connectingSocket, char** argv);

	bool InitializeWindowsSockets();

private:

};