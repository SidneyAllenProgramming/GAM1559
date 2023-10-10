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

	void Initialize();
	void Connect(const char* message, SOCKET connectingSocket, char** argv);


	void Shutdown(int iResult);

	void ServerConnect(const char* serverIP, const int serverPort);
	void SetClientSockAddr(const char* serverIP, const int serverPort);
	void StartChatRoom();
	void Read_Message();
	void Send_Message();

	void Ping();

	bool InitializeWindowsSockets();

private:
	SOCKET cSocket;
	sockaddr_in cSocketAddress;
};