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

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Widget.h"
#include <string>
class Atlas;
class AtlasEntry;
class Sprite : public Widget
{
public:
	
	Sprite();
	Sprite(const Sprite &other);
	~Sprite();

	void operator= (const Sprite & other);
	
	void Load(Atlas * _atlas, const std::string& _name);
	void SetColor(unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);
	void Render();
	void UnLoad();
	void SetNinePatch(bool _value);
	void SetNinePatchRect(int _x,int _y,int _w,int _h);

public:

	Atlas			*atlas;
	AtlasEntry		*entry;

private:
	SDL_Color		color;
	bool			nine_patch;
	SDL_Rect		nine_rect;

};

Sprite		*Sprite_Factory();
void		RegisterSprite();


#endif