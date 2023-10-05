#pragma once

#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"

class Connection
{
public:
	Connection(const SOCKET cSocket, const sockaddr_in cSocketAddress);

private:
	SOCKET clientSocket;
	sockaddr_in clientSocketAddress;
	std::string clientName;
};