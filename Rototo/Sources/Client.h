/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2014 - Dominique Dumont

   Permission is granted to use this software under the terms of either:
   a) the GPL v3 (or any later version)
   b) the Affero GPL v3

   Details of these licenses can be found at: www.gnu.org/licenses

   T.R.P. is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

   -----------------------------------------------------------------------------

   To release a closed-source product which uses T.R.P., commercial licenses are
   available: visit veed.fr for more information.

  ==============================================================================
*/

#ifndef __CLIENT_H__
#define __CLIENT_H__

#ifdef TRP_USE_NETWORK

#include "SDL_net.h"

#include <iostream>
#include <string>
#include <sstream>

class Client
{
public:

		static const unsigned int CONNECTION_TIMEOUT_PERIOD;
		static const std::string       SERVER_NOT_FULL;
		static const unsigned int SOCKET_SET_POLL_PERIOD;


		Client(std::string _theServerAddress, unsigned int _theServerPort, unsigned int _theBufferSize);
		~Client();
		// Function to poll for clients connecting
		void ConnectToServer();
		void ReceiveFile(SDL_RWops * _rw);
        void MakeDir(SDL_RWops * _rw);
		SDL_RWops* CheckForIncomingMessages();



private:

    std::string    dotQuadString;    // The IP address of the server to connect to as a dot-quad (i.e. 127.0.0.1)
		unsigned int serverPort;    // The port of the server to connect to
		int			bufferSize;    // The size of our message buffer

		std::string    serverHostname;   // The host name of the server (i.e. "localhost", "www.foo.com" etc.

		char   *pBuffer;            // A pointer to (what will be) an array of characters used to store the messages we send and receive


		SDLNet_SocketSet socketSet; // Our entire set of sockets (i.e. just the server socket and our client socket)

		TCPsocket serverSocket;     // The server socket
		TCPsocket clientSocket;     // Our own client socket

		IPaddress serverIP;         // The IP address of the server to connect to (NOT in dot-quad format)




};

// Template function to convert most anything to a string
template<class T>
std::string toString(const T& t)
{
	std::ostringstream stream;
	stream << t;
	return stream.str();
}
#endif 
#endif