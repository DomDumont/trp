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

#ifndef __LISTBOX_H__
#define __LISTBOX_H__

#include "Widget.h"
#include <vector>
#include "Label.h"
#include <angelscript.h>
#include "scripthandle/scripthandle.h"
#include "Sprite.h"

class ListBox : public Widget
{
public:
	ListBox();
	~ListBox();


	void Render();
	void SetPosition(int _x,int _y,int _from = 0);
	void SetSize(int _w,int _h);
	int  AddItem(const std::string& _text);
    	void RemoveItem(int _index);
    	void ResetContent();
	void SetFont(Font & _font);
    	void SetBackgroundColor(int _r,int _g,int _b,int _a);
    	void SetItemColor(int _r,int _g,int _b,int _a);
    	void SetSelectedItemColor(int _r,int _g,int _b,int _a);
    	void SetTextColor(int _r,int _g,int _b,int _a);
	

	int OnMouseButtonDown( SDL_Event * event);
	int OnMouseButtonUp( SDL_Event * event);
	void OnMouseMotion( SDL_Event * event);

	int  GetSelectedIndex();
	std::string GetItemText(int _index);

public:
	asIScriptFunction *	onSelectionChangedHandler;
	CScriptHandle		userData;
	CScriptHandle		sender;
private:
	void BuildInternalTexture();
	SDL_Texture *bgTexture;
	int offsetBG;
	int sizeXBG;
	int sizeYBG;
	int sizeItemBG;
private:
	//int refCount;
	SDL_Color			backgroundColor;
    	SDL_Color			itemColor;
    	SDL_Color			selectedItemColor;
    	SDL_Color			textColor;

    	Sprite                  	sprite_selected;
    	Sprite                  	sprite_item;

	int				dragState;
	Font	*			font;
	std::vector<Label*> 		labels; 
	std::vector<Label *>::iterator 	labelsIT;
	int 				selectedIndex;
	

};


ListBox *ListBox_Factory();
void RegisterListBox();

#endif
