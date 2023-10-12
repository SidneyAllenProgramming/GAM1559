#include "Connection.h"

Connection::Connection(const SOCKET cSocket, const sockaddr_in cSocketAddress)
	: clientSocket(cSocket),
	  clientSocketAddress(cSocketAddress)
{
}
