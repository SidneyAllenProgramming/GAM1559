#include "Server.h"
#include <string>

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

void Server::Initialize()
{
    if (InitializeWindowsSockets() == false)
    {
        return;
    }

    return;
}

void Server::StartServer(SOCKET listenSocket)
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
        return;
    }

    // Create a socket for server connection.
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (listenSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());

        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    // Setup the listening socket to listen mode.
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %ld\n", WSAGetLastError());

        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    
    printf("Server initialized, waiting for clients.\n");

    AcceptConnections(listenSocket);

    return;
}

void Server::AcceptConnections(SOCKET listenSocket)
{
    //char recvbuf[DEFAULT_BUFLEN];
    SOCKET acceptedSocket = INVALID_SOCKET;

    // Accept a client socket
    acceptedSocket = accept(listenSocket, NULL, NULL);
    if (acceptedSocket == INVALID_SOCKET) 
    {
        printf("accept failed with error: %d\n", WSAGetLastError());

        closesocket(listenSocket);
        WSACleanup();
        return;
    }
    
    ReceiveMessage(acceptedSocket);

    closesocket(listenSocket);
    return;
}

void Server::ReceiveMessage(SOCKET acceptedSocket)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];

    // Somehow, I need to wait and then receive the client's name here?

    while (true)
    {
        // receive the client's name.
        iResult = recv(acceptedSocket, recvbuf, sizeof(DEFAULT_BUFLEN), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(acceptedSocket);
            WSACleanup();
            break;
        }
        else if (iResult == 0)
        {
            break;
        }

        // Send a Pong string to the client.
        //iResult = send(acceptedSocket, pongStr.c_str(), (int)pongStr.length() + 1, 0);
        //if (iResult == SOCKET_ERROR)
        //{
        //    printf("send failed with error: %d\n", WSAGetLastError());
        //    closesocket(acceptedSocket);
        //    WSACleanup();
        //    break;
        //}
    }

    Shutdown(iResult, acceptedSocket);

    return;
}

void Server::Pong(SOCKET acceptedSocket)
{
    int iResult;
    char pingStr[10];

    while (true) 
    {
        // receive the client's ping message.
        iResult = recv(acceptedSocket, pingStr, sizeof(pingStr), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(acceptedSocket);
            WSACleanup();
            break;
        }
        else if (iResult == 0)
        {
            break;
        }

        // Print the client's ping to the console.
        printf(pingStr);
        printf("\n");

        std::string pongStr = "Pong\n"; //Testing Api with STL string

        // Send a Pong string to the client.
        iResult = send(acceptedSocket, pongStr.c_str(), (int)pongStr.length() + 1, 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(acceptedSocket);
            WSACleanup();
            break;
        }
    }

    Shutdown(iResult, acceptedSocket);

    return;
}

void Server::Shutdown(int iResult, SOCKET connectingSocket)
{
    // shutdown the connection since we're done
    iResult = shutdown(connectingSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(connectingSocket);
        WSACleanup();
        return;
    }

    // cleanup
    closesocket(connectingSocket);
    WSACleanup();

    return;
}

void Server::SetServerSockAddr(sockaddr_in* sockAddress, int portNumber)
{
    sSocketAddress = *sockAddress;
}

void Server::StartChatRoom()
{
}

void Server::AddClientToRoom(Connection& c)
{
}

void Server::ReadNessage(Connection& c)
{
}

void Server::WriteMEssage()
{
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