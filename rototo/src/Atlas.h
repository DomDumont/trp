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

#ifndef __ATLAS_H__
#define __ATLAS_H__

#include <vector>
#include <string>

class Atlas;

class AtlasEntry
{
	public:
	
	Atlas *		atlas;
	std::string	name;
	SDL_Rect	frame;

	AtlasEntry()
	{
		name = "";
		atlas = NULL;
	}

};

class Atlas
{
	public:
	
	Atlas();
	~Atlas();

	void Load(const std::string& _file,int _flags = 13 /*GAMEDATA|BOTH*/);
	void LoadFromImage(const std::string& _file,int _flags = 13 /*GAMEDATA|BOTH*/);
	void UnLoad();
	AtlasEntry * FindEntry(const std::string& _name);

	void AddRef()
	{
		// Increase the reference counter
		refCount++;
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas 0x%x AddRef : nb active ref = %d\n",this,refCount);
	}

	void Release()
	{
		// Decrease ref count and delete if it reaches 0
		refCount--;
		if ( refCount == 0 )
			delete this;
		else
		if ( refCount > 0 )
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas 0x%x Release : nb active ref = %d\n",this,refCount);
		else
			SDL_assert(0);
			
	}

	public:
	
	std::vector <AtlasEntry *>	atlasEntries;
	SDL_Texture *			texture;

	private:

	//Now in utils.cpp std::string LoadString(const std::string& _sourceFile);
	 
	SDL_Rect	size;
	int		refCount;
};

Atlas *Atlas_Factory();
void RegisterAtlas();

#endif
