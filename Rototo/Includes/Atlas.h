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
struct SDL_Texture; //TODO change this
#include "Rect.h"

class AtlasEntry
{
	public:
	
	Atlas *		atlas;
	std::string	name;
	Rect		frame;

	AtlasEntry()
	{
		name = "";
		atlas = NULL;
		frame.x = 0;
		frame.y = 0;
		frame.w = 0;
		frame.h = 0;
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

	void AddRef();

	void Release();

	public:
	
	std::vector <AtlasEntry *>	atlasEntries;
	SDL_Texture *			texture;

	private:

	//Now in utils.cpp std::string LoadString(const std::string& _sourceFile);
	 
	Rect	size;
	int		refCount;
};

Atlas *Atlas_Factory();
void RegisterAtlas();

#endif
