#include "Server.h"

int main()
{
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET acceptedSocket = INVALID_SOCKET;

    Server().Initialize();
    Server().StartServer(listenSocket);
}

Server::Server()
{
}

Server::~Server()
{
}

bool Server::Initialize()
{
    if (InitializeWindowsSockets() == false)
    {
        return 1;
    }

    return 0;
}

bool Server::StartServer(SOCKET listenSocket)
{
    int iResult;

    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Returns socket's address info to the iResult var.
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);

        WSACleanup();
        return 1;
    }

    // Create a socket for server connection.
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());

        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Setup the listening socket to listen mode.
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %ld\n", WSAGetLastError());

        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    
    printf("Server initialized, waiting for clients.\n");

    AcceptConnections(listenSocket);

    return 0;
}

bool Server::AcceptConnections(SOCKET listenSocket)
{
    int iResult;

    char recvbuf[DEFAULT_BUFLEN];
    SOCKET acceptedSocket = INVALID_SOCKET;

    do
    {
        // Accept a client socket
        acceptedSocket = accept(listenSocket, NULL, NULL);
        if (acceptedSocket == INVALID_SOCKET) 
        {
            printf("accept failed with error: %d\n", WSAGetLastError());

            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }

        do
        {
            // Receive data until the client shuts down the connection
            iResult = recv(acceptedSocket, recvbuf, DEFAULT_BUFLEN, 0);
            if (iResult > 0)
            {
                printf("Message received from client: %s.\n", recvbuf);
            }
            else if (iResult == 0)
            {
                // connection was closed gracefully
                printf("Connection with client closed.\n");
                closesocket(acceptedSocket);
            }
            else
            {
                // there was an error during recv
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(acceptedSocket);
            }
        } while (iResult > 0);

        // here is where server shutdown logic could be placed

    } while (1);

    // shutdown the connection since we're done
    iResult = shutdown(acceptedSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(acceptedSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(listenSocket);
    closesocket(acceptedSocket);
    WSACleanup();

    return 0;
}

bool Server::Pong()
{
    return false;
}

bool Server::InitializeWindowsSockets()
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