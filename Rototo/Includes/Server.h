/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2015 - Dominique Dumont

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

#ifndef __SERVER_H__
#define __SERVER_H__



#include <string>

#ifdef TRP_USE_NETWORK

struct SDL_RWops;
class Server_p;

class Server
{
public:
		static const std::string SERVER_NOT_FULL;
		static const std::string SERVER_FULL;

		Server(unsigned int _port, unsigned int _bufferSize, unsigned int _maxSockets);
		~Server();

		// Function to poll for clients connecting
		void CheckForConnections();

		// Function to poll for client activity (i.e. message sent or dropped connection)
		// Returns either the number of an active client, or -1 if no clients with activity to process
		SDL_RWops * CheckForActivity();

		void SendMessageToAllClients(const std::string& _msg);
		void SendFileToAllClients(const std::string& _filename);
        void MakeDirToAllClients(const std::string& _dirname);


private:
		std::unique_ptr<Server_p> server_p; // opaque type here

		unsigned int port;          // The port our server will listen for incoming connecions on
		unsigned int bufferSize;    // Size of our message buffer
		unsigned int maxSockets;    // Max number of sockets
		unsigned int maxClients;    // Max number of clients in our socket set (defined as maxSockets - 1 as the server socket itself take 1 port)


		bool *pSocketIsFree;        // A pointer to (what will be) an array of flags indicating which client sockets are free
		char *pBuffer;              // A pointer to (what will be) an array of characters used to store the messages we receive


		unsigned int clientCount;   // Count of how many clients are currently connected to the server

};
#endif
#endif
