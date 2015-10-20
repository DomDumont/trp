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

#ifndef __RENDERER_H__
#define __RENDERER_H__

class Font;
class Color;
struct SDL_Texture; //TODO change this

class Renderer
{

public:
	static Renderer& Get();
	SDL_Texture * RenderText(Font * _font,const std::string& _text, Color _color);

private:
	Renderer();
};

#endif