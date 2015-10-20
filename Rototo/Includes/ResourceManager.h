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


#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "Global.h" //TODO ne devrait pas être là trop gros include.
#include <string>

enum ResourceManagerFlags
{
    GAMEDATA   = 1,
    FULLPATH   = 2,
    INTERNAL   = 4,
    EXTERNAL   = 8,
    BOTH       = 12 // Both internal & external
};

class SDL_RWops; //TODO certainly remove this one also

class ResourceManager
{
public:
	static ResourceManager& Get();
	
	~ResourceManager       ();
    
	void        Init        ();
	void        Shutdown    ();
    
	SDL_RWops*  Load        (const std::string & _file, unsigned int _flags);
	SDL_RWops*  Save        (const std::string & _file, unsigned int _flags);
private:
	ResourceManager();

	
};

#endif
