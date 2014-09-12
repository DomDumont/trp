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

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Widget.h"

#include "Label.h"
#include "Sprite.h"

#include <angelscript.h>
#include "scripthandle/scripthandle.h"

#define STATE_UP        0
#define STATE_DOWN      1
#define STATE_DISABLE   2

#define TYPE_BUTTON     0
#define TYPE_CHECKBOX   1
#define TYPE_ONOFF      2

#define CHECKBOX_SIZE   20  


class Button : public Widget
{
 public:

    Button                  ();
    ~Button                 ();

    void Render             ();
    void SetText            (const std::string& _newText);
    void SetFont            (Font & _font);
    void ApplyTheme         ();
    void SetSprite          (int _index,Atlas * _atlas, const std::string& _name, bool _ninePatch = false);
    void SetPosition        (int _x,int _y,int _from = 0);
    void SetSize            (int _w,int _h);
    void SetTextColor       (unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);
    void SetRotation        (float _angle);
    void SetScale           (double _xFactor,double _yFactor);
    void SetEnabled         (bool _value);
    int  OnMouseButtonDown  ( SDL_Event * event);
    void SetType            (int _type);
    void SetState           (int _state);

 public:

    asIScriptFunction *     on_click_handler;
    CScriptHandle           user_data;
    CScriptHandle           sender;


 private:

    Label                   label;
    Sprite                  sprite_up;
    Sprite                  sprite_down;
    Sprite                  sprite_disable;

    int                     state;
    int                     type;



};

Button *ButtonFactory();
void    RegisterButton();

#endif