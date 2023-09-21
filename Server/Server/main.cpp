//#include "Server.h"
//#include "../Client/Client.h"
//
//int __cdecl main(int argc, char** argv)
//{
//    const char *message = "This is the Client.";
//
//    if (argc != 2)
//    {
//        printf("usage: %s server-name\n", argv[0]);
//        
//        return 1;
//    }
//
//    Server* pServer = new Server();
//    Client* pClient = new Client();
//
//
//    SOCKET listenSocket = INVALID_SOCKET;
//    SOCKET acceptedSocket = INVALID_SOCKET;
//
//    pServer->Initialize();
//    pClient->Initialize();
//
//    pServer->StartServer(listenSocket);
//    pServer->AcceptConnections(acceptedSocket);
//
//    pClient->Connect(message, argv);
//}