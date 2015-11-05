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
#include "Global.h"
#include "Client.h"
#include "Application.h"
#include "Utils.h"
#include "ResourceManager.h"

#include "SDL_net.h"
#include "Client_p.h" //TODO Maybe not here ?

#ifdef TRP_USE_NETWORK



#ifdef TRP_IOS
#include <sys/stat.h>
#endif
const std::string  Client::SERVER_NOT_FULL           = "OK\n";        // Server's response when there are free spots a client can join
const unsigned int Client::CONNECTION_TIMEOUT_PERIOD = 5000;        // 5 second connection timeout
const unsigned int Client::SOCKET_SET_POLL_PERIOD    = 0;          // 10ms, so poll 100 times/second


/*----------------------------------------------------------------------------*/
Client_p::Client_p()
{
	//TODO Initialize members here
}


/*----------------------------------------------------------------------------*/

Client::Client(std::string _theServerAddress, unsigned int _theServerPort, unsigned int _theBufferSize) :client_p(new Client_p)
{

	// The host name of the server.
	// This can be either a dot-quad like 127.0.0.1 or a hostname like "localhost" or "foo.com" etc.
	serverHostname = _theServerAddress;

	serverPort = _theServerPort; // The port on the server to connect to
	bufferSize = _theBufferSize; // The maximum message size



	pBuffer = new char[bufferSize]; // Create the transmission buffer character array


	// Create a socket set big enough to hold the server socket and our own client socket
	client_p->socketSet = SDLNet_AllocSocketSet(2);

	// If we couldn't create the socket set then throw an exception
	if (client_p->socketSet == NULL)
		{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Failed to allocate the socket set in ClientSocket constructor: %s ",SDLNet_GetError());
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Client::~Client()
{
	// Close our server and client sockets
	SDLNet_TCP_Close(client_p->serverSocket);
	SDLNet_TCP_Close(client_p->clientSocket);

	// Free our socket set (i.e. all the clients in our socket set)
	SDLNet_FreeSocketSet(client_p->socketSet);

	// Release any properties on the heap
	delete pBuffer;

}




/*----------------------------------------------------------------------------*/
/* Function to poll for clients connecting                                    */
/*----------------------------------------------------------------------------*/

void Client::ConnectToServer()
{
	// Try to resolve the hostname to an IP address, if it's already an IP address then that's fine
	// If successful, this places the connection details in the serverIP object.
	int hostResolved = SDLNet_ResolveHost(&(client_p->serverIP), serverHostname.c_str(), serverPort);

	// If we couldn't resolve the hostname then throw an error
	if (hostResolved == -1)
		{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error: Failed to resolve the server hostname to an IP address.");
		}
	else
		{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"SDLNet_ResolveHost OK 2\n");
		}

	Uint8 * dotQuad = (Uint8*)&(client_p->serverIP.host);
	
	dotQuadString  = toString( (unsigned short)dotQuad[0] );
	dotQuadString += ".";
	dotQuadString += toString( (unsigned short)dotQuad[1] );
	dotQuadString += ".";
	dotQuadString += toString( (unsigned short)dotQuad[2] );
	dotQuadString += ".";
	dotQuadString += toString( (unsigned short)dotQuad[3] );
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Successfully resolved server to IP: %s \n",dotQuadString.c_str());

	
	// Try to open a connection between the client and the server - quit out if we can't connect
	client_p->clientSocket = SDLNet_TCP_Open(&client_p->serverIP);
	if (!client_p->clientSocket)
		{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Error: Failed to open connection to server: %s",SDLNet_GetError());
		}
	else // If we successfully opened a connection then check for the server response to our connection
		{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Connection okay, about to read connection status from the server...\n");

		// Add our socket to the socket set for polling
		SDLNet_TCP_AddSocket(client_p->socketSet, client_p->clientSocket);

		// Wait for up to five seconds for a response from the server.
		// Note: If we don't check the socket set and WAIT for the response, we'll be checking before the
		// server can respond, and it'll look as if the server sent us nothing back!
		int activeSockets = SDLNet_CheckSockets(client_p->socketSet, Client::CONNECTION_TIMEOUT_PERIOD);

		//cout << "There are " << activeSockets << " socket(s) with data on them at the moment." << endl;
	
		if (activeSockets != 0)
			{

			// Check if we got a response from the server
				int gotServerResponse = SDLNet_SocketReady(client_p->clientSocket);

			if (gotServerResponse != 0)
				{
				// Read the message on the client socket
				int serverResponseByteCount = SDLNet_TCP_Recv(client_p->clientSocket, pBuffer, bufferSize);

				if (serverResponseByteCount != 0)
					{
					//cout << "Message from server: " << pBuffer << "(" << serverResponseByteCount << " bytes)" << endl;

					// Get the contents of the buffer as a string
					std::string bufferContents = pBuffer;

					// If we got the SERVER_NOT_FULL response from the server then we can join!
					if ( pBuffer == Client::SERVER_NOT_FULL )
						{
						SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Joining server now...\n");
						}
					else // Otherwise we must have got the SERVER_FULL response so we can't.
						{
						SDL_Log("Error: Server is full...\n");
						}
					}
				}
			else // If there's no activity on the client socket then we've failed to connect
				{
				SDL_Log("Error: No response from server: \n");
				}
			} // if (activeSockets != 0)
		} // End of if we managed to open a connection to the server condition
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Client::MakeDir(SDL_RWops * _rw)
{
	char dirName[20];

	int ret;
	ret = (int) SDL_RWread(_rw,dirName,1,20);
	SDL_assert(ret == 20);
	char * pFind = strchr(dirName,' ');
	SDL_assert(pFind != NULL);
	*pFind = 0;

	std::string tempString = "Server asks to makedir ";
	tempString += dirName;
	SDL_Log(tempString.c_str());

	
#ifdef TRP_IOS
	std::string fullDirPath;
	
	fullDirPath =  "..//Documents//" + g_app->settings_gamedataURL+"//";
	
	ret = mkdir(fullDirPath.c_str(), S_IRWXU);
	if (ret != 0)
		SDL_Log("Cannot create gamedata Directory");
	
	fullDirPath += dirName;

	ret = mkdir(fullDirPath.c_str(), S_IRWXU);
	if (ret != 0)
		SDL_Log("Cannot create Directory");
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Client::ReceiveFile(SDL_RWops * _rw)
{
	char fileName[20];

	int ret;
	ret = (int) SDL_RWread(_rw,fileName,1,20);
	SDL_assert(ret == 20);
	char * pFind = strchr(fileName,' ');
	SDL_assert(pFind != NULL);
	*pFind = 0;
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"FILENAME RECEIVED = %s",fileName);
	int length;
	ret = (int) SDL_RWread(_rw,&length,1,4);
	if (ret != 4)
		{
		//Not enough data in the stream, ask myself for more (blocking)
			int serverResponseByteCount = SDLNet_TCP_Recv(client_p->clientSocket, &length, 4);
		SDL_assert(serverResponseByteCount == 4);
	}
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"LENGTH OF THE RECEIVED FILE = %d \n",length);

	

	SDL_RWops *out = ResourceManager::Get().Save(fileName, GAMEDATA | BOTH);


	char *tempBuffer = NULL;
	tempBuffer = new char[bufferSize]; //Violent
	
	//First try to read everything
	ret = (int) SDL_RWread(_rw,tempBuffer,1,length);
	if (ret != length)
		{
		//Write at least what we have
		SDL_RWwrite(out, tempBuffer, ret, 1);
		//We don't have anything try to read the rest
		int remain = length - ret;
		do
			{
			int nbToread =  MYMIN(remain,bufferSize);
			int serverResponseByteCount = SDLNet_TCP_Recv(client_p->clientSocket, tempBuffer, nbToread);
			SDL_RWwrite(out, tempBuffer, serverResponseByteCount, 1);
			remain = remain - serverResponseByteCount;
		
			}
		while (remain);
		}
	else
		{
		//We got evrything, just write it to disk
		SDL_RWwrite(out, tempBuffer, length, 1);
		}

	SDL_RWclose(out);
	delete []tempBuffer;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SDL_RWops* Client::CheckForIncomingMessages()
{

SDL_RWops* tempFile = NULL;

	// Poll for messages for a specified time (default: 10ms, so 100 times per second)
int activeSockets = SDLNet_CheckSockets(client_p->socketSet, Client::SOCKET_SET_POLL_PERIOD);

	// This produces a LOT of debug output, so only uncomment if the code's really misbehaving...
	//if (debug) { cout << "There are " << activeSockets << " socket(s) with data on them at the moment." << endl; }

	if (activeSockets != 0)
		{
		// Check if we got a message from the server
			int gotMessage = SDLNet_SocketReady(client_p->clientSocket);

		if (gotMessage != 0)
			{
				int serverResponseByteCount = SDLNet_TCP_Recv(client_p->clientSocket, pBuffer, bufferSize);

			if (serverResponseByteCount > 0)
				{
				SDL_Log("serverResponseByteCount %d\n",serverResponseByteCount);
				tempFile = SDL_RWFromMem(pBuffer,serverResponseByteCount);
				}
			else // If we've received a 0 byte message from the server then we've lost the connection!
				{
				SDL_Log("Lost connection to the server!\n");
				}

			} // End of if (gotMessage != 0) section

		} // End of if (activeSockets != 0) section

	// Return the message, whether the actually is a message, or whether it's blank
	return tempFile;

}
#endif 