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

#ifndef __TEXT_MANAGER_H__
#define __TEXT_MANAGER_H__

#include <map>
#include <string>

class TextManager
{
public:

	TextManager();
	~TextManager();

	void Init();
	void Shutdown();
	
	void Load(const std::string& _file,int _flags = 13 /*GAMEDATA|BOTH*/);
	std::string GetString(const std::string& _id);
	void UnLoad();

public:
	//TTF_Font *font;
	SDL_Color color;
	
private:
	std::map<std::string,std::string> mapStrings;
};


void RegisterTextManager();

#endif