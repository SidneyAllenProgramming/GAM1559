#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <wS2tcpip.h>
#include <windows.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT 12345

bool InitializeWindowsSockets();

int __cdecl main(int argc, char** argv)
{
    SOCKET connectingSocket = INVALID_SOCKET;
    int iResult;
    const char *message = "This is the Client.";

    if (argc != 2)
    {
        printf("usage: %s server-name\n", argv[0]);
        
        return 1;
    }

    if (InitializeWindowsSockets() == false)
    {
        return 1;
    }

    // Create a connecting socket to connect to the server.
    connectingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (connectingSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());

        WSACleanup();
        return 1;
    }

    // create and initialize address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddress.sin_port = htons(DEFAULT_PORT);

    // connect to server specified in serverAddress and socket connectSocket
    if (connect(connectingSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        
        closesocket(connectingSocket);
        WSACleanup();
    }

    // Send an prepared message with null terminator included
    iResult = send(connectingSocket, message, (int)strlen(message) + 1, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Error at send(): %d\n", WSAGetLastError());
        
        closesocket(connectingSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // cleanup
    closesocket(connectingSocket);
    WSACleanup();

    return 0;
}

bool InitializeWindowsSockets()
{
    WSADATA wsaData;
    // Initialize windows sockets library for this process
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup failed with error: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}