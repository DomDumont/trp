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

#include "Global.h"



#include "Server.h"

#include "Application.h"
#include "Utils.h"

const std::string Server::SERVER_NOT_FULL = "OK\n";
const std::string Server::SERVER_FULL     = "FULL";

Server::Server(unsigned int _port, unsigned int _bufferSize, unsigned int _maxSockets)
	{
	port       = _port;                      // The port number on the server we're connecting to
	bufferSize = _bufferSize;                // The maximum size of a message
	maxSockets = _maxSockets;                // Maximum number of sockets in our socket set
	maxClients = _maxSockets - 1;            // Maximum number of clients who can connect to the server

	pClientSocket = new TCPsocket[maxClients]; // Create the array to the client sockets
	pSocketIsFree = new bool[maxClients];      // Create the array to the client socket free status'
	pBuffer = new char[bufferSize];            // Create the transmission buffer character array

	clientCount    = 0;     // Initially we have zero clients...


	// Create the socket set with enough space to store our desired number of connections (i.e. sockets)
	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (socketSet == NULL)
		{
		SDL_Log("Failed to allocate the socket set: %s",SDLNet_GetError());		
		}

	// Initialize all the client sockets (i.e. blank them ready for use!)
	for (unsigned int loop = 0; loop < maxClients; loop++)
		{
		pClientSocket[loop] = NULL;
		pSocketIsFree[loop] = true; // Set all our sockets to be free (i.e. available for use for new client connections)
		}

	// Try to resolve the provided server hostname to an IP address.
	// If successful, this places the connection details in the serverIP object and creates a listening port on the
	// provided port number.
	// Note: Passing the second parameter as "NULL" means "make a listening port". SDLNet_ResolveHost returns one of two
	// values: -1 if resolving failed, and 0 if resolving was successful
	int hostResolved = SDLNet_ResolveHost(&serverIP, NULL, port);

	if (hostResolved == -1)
		{
		SDL_Log("Failed to open the server socket: %s",SDLNet_GetError());
		}


	// Try to open the server socket
	serverSocket = SDLNet_TCP_Open(&serverIP);

	if (!serverSocket)
		{
		SDL_Log("Failed to open the server socket: ",SDLNet_GetError());
		}


	// Add our server socket (i.e. the listening socket) to the socket set
	SDLNet_TCP_AddSocket(socketSet, serverSocket);

	

	}


Server::~Server()
	{
	// Close all the open client sockets
	for (unsigned int loop = 0; loop < maxClients; loop++)
		{
		if (pSocketIsFree[loop] == false)
			{
			SDLNet_TCP_Close(pClientSocket[loop]);
			pSocketIsFree[loop] = true;
			}
		}

	// Close our server socket
	SDLNet_TCP_Close(serverSocket);

	// Free our socket set
	SDLNet_FreeSocketSet(socketSet);

	// Release any properties on the heap
	delete [] pClientSocket;
	delete [] pSocketIsFree;
	delete [] pBuffer;
	}


void Server::CheckForConnections()
{
	// Check for activity on the entire socket set. The second parameter is the number of milliseconds to wait for.
	// For the wait-time, 0 means do not wait (high CPU!), -1 means wait for up to 49 days (no, really), and any other
	// number is a number of milliseconds, i.e. 5000 means wait for 5 seconds, 50 will poll (1000 / 50 = 20) times per second.
	// I've used 1ms below, so we're polling 1,000 times per second, which is overkill for a small chat server, but might
	// be a good choice for a FPS server where every ms counts! Also, 1,000 polls per second produces negligable CPU load,
	// if you put it on 0 then it WILL eat all the available CPU time on one of your cores...
	int numActiveSockets = SDLNet_CheckSockets(socketSet, 1);

	if (numActiveSockets != 0)
		{
		// Check if our server socket has received any data
		// Note: SocketReady can only be called on a socket which is part of a set and that has CheckSockets called on it (the set, that is)
		// SDLNet_SocketRead returns non-zero for activity, and zero is returned for no activity. Which is a bit bass-ackwards IMHO, but there you go.
		int serverSocketActivity = SDLNet_SocketReady(serverSocket);

		// If there is activity on our server socket (i.e. a client has trasmitted data to us) then...
		if (serverSocketActivity != 0)
			{
			// If we have room for more clients...
			if (clientCount < maxClients)
				{
				// Find the first free socket in our array of client sockets
				int freeSpot = -99;
				for (unsigned int loop = 0; loop < maxClients; loop++)
					{
					if (pSocketIsFree[loop] == true)
						{
						pSocketIsFree[loop] = false; // Set the socket to be taken
						freeSpot = loop;             // Keep the location to add the new connection at that index in the array
						break;                       // Break out of the loop straight away
						}
					}

				// ...accept the client connection and then...
				pClientSocket[freeSpot] = SDLNet_TCP_Accept(serverSocket);

				// ...add the new client socket to the socket set (i.e. the list of sockets we check for activity)
				SDLNet_TCP_AddSocket(socketSet, pClientSocket[freeSpot]);

				// Increase our client count
				clientCount++;

				// Send a message to the client saying "OK" to indicate the incoming connection has been accepted
				strcpy( pBuffer, SERVER_NOT_FULL.c_str() );
				int msgLength = strlen(pBuffer) + 1;
				SDLNet_TCP_Send(pClientSocket[freeSpot], (void *)pBuffer, msgLength);



				SDL_Log("Client connected. There are now %d client(s) connected.\n",clientCount);
				}
			else // If we don't have room for new clients...
				{
				SDL_Log("Max client count reached - rejecting client connection\n");

				// Accept the client connection to clear it from the incoming connections list
				TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);

				// Send a message to the client saying "FULL" to tell the client to go away
				strcpy( pBuffer, SERVER_FULL.c_str() );
				int msgLength = strlen(pBuffer) + 1;
				SDLNet_TCP_Send(tempSock, (void *)pBuffer, msgLength);

				// Shutdown, disconnect, and close the socket to the client
				SDLNet_TCP_Close(tempSock);
				}

			} // End of if server socket is has activity check
		}

}

