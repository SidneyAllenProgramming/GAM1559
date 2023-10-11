#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

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
	void StartChatRoom();

	void Ping();

	bool InitializeWindowsSockets();

private:
	void SetClientSockAddr(const char* serverIP, const int serverPort);
	void Read_Message();
	void Send_Message();


private:
	SOCKET cSocket;
	sockaddr_in cSocketAddress;
};