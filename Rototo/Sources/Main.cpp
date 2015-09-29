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



#include "Application_p.h"
#include "Atlas.h"

#include "Sprite.h"

#ifdef TRP_USE_MAIN

int main(int argc, char *argv[])
{
	
	g_app = new Application();

	int doneCode = DONECODE_NOT_DONE;
	do
		{
		g_app->Init();	
		doneCode = g_app->Run();
		g_app->Shutdown();
		}
	while( doneCode != DONECODE_REAL_QUIT);

	delete g_app;
	return 0;
}

#endif