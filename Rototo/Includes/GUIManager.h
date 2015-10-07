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


#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "Global.h" //TODO ne devrait pas être là trop gros include.
#include <string>
#include <vector>
#include "Widget.h"

class Font;
class Sprite;
class Atlas;


class GUIManager
{

public:

	GUIManager	();
	~GUIManager	();
	
	void Init		();
	void Shutdown		();
	void HandleEvent	( SDL_Event * event);
	void AddWidget		(Widget *_widget);
	void RemoveWidget	(Widget *_widget);
	void LoadTheme		(const std::string& _file);
	void UnLoadTheme	();

private:
	Sprite *  LoadSprite	(void* _elem);

public:
	Font			*font;
	Atlas			*atlas;

	Sprite			*button_up;
	Sprite			*button_down;
	Sprite			*button_disable;
	Sprite			*list_normal_item;
	Sprite			*list_selected_item;
	
	Sprite			*checkbox_up;
	Sprite			*checkbox_down;
	Sprite			*checkbox_disable;

	Sprite			*radiobox_up;
	Sprite			*radiobox_down;
	Sprite			*radiobox_disable;

	SDL_Color		primary_text_color;
	SDL_Color		disable_text_color;
	SDL_Color		background_color;

private:

	std::vector<Widget*>			widgets;
	std::vector<Widget *>::iterator		widgetsIT;

};

void GUI_AddWidget(Widget *_widget);
void GUI_RemoveWidget(Widget *_widget);
void GUI_LoadTheme(const std::string& _file);
void GUI_UnLoadTheme();

void RegisterGUI();

#endif
