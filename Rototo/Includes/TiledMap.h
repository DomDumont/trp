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

#ifndef __TILED_MAP_H__
#define __TILED_MAP_H__

#include "Widget.h"
#include <string>



class Atlas;
class AtlasEntry;

class TiledMap : public Widget
{
public:
	
	TiledMap();
	~TiledMap();
    void Load(const std::string& _fullPath);
    void UnLoad();
	void Render();

	
	
private:

	int tileOffsetX;
	int tileOffsetY;
	int tileWidth;
	int tileHeight;
	int	width; //TODO put in a layer
	int	height; //TODO put in a layer

	std::string tilesetPath;
	Atlas * atlas;

};

TiledMap *TiledMap_Factory();

void RegisterTiledMap();
/*
void ConstructSprite(Sprite *thisPointer);
void DestructSprite(Sprite *thisPointer);
*/

#endif