void Server::MakeDirToAllClients(const std::string& _dirname)
{
//Send Command
std::string msg;
std::string paddedFileName = _dirname ;
PadTo(paddedFileName,20);
msg = "D>"+ paddedFileName;

SendMessageToAllClients(msg);
}

void Server::SendFileToAllClients(const std::string& _filename)
{



SDL_RWops *rw = g_app->resourceManager->Load(_filename,GAMEDATA|BOTH);
if (rw == NULL)
	{
	SDL_Log("bizarre %s\n",SDL_GetError());
	return;
	}

//Send Command
std::string msg;
std::string paddedFileName = _filename ;
PadTo(paddedFileName,20);
msg = "U>"+ paddedFileName;

SendMessageToAllClients(msg);


//Send Length
unsigned int length = (unsigned int) SDL_RWseek(rw,0,SEEK_END);
SDL_RWseek(rw,0,SEEK_SET);// ON retourne au début

SDL_Log("LENGTH SENT = %d 0x%x\n",length,length);
for (unsigned int loop = 0; loop < maxClients; loop++)
	{
	if (pSocketIsFree[loop] == false)
		{
		SDLNet_TCP_Send(pClientSocket[loop], (void *)&length, 4);						
		}
	}


int nbbytes = 0;
 
while ((nbbytes = SDL_RWread(rw,pBuffer,1,bufferSize)) != 0)
	{
	SDL_Log("Send %d bytes of file\n",nbbytes);
	for (unsigned int loop = 0; loop < maxClients; loop++)
		{
		if (pSocketIsFree[loop] == false)
			{
			SDLNet_TCP_Send(pClientSocket[loop], (void *)pBuffer, nbbytes);						
			}
		}
	}
SDL_RWclose(rw);

}
void Server::SendMessageToAllClients(const std::string& _msg)
{
    //SDL_assert(_msg.length()<this->bufferSize);
    std::string stringToSend;
    int msgLength;
    
    stringToSend = _msg; // Copie inutile ?
    
    while (stringToSend.length())
        {
        if (stringToSend.length()<this->bufferSize)
            {
            // Sent everything then
            strcpy( pBuffer, stringToSend.c_str() );
            msgLength = strlen(pBuffer);
            stringToSend = "";
            }
        else
            {
            strcpy( pBuffer, stringToSend.substr(0,this->bufferSize-1).c_str() );
            msgLength = this->bufferSize-1;
            stringToSend = stringToSend.substr(this->bufferSize-1);
            }
        for (unsigned int loop = 0; loop < maxClients; loop++)
            {
                if (pSocketIsFree[loop] == false)
                {
                    SDLNet_TCP_Send(pClientSocket[loop], (void *)pBuffer, msgLength);						
                }
            }

        }
}

SDL_RWops * Server::CheckForActivity()
{
    SDL_RWops* tempFile = NULL;

	// Loop to check all possible client sockets for activity
	for (unsigned int clientNumber = 0; clientNumber < maxClients; clientNumber++)
		{
		// If the socket is has activity then SDLNet_SocketReady() returns non-zero
		int clientSocketActivity = SDLNet_SocketReady(pClientSocket[clientNumber]);

		// The line below produces a LOT of debug, so only uncomment if the code's seriously misbehaving!
		//cout << "Client number " << clientNumber << " has activity status: " << clientSocketActivity << endl;

		// If there is any activity on the client socket...
		if (clientSocketActivity != 0)
			{
			// Check if the client socket has transmitted any data by reading from the socket and placing it in the buffer character array
			int receivedByteCount = SDLNet_TCP_Recv(pClientSocket[clientNumber], pBuffer, bufferSize);

			// If there's activity, but we didn't read anything from the client socket, then the client has disconnected...
			if (receivedByteCount <= 0)
				{
				//...so output a suitable message and then...
				SDL_Log("Client %d disconnected.\n",clientNumber);

				//... remove the socket from the socket set, then close and reset the socket ready for re-use and finally...
				SDLNet_TCP_DelSocket(socketSet, pClientSocket[clientNumber]);
				SDLNet_TCP_Close(pClientSocket[clientNumber]);
				pClientSocket[clientNumber] = NULL;

				// ...free up their slot so it can be reused...
				pSocketIsFree[clientNumber] = true;

				// ...and decrement the count of connected clients.
				clientCount--;

				SDL_Log("Server is now connected to %d client(s).\n",clientCount);
				}
			else // If we read some data from the client socket...
				{
				// ... return the active client number to be processed by the dealWithActivity function
                tempFile = SDL_RWFromMem(pBuffer,receivedByteCount);				
				}

			} // End of if client socket is active check

	} // End of server socket check sockets loop

	// If we got here then there are no more clients with activity to process!
	return tempFile;


}
