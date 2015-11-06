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
#include "NetworkManager.h"
#include "Application.h"


#ifdef TRP_USE_NETWORK

#include "SDL_net.h"


/*----------------------------------------------------------------------------*/

NetworkManager& NetworkManager::Get()
{
	static NetworkManager foo;
	return foo;
}

/*----------------------------------------------------------------------------*/
NetworkManager::NetworkManager() : command(NULL)
{

}

/*----------------------------------------------------------------------------*/
NetworkManager::~NetworkManager()
{
}

/*----------------------------------------------------------------------------*/
void NetworkManager::Init()
{

if (SDLNet_Init()== -1)
	{
	SDL_Log("SDLNET_Init Failed !!!\n");
	}
else
	{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"SDLNET_Init OK\n");
	}
mode = MODE_NONE;

}

/*----------------------------------------------------------------------------*/

void  NetworkManager::ConnectToServer(const std::string &_serverName)
{
	client = new Client(_serverName/*"10.31.5.29"*/, 8080, 512);
	client->ConnectToServer();
	mode = MODE_CLIENT;
}

/*----------------------------------------------------------------------------*/

void NetworkManager::CreateServer()
{
	server = new Server(8080, 512, 4);
	mode = MODE_SERVER;
		
	activeClient = -1;
}

/*----------------------------------------------------------------------------*/

void NetworkManager::MakeDirToAllClients(const std::string& _dirname)
{
	if (mode == MODE_SERVER)
		{
		server->MakeDirToAllClients(_dirname);
		}

}

/*----------------------------------------------------------------------------*/

void NetworkManager::SendFileToAllClients(const std::string& _filename)
{
	if (mode == MODE_SERVER)
		{
		server->SendFileToAllClients(_filename);
		}

}

/*----------------------------------------------------------------------------*/

void NetworkManager::SendMessageToAllClients(const std::string& _msg)
{
	if (mode == MODE_SERVER)
		{
		server->SendMessageToAllClients(_msg);
		}
}

/*----------------------------------------------------------------------------*/

void NetworkManager::Update()
	{
	if (mode == MODE_SERVER)
		{
		server->CheckForConnections();

		// At least once, but as many times as necessary to process all active clients...
		do
			{
			// ..get the client number of any clients with unprocessed activity (returns -1 if none)
			this->command = server->CheckForActivity();
			} 
		while (activeClient != -1);

		} //MODE SERVER
	else
	if (mode == MODE_CLIENT)
		{
			
		SDL_RWops *rw = client->CheckForIncomingMessages();
		if (rw != NULL)
			{
			//unsigned int length = (unsigned int) SDL_RWseek(rw,0,SEEK_END);
			//SDL_RWseek(rw,0,SEEK_SET);// ON retourne au début

			char command[2];
			while(SDL_RWread(rw,command,2,1) == 1)
				{
				//Tant qu'il y a des commandes à manger
				if ((command[0] == 'R')&&(command[1] == '>'))
					{
					g_app->doneCode = DONECODE_RESTART_ONLY; //Restart
					}
                else
				if ((command[0] == 'U')&&(command[1] == '>'))
					{
					client->ReceiveFile(rw);                    
					}
                else
				if ((command[0] == 'D')&&(command[1] == '>'))
					{
					client->MakeDir(rw);                    
					}

				}
			SDL_RWclose(rw);
			}
		}


	}

/*----------------------------------------------------------------------------*/

void NetworkManager::Shutdown()
{			
if (server != NULL)
	{
	delete server;
	server = NULL;
	}
SDLNet_Quit();
}

#endif