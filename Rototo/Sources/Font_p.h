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

#ifndef __FONT_P_H__
#define __FONT_P_H__

#include "stb_truetype.h"

class SDL_Texture; //TODO remove this one

class Font_p
{
	friend class Renderer;

	SDL_Texture*      texture;      //!< font texture
	stbtt_bakedchar*  cdata;    //!< font data: ASCII 32..126 is 95 glyphs
	float             fontHeight;

public:
	Font_p();
	void GetTextExtent(const std::string& _text, float & _x, float &_y);
	float GetFontHeight();
	void Load(const std::string& _file, int _size, int _flags);
	void UnLoad();

};


#endif

