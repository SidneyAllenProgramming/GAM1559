#pragma once

#include "Connection.h"
#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"

class Server
{
public:
	Server();
	~Server();

	void Initialize();
	void StartServer();
	void AcceptConnections();
	void ReceiveMessage(SOCKET acceptedSocket);
	void Shutdown(int iResult, SOCKET connectingSocket);

	void SetServerSockAddr(sockaddr_in* sockAddress, int portNumber);
	void StartChatRoom();
	void AddClientToRoom(Connection& c);
	void Read_Message(Connection& c);
	void Write_Message();

	void Pong(SOCKET acceptedSocket);

	bool InitializeWindowsSockets();

private:
	SOCKET sSocket;
	sockaddr_in sSocketAddress;
	std::map<SOCKET, Connection> connections;
};