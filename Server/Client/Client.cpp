#include "Client.h"

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