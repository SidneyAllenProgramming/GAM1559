#include "Server.h"

int main()
{
    Server().Initialize();
    Server().StartServer();

    //Server().StartChatRoom();
    &Server::StartChatRoom;
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

void Server::StartServer()
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
    sSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (sSocket == INVALID_SOCKET)
    {
        printf("socket failed with error: %ld\n", WSAGetLastError());

        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind(sSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("bind failed with error: %d\n", WSAGetLastError());

        freeaddrinfo(result);
        closesocket(sSocket);
        WSACleanup();
        return;
    }

    // Setup the listening socket to listen mode.
    if (listen(sSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("listen failed with error: %ld\n", WSAGetLastError());

        closesocket(sSocket);
        WSACleanup();
        return;
    }
    
    printf("Server initialized, waiting for clients.\n");

    return;
}

void Server::AcceptConnections()
{
    SOCKET acceptedSocket = INVALID_SOCKET;

    // Accept a client socket
    acceptedSocket = accept(sSocket, NULL, NULL);
    if (acceptedSocket == INVALID_SOCKET) 
    {
        printf("accept failed with error: %d\n", WSAGetLastError());

        closesocket(sSocket);
        WSACleanup();
        return;
    }
    
    ReceiveMessage(acceptedSocket);

    closesocket(sSocket);
    return;
}

void Server::ReceiveMessage(SOCKET acceptedSocket)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];

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
    std::thread connectionHandlerThread([&]
        {
            SOCKET acceptedSocket = INVALID_SOCKET;
            sockaddr_in clientSocketAddress;

            // Accept a client socket
            acceptedSocket = accept(sSocket, (struct sockaddr*)&clientSocketAddress, (int*)sizeof(clientSocketAddress));
            if (acceptedSocket == INVALID_SOCKET)
            {
                printf("accept failed with error: %d\n", WSAGetLastError());

                closesocket(sSocket);
                WSACleanup();
                return;
            }


            for (int i = 0; i < connections.size(); i++)
            {
                connections[i].clientSocketAddress = clientSocketAddress;
                Server::AddClientToRoom(connections[i]);
            }

            closesocket(sSocket);
            return;
        });

    connectionHandlerThread.detach();
}

void Server::AddClientToRoom(Connection& c)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];

    while (true)
    {
        // receive the client's name.
        iResult = recv(c.clientSocket, recvbuf, sizeof(DEFAULT_BUFLEN), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(c.clientSocket);
            WSACleanup();
            break;
        }
        else if (iResult == 0)
        {
            break;
        }
        
        c.clientName = recvbuf;
    }
    std::thread t(&Server::Read_Message, &c);
    t.detach();
    
    connections.insert(std::pair<SOCKET, Connection>(c.clientSocket, c));
}

void Server::Read_Message(Connection c)
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    
    while (true)
    {
        // receive the client's message.
        iResult = recv(c.clientSocket, recvbuf, sizeof(DEFAULT_BUFLEN), 0);
        if (iResult == SOCKET_ERROR)
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(c.clientSocket);
            WSACleanup();
            break;
        }
        else if (iResult == 0)
        {
            break;
        }

        printf("> ");
        printf(recvbuf);
        printf("\n");

        for (auto i : connections)
        {
            if (i.first != c.clientSocket)
            {
                // send the message to all other clients.
                iResult = send(i.first, recvbuf, sizeof(recvbuf) + 1, 0);
                if (iResult == SOCKET_ERROR)
                {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(i.first);
                    WSACleanup();
                    break;
                }
            }
        }
    }
}

void Server::Write_Message()
{
    int iResult;
    std::string message;
    printf("You: ");
    std::getline(std::cin, message);
    iResult = send(sSocket, message.c_str(), (int)strlen(message.c_str()) + 1, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Error at send(): %d\n", WSAGetLastError());

        closesocket(sSocket);
        WSACleanup();
        return;
    }
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
