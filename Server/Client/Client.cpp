#include "Client.h"

int __cdecl main(int argc, char** argv)
{
    const char *message = "This is the Client.";

    Client().Initialize();

    Client().ServerConnect("127.0.0.1", 12345);
    //Client().Connect(message, cSocket, argv);
}

Client::Client()
{
}

Client::~Client()
{
}

void Client::Initialize()
{
    if (InitializeWindowsSockets() == false)
    {
        return;
    }

    return;
}

void Client::Connect(const char* message, SOCKET cSocket, char** argv)
{
    // Create a connecting socket to connect to the server.
    cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (cSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());

        WSACleanup();
        return;
    }

    // create and initialize address structure
    cSocketAddress.sin_family = AF_INET;
    cSocketAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    cSocketAddress.sin_port = htons(DEFAULT_PORT);

    // connect to server specified in serverAddress and socket connectSocket
    if (connect(cSocket, (SOCKADDR*)&cSocketAddress, sizeof(cSocketAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");
        
        closesocket(cSocket);
        WSACleanup();
    }
}

void Client::Shutdown(int iResult)
{
    // Delete socket
    iResult = closesocket(cSocket);
    printf("remote client socket has closed");
    if (iResult == SOCKET_ERROR)
    {
        printf("Unable to close socket.\n");

        closesocket(cSocket);
        WSACleanup();
        return;
    }

    // CleanUp Winsock
    iResult = WSACleanup();
    printf("remote client winsock has closed");
    if (iResult == SOCKET_ERROR)
    {
        printf("Unable to cleanup Winsock.\n");

        closesocket(cSocket);
        WSACleanup();
        return;
    }
}

void Client::ServerConnect(const char* serverIP, const int serverPort)
{
    int iResult;

    // Create a connecting socket to connect to the server.
    cSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (cSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());

        WSACleanup();
        return;
    }

    SetClientSockAddr(serverIP, serverPort);

    // connect to server specified in serverAddress and socket connectSocket
    if (connect(cSocket, (SOCKADDR*)&cSocketAddress, sizeof(cSocketAddress)) == SOCKET_ERROR)
    {
        printf("Unable to connect to server.\n");

        closesocket(cSocket);
        WSACleanup();
    }

    std::string clientName;

    printf("Input your name, User.\n");
    std::getline(std::cin, clientName);
    iResult = send(cSocket, clientName.c_str(), (int)strlen(clientName.c_str()) + 1, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Error at send(): %d\n", WSAGetLastError());

        closesocket(cSocket);
        WSACleanup();
        return;
    }

    StartChatRoom();

    while (true)
    {
        Sleep(1000);
    }
}

void Client::SetClientSockAddr(const char* serverIP, const int serverPort)
{
    // create and initialize address structure
    cSocketAddress.sin_addr.s_addr = inet_addr(serverIP);
    cSocketAddress.sin_port = htons(serverPort);
    cSocketAddress.sin_family = AF_INET;
}

void Client::StartChatRoom()
{
    bool disconnect = true;

    while (!disconnect)
    {
        //std::thread thread_for_message_read(&Client::Read_Message);
        //thread_for_message_read.join();
    }
    while (!disconnect)
    {
        /*std::thread thread_for_message_send(&Client::Send_Message);
        thread_for_message_send.join();*/
    }
}

void Client::Read_Message()
{
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    
    // recv a message from the server.
    iResult = recv(cSocket, recvbuf, sizeof(DEFAULT_BUFLEN), 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Error at recv(): %d\n", WSAGetLastError());

        closesocket(cSocket);
        WSACleanup();
        return;
    }
    else if (iResult == 0)
    {
        printf("Error at recv(): %d\n", WSAGetLastError());

        closesocket(cSocket);
        WSACleanup();
        return;
    }

    printf("> ");
    printf(recvbuf);
    printf("\n");
}

void Client::Send_Message()
{
    int iResult;
    std::string message;
    printf("You: ");
    std::getline(std::cin, message);
    iResult = send(cSocket, message.c_str(), (int)strlen(message.c_str()) + 1, 0);

    if (iResult == SOCKET_ERROR)
    {
        printf("Error at send(): %d\n", WSAGetLastError());

        closesocket(cSocket);
        WSACleanup();
        return;
    }
}

void Client::Ping()
{
    int iResult;

    char* pong = new char[10];

    // Loop through and send "Ping" 10 times, after receiving a "Pong".
    for (int i = 0; i < 10; i++)
    {
        // send a "Ping" to the server.
        iResult = send(cSocket, "Ping", (int)strlen("Ping") + 1, 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("Error at send(): %d\n", WSAGetLastError());

            closesocket(cSocket);
            WSACleanup();
            return;
        }
        
        // recv a "Pong" from the server.
        iResult = recv(cSocket, pong, sizeof(pong), 0);

        if (iResult == SOCKET_ERROR)
        {
            printf("Error at recv(): %d\n", WSAGetLastError());

            closesocket(cSocket);
            WSACleanup();
            return;
        }

        // print the server message to console.
        printf(pong);
        Sleep(500);
    }

    Shutdown(iResult);
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