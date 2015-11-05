#ifndef __CLIENT_P_H__
#define __CLIENT_P_H__

class Client_p
{

public:
	SDLNet_SocketSet socketSet; // Our entire set of sockets (i.e. just the server socket and our client socket)

	TCPsocket serverSocket;     // The server socket
	TCPsocket clientSocket;     // Our own client socket

	IPaddress serverIP;         // The IP address of the server to connect to (NOT in dot-quad format)

public:
	Client_p();
};

#endif