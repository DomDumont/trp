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

#ifndef __LABEL_H__
#define __LABEL_H__

#include "Widget.h"
#include "Color.h"

class Font;
struct SDL_Texture;
class Label : public Widget
{
	friend class Renderer;

public:
	Label();
	~Label();



	void Render();
	void SetFont(Font & _font);
	void SetColor(unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);
	void SetText(const std::string& _text,bool _justified = true);
	void SetEnabled(bool _value);
	void SetPosition(int _x,int _y);
	void SetScale(float _xFactor, float _yFactor);

private:



public:

	Font	*		font;
	SDL_Texture *		texture;

	Color		primary_text_color;
	Color		disable_text_color;
	Color		color_background;

	std::string 	text;
	bool			justified;
private:
	
	bool isDirty;


};

Label		*Label_Factory();
void		RegisterLabel();

#endif
