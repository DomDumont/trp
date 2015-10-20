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

#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include "Widget.h"
#include <vector>
#include "Label.h"
#include "Color.h"

class Event;

class TextBox : public Widget
{
public:
	TextBox();
	~TextBox();


	void Render();
	void SetPosition(int _x,int _y);
	void SetSize(int _w,int _h);
	void SetText(const std::string& _text);
	void SetFont(Font & _font);
    void SetTextColor(unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);
	

	int OnMouseButtonDown( Event * event);
	int OnMouseButtonUp( Event * event);
	void OnMouseMotion( Event * event);
    void OnKeyUp( Event * event);

	std::string GetText(int _index);

private:
	Color	color;
	Color	backup_text_color;
	Label label; 
	int state;
	int cursor;
	int pan;


};


TextBox *TextBox_Factory();
void RegisterTextBox();

#endif
