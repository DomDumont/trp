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

#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#ifdef TRP_USE_NETWORK

#include "SDL_net.h"
#include <string>
#include "Server.h"
#include "Client.h"

////http://r3dux.org/2011/01/a-simple-sdl_net-chat-server-client/

#define MODE_NONE 0
#define MODE_SERVER 1
#define MODE_CLIENT 2


class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void Init();
	void CreateServer();
	void ConnectToServer(const std::string &_serverName);
	void SendMessageToAllClients(const std::string& _msg);
	void SendFileToAllClients(const std::string& _filename);
    void MakeDirToAllClients(const std::string& _dirname);
	void Update();
	void Shutdown();

private:
	Server *server;
	int		mode;

	// Specify which client is active, -1 means "no client is active"
	int activeClient;

	Client *client;

public:
    SDL_RWops * command;
};

#endif 

#endif