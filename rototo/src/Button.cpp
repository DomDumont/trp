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

#include "Global.h"
#include "Button.h"
#include "Application.h"
#include "Font.h"


/************************************************************************/
/*                                                                      */
/************************************************************************/
Button *ButtonFactory()
{
    return new Button();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void RegisterButton()
{
    int r;

    ///class:Button
    r = g_app->scriptManager->engine->RegisterObjectType("Button", 0, asOBJ_REF); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_FACTORY, "Button@ f()", asFUNCTION(ButtonFactory), asCALL_CDECL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_ADDREF, "void f()", asMETHOD(Button,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_RELEASE, "void f()", asMETHOD(Button,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
    
    ///func:void SetText(string &in newText)
    g_app->scriptManager->RegisterClassMethod("Button","void SetText(string &in _newText)", asMETHOD(Button, SetText));
    ///func:void Render()
    g_app->scriptManager->RegisterClassMethod("Button","void Render()", asMETHOD(Button, Render));
    ///func:void SetSize(int w,int h)
    g_app->scriptManager->RegisterClassMethod("Button","void SetSize(int _w,int _h)", asMETHOD(Button, SetSize));
    ///func:void SetPosition(int x,int y,int from=0)
    g_app->scriptManager->RegisterClassMethod("Button","void SetPosition(int _x,int _y,int _from=0)", asMETHOD(Button, SetPosition));
    g_app->scriptManager->RegisterClassMethod("Button","void SetFont(Font @ _font)", asMETHOD(Button, SetFont));
    g_app->scriptManager->RegisterClassMethod("Button","void SetSprite(int _index,Atlas @ _atlas, string &in _name, bool _ninePatch = false)", asMETHOD(Button, SetSprite));
    g_app->scriptManager->RegisterClassMethod("Button","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Button, SetTextColor));
    g_app->scriptManager->RegisterClassMethod("Button","void SetRotation(float _angle)", asMETHOD(Button, SetRotation));
    g_app->scriptManager->RegisterClassMethod("Button","bool Touched(int _x,int _y)", asMETHOD(Button, Touched));
    g_app->scriptManager->RegisterClassMethod("Button","void SetScale(double _xFactor,double _yFactor)", asMETHOD(Button, SetScale));
    g_app->scriptManager->RegisterClassMethod("Button","void SetEnabled(bool _value)", asMETHOD(Button, SetEnabled));
    g_app->scriptManager->RegisterClassMethod("Button","void SetType(int _type)", asMETHOD(Button, SetType));
    g_app->scriptManager->RegisterClassMethod("Button","void SetState(int _state)", asMETHOD(Button, SetState));
    r = g_app->scriptManager->engine->RegisterObjectProperty("Button", "CallbackHandler @on_click_handler", asOFFSET(Button, on_click_handler)); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectProperty("Button", "ref @user_data", asOFFSET(Button, user_data)); SDL_assert( r >= 0 );
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
Button::Button() : on_click_handler(NULL),state(0),type(0)
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Button Constructor\n");
    this->user_data.Set(NULL,NULL);
    this->sender.Set(NULL,NULL);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
Button ::~Button()
{

    MY_SAFE_RELEASE(this->label.font);
    MY_SAFE_RELEASE(this->on_click_handler);

}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetSprite(int _index,Atlas * _atlas, const std::string& _name,bool _ninePatch)
{
    switch(_index)
    {
    case STATE_UP:
        this->sprite_up.Load(_atlas,_name);
        this->sprite_up.SetNinePatch(_ninePatch);
        this->sprite_up.position = this->position;
        this->sprite_up.angle = this->angle;
        break;

    case STATE_DOWN:
        this->sprite_down.Load(_atlas,_name);
        this->sprite_down.SetNinePatch(_ninePatch);
        this->sprite_down.position = this->position;
        this->sprite_down.angle = this->angle;
        break;

    case STATE_DISABLE:
        this->sprite_disable.Load(_atlas,_name);
        this->sprite_disable.SetNinePatch(_ninePatch);
        this->sprite_disable.position = this->position;
        this->sprite_disable.angle = this->angle;
        break;

    }
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetTextColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
    label.SetColor(_r,_g,_b,_a);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetText(const std::string& _newText)
{
    label.SetText(_newText);
    ApplyTheme();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetType(int _type)
{
    this->type = _type;
    ApplyTheme();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetState(int _state)
{
    this->state = _state;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::ApplyTheme()
{
switch(this->type)
    {
    case TYPE_BUTTON :
        if ((this->sprite_up.entry == NULL) && (g_app->guiManager->button_up != NULL))
                this->sprite_up = *(g_app->guiManager->button_up);

        if ((this->sprite_down.entry == NULL) && (g_app->guiManager->button_down != NULL))
            this->sprite_down = *(g_app->guiManager->button_down);

        if ((this->sprite_disable.entry == NULL) && (g_app->guiManager->button_disable != NULL))
                this->sprite_disable = *(g_app->guiManager->button_disable);
        break;
    case TYPE_CHECKBOX :
        if ((this->sprite_up.entry == NULL) && (g_app->guiManager->checkbox_up != NULL))
            this->sprite_up = *(g_app->guiManager->checkbox_up);

        if ((this->sprite_down.entry == NULL) && (g_app->guiManager->checkbox_down != NULL))
            this->sprite_down = *(g_app->guiManager->checkbox_down);

        if ((this->sprite_disable.entry == NULL) && (g_app->guiManager->checkbox_disable != NULL))
            this->sprite_disable = *(g_app->guiManager->checkbox_disable);
        break;

    default:
        SDL_assert(0);
        break;
    };
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::Render()
{
    if (this->shown == false)
        return;

    //if (this->type == TYPE_CHECKBOX)
    if (0)
        {
        SDL_SetRenderDrawColor(g_app->sdlRenderer,0,0,0,255);
        SDL_RenderFillRect(g_app->sdlRenderer,&this->position);
        }

    if (this->enabled)
        if (this->state == STATE_UP)
            this->sprite_up.Render();
        else
            this->sprite_down.Render();
    else
        this->sprite_disable.Render();


    this->label.Render();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetFont(Font & _font)
{
    MY_SAFE_RELEASE(this->label.font);
    this->label.font = &_font;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetRotation(float _angle)
{
    Widget::SetRotation(_angle);

    this->sprite_up.SetRotation(_angle);
    this->sprite_down.SetRotation(_angle);
    this->sprite_disable.SetRotation(_angle);
    this->label.SetRotation(_angle);
    
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
int Button::OnMouseButtonDown( SDL_Event * event)
{
    int ret;

    if (this->enabled == false)
        return false;

    if (this->type == TYPE_CHECKBOX)
        {
        if (this->state == STATE_UP)
            this->state = STATE_DOWN;
        else
            this->state = STATE_UP;
        }
    //Call Callback

    if (this->on_click_handler != NULL)
        {
        this->sender.Set(this,g_app->scriptManager->engine->GetObjectTypeByName("Button"));
        ret = g_app->scriptManager->RunCallback(this->on_click_handler,&(this->sender),&(this->user_data));
        this->sender.Set(NULL,NULL);
        return ret;
        }

    return false;
}


void Button::SetPosition(int _x,int _y,int _from)
{
    switch(_from)
    {
    case 0:
        {
        Widget::SetPosition(_x,_y,0);

        //Now position the sprite if any
        if (this->type == TYPE_BUTTON)
            {
            this->sprite_up.SetPosition(_x,_y,0);
            this->sprite_down.SetPosition(_x,_y,0);
            this->sprite_disable.SetPosition(_x,_y,0);
            }
        else
            {
	        this->sprite_up.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2),1);
	        this->sprite_down.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2),1);
	        this->sprite_disable.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2),1);
            }

        //Now position the text within the button

        if (this->type == TYPE_BUTTON)
            {
	        int centerX = this->position.x + this->position.w/2; 
	        int centerY = this->position.y + this->position.h/2; 
	        label.SetPosition(centerX,centerY,0);
            }
        else
            {
            int centerX = this->position.x + CHECKBOX_SIZE + 20; 
            int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
            label.SetPosition(centerX,centerY,1);
            }

        }
        break;

    case 1:
        {
	    Widget::SetPosition(_x,_y,1);
	    //Now position the sprite 
	    this->sprite_up.SetPosition(_x,_y,1);
        this->sprite_down.SetPosition(_x,_y,1);
        this->sprite_disable.SetPosition(_x,_y,1);

         //Now position the text within the button
        if (this->type == TYPE_BUTTON)
        {
            int centerX = this->position.x + this->position.w/2; 
            int centerY = this->position.y + this->position.h/2; 
            label.SetPosition(centerX,centerY,0);
        }
        else
        {
            int centerX = this->position.x + CHECKBOX_SIZE + 20; 
            int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
            label.SetPosition(centerX,centerY,1);
        }
        }
        break;
    }
}



/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetScale(double _xFactor,double _yFactor)
{
    Widget::SetScale(_xFactor,_yFactor);

	this->sprite_up.position = this->position;
	this->sprite_down.position = this->position;
	this->sprite_disable.position = this->position;
    if (this->type == TYPE_CHECKBOX)
        {
        this->sprite_up.position.w = CHECKBOX_SIZE;
        this->sprite_down.position.w = CHECKBOX_SIZE;
        this->sprite_disable.position.w = CHECKBOX_SIZE;
        this->sprite_up.position.h = CHECKBOX_SIZE;
        this->sprite_down.position.h = CHECKBOX_SIZE;
        this->sprite_disable.position.h = CHECKBOX_SIZE;

        this->sprite_up.position.y += ((this->position.h-CHECKBOX_SIZE) /2);
        this->sprite_down.position.y += ((this->position.h-CHECKBOX_SIZE) /2);
        this->sprite_disable.position.y += ((this->position.h-CHECKBOX_SIZE) /2);

        }
    label.SetScale(_xFactor, _yFactor);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetSize(int _w,int _h)
{
    //Special for button, set the frame also

    this->frame.w = _w;
    this->frame.h = _h;

    Widget::SetSize(_w,_h);
    //Now reposition the sprite

    this->sprite_up.SetSize(_w/10,_h/2);
    this->sprite_down.SetSize(_w/10,_h/2);
    this->sprite_disable.SetSize(_w/10,_h/2);

    //Now reposition the text because size changed
    if (this->type == TYPE_BUTTON)
    {
        int centerX = this->position.x + this->position.w/2; 
        int centerY = this->position.y + this->position.h/2; 
        label.SetPosition(centerX,centerY,0);
    }
    else
    {
        int centerX = this->position.x + CHECKBOX_SIZE + 20; 
        int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
        label.SetPosition(centerX,centerY,1);
    }
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void Button::SetEnabled(bool _value)
{
    Widget::SetEnabled(_value);    
    this->label.SetEnabled(_value);
}
