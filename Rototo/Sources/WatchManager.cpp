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

#include "WatchManager.h"
#include "Application.h"
#include "Utils.h"

#ifdef TRP_USE_WATCH
#if defined WIN32 || defined TRP_OSX || defined TRP_LINUX
    #include <efsw/efsw.hpp>
  #endif
#endif

#if defined TRP_OSX
#include <mach-o/dyld.h>	/* _NSGetExecutablePath */
#include <libgen.h>
#endif

WatchManager& WatchManager::Get()
{
	static WatchManager foo;
	return foo;
}

#ifdef TRP_USE_WATCH

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

class UpdateListener : public efsw::FileWatchListener
{
public:
	UpdateListener() {}


	void handleFileAction( efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = ""  )
	{
		std::string tempString;

		//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"%s %d\n",filename.c_str(),action);
		tempString = filename;
#ifdef TRP_OSX
		//TODO REMOVE THIS ANTIBUG OSX
		//tempString = filename.substr(11);
		tempString = basename((char*)filename.c_str());
#endif

		if (StartsWith(tempString,"save_")== false)
			{
#ifdef TRP_USE_NETWORK
				NetworkManager::Get().SendFileToAllClients(tempString);
			if (g_app->settings.autorestart)
				{
				NetworkManager::Get().SendMessageToAllClients("R>\n"); //Restart
				g_app->doneCode = DONECODE_RESTART_ONLY; //Restart
				}
#endif
			}
		//std::cout << "DIR (" << dir + ") FILE (" + filename + ") has event " << action << std::endl;
	}
};

#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

WatchManager::WatchManager()
{
#if defined WIN32 || defined TRP_OSX || defined TRP_LINUX
	fileWatcher = NULL;
	watcherListener = NULL;
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

WatchManager::~WatchManager()
	{
	}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WatchManager::Init()
{
	
#if defined WIN32 && defined TRP_USE_WATCH
	char pathbuf[512];
	fileWatcher = new efsw::FileWatcher();
	// add a watch to the system
	watcherListener = new UpdateListener();
	strcpy(pathbuf,"./");
	strcat(pathbuf,g_app->settings.gamedataURL.c_str());
	efsw::WatchID watchID = fileWatcher->addWatch(pathbuf,watcherListener);
#endif

#if  defined TRP_OSX
	char pathbuf[512];

	strcpy(pathbuf,g_app->settings_configURL.c_str());
	strcat(pathbuf,"/");
	strcat(pathbuf,g_app->settings_gamedataURL.c_str());
	
	SDL_Log("Path to watch %s\n",pathbuf);
	fileWatcher = new efsw::FileWatcher();
	// add a watch to the system
	watcherListener = new UpdateListener();
	efsw::WatchID watchID = fileWatcher->addWatch(pathbuf,watcherListener,true /*recursive*/);
	if (watchID == 0)
		{
		SDL_Log("Hum Hum cannot watch path %s ...",pathbuf);
		}
		
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WatchManager::Update(unsigned int _elapsed)
{
#if defined WIN32 || defined TRP_OSX || defined TRP_LINUX
	//fileWatcher->update(); //TODO Check where the update is done now.
#endif
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void WatchManager::Shutdown()
{
#ifdef TRP_USE_WATCH
	delete fileWatcher;
	delete watcherListener;
	fileWatcher = NULL;
#endif
}
