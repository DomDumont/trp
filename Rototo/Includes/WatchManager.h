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

#ifndef __WATCH_MANAGER_H__
#define __WATCH_MANAGER_H__

//#if defined WIN32 || defined TRP_OSX
//#include <efsw/efsw.hpp>
//#endif
namespace efsw
{
    class FileWatcher;
    class FileWatchListener;
}

class WatchManager
{
public:
	static WatchManager& Get();
	
	~WatchManager();

	void Init();
	void Update(unsigned int _elapsed);
	void Shutdown();

private:
	WatchManager();
#if defined WIN32 || defined TRP_OSX || defined TRP_LINUX
	efsw::FileWatcher *fileWatcher;
	efsw::FileWatchListener* watcherListener;
#endif
};

#endif
