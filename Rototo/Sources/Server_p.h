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

#ifndef __SERVER_P_H__
#define __SERVER_P_H__


class Server_p
{
public:

		IPaddress serverIP;         // The IP of the socket server (this will end up being 0.0.0.0 - which means roughly "any IP address")
		TCPsocket serverSocket;     // The server socket that clients will connect to
		TCPsocket *pClientSocket;   // A pointer to (what will be) an array of sockets for the clients
		SDLNet_SocketSet socketSet; // Our entire set of sockets

public:
	Server_p();


};
#endif
