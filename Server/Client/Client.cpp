#include "Client.h"
#include <iostream>
#include <sstream>
#include <string>

int __cdecl main(int argc, char** argv)
{
    SOCKET connectingSocket = INVALID_SOCKET;
    const char *message = "This is the Client.";

    Client().Initialize();

    Client().Connect(message, connectingSocket, argv);
}

Client::Client()
{
}

Client::~Client()
{
}

bool Client::Initialize()
{
    if (InitializeWindowsSockets() == false)
    {
        return 1;
    }

    return 0;
}

bool Client::Connect(const char* message, SOCKET connectingSocket, char** argv)
{
    int iResult;

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
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(DEFAULT_PORT);

    // connect to server specified in serverAddress and socket connectSocket
    if (connect(connectingSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        
        closesocket(connectingSocket);
        WSACleanup();
    }

    std::string clientName;

    printf("Input your name, User.\n");
    std::getline(std::cin, clientName);
    iResult = send(connectingSocket, clientName.c_str(), (int)strlen(clientName.c_str()) + 1, 0);


}

bool Client::Shutdown(int iResult, SOCKET connectingSocket)
{
    // Delete socket
    iResult = closesocket(connectingSocket);
    printf("remote client socket has closed");
    if (iResult == SOCKET_ERROR)
    {
        printf("Unable to close socket.\n");

        closesocket(connectingSocket);
        WSACleanup();
        return 1;
    }

    // CleanUp Winsock
    iResult = WSACleanup();
    printf("remote client winsock has closed");
    if (iResult == SOCKET_ERROR)
    {
        printf("Unable to cleanup Winsock.\n");

        closesocket(connectingSocket);
        WSACleanup();
        return 1;
    }
}

bool Client::Ping(SOCKET connectingSocket)
{
    int iResult;

    char* pong = new char[10];

    // Loop through and send "Ping" 10 times, after receiving a "Pong".
    for (int i = 0; i < 10; i++)
    {
        // send a "Ping" to the server.
        iResult = send(connectingSocket, "Ping", (int)strlen("Ping") + 1, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("Error at send(): %d\n", WSAGetLastError());

            closesocket(connectingSocket);
            WSACleanup();
            return 1;
        }
        
        // recv a "Pong" from the server.
        iResult = recv(connectingSocket, pong, sizeof(pong), 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("Error at recv(): %d\n", WSAGetLastError());

            closesocket(connectingSocket);
            WSACleanup();
            return 1;
        }

        // print the server message to console.
        printf(pong);
        Sleep(500);
    }

    Shutdown(iResult, connectingSocket);
}

bool Client::InitializeWindowsSockets()
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