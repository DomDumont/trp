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
#include "Button_p.h"
#include "Application.h"
#include "Font.h"
#include "Vector2D.h"

#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Button *ButtonFactory()
{
	return new Button();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterButton()
{
	int r;

#ifndef __EMSCRIPTEN__

	///class:Button
	r = g_app->scriptManager->engine->RegisterObjectType("Button", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_FACTORY, "Button@ f()", asFUNCTION(ButtonFactory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_ADDREF, "void f()", asMETHOD(Button,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_RELEASE, "void f()", asMETHOD(Button,Release), asCALL_THISCALL); SDL_assert( r >= 0 );

	///func:void SetText(string &in newText)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetText(string &in _newText)", asMETHOD(Button, SetText), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void Render()
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void Render()", asMETHOD(Button, Render), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetSize(int w,int h)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetSize(int _w,int _h)", asMETHODPR(Button, SetSize,(int,int),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void set_Position(Vector2D vec)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Size(Vector2D _vec)", asMETHODPR(Button, SetSize,(Vector2D),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetPosition(int x,int y)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetPosition(int _x,int _y)", asMETHODPR(Button, SetPosition,(int,int),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void set_Position(Vector2D vec)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Position(Vector2D _vec)", asMETHODPR(Button, SetPosition,(Vector2D),void), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetFont(Font @ font)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetFont(Font @ _font)", asMETHOD(Button, SetFont), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetSprite(int index,Atlas @ atlas, string &in name, bool ninePatch = false)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetSprite(int _index,Atlas @ _atlas, string &in _name, bool _ninePatch = false)", asMETHOD(Button, SetSprite), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetTextColor(uint8 r=255,uint8 g=255,uint8 b=255,uint8 a=255)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Button, SetTextColor), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetRotation(float angle)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Rotation(float _angle)", asMETHOD(Button, SetRotation), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:float get_Rotation()
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","float get_Rotation()", asMETHOD(Button, GetRotation), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:bool Touched(int x,int y)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","bool Touched(int _x,int _y)", asMETHOD(Button, Touched), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetScale(double xFactor,double yFactor)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetScale(double _xFactor,double _yFactor)", asMETHOD(Button, SetScale), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetEnabled(bool value)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetEnabled(bool _value)", asMETHOD(Button, SetEnabled), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetType(int type)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetType(int _type)", asMETHOD(Button, SetType), asCALL_THISCALL);SDL_assert( r >= 0 );
	///func:void SetState(int state)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetState(int _state)", asMETHOD(Button, SetState), asCALL_THISCALL);SDL_assert( r >= 0 );

	///prop:CallbackHandler @on_click_handler
	g_app->scriptManager->engine->RegisterObjectProperty("Button", "CallbackHandler @on_click_handler", asOFFSET(Button, on_click_handler_script));
	///prop:ref @user_data
	g_app->scriptManager->engine->RegisterObjectProperty("Button", "ref @user_data", asOFFSET(Button, user_data_script));
#else

	///class:Button
	r = g_app->scriptManager->engine->RegisterObjectType("Button", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_FACTORY, "Button@ f()", WRAP_FN(ButtonFactory), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Button,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Button", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Button,Release), asCALL_GENERIC); SDL_assert( r >= 0 );

	///func:void SetText(string &in newText)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetText(string &in _newText)", WRAP_MFN(Button, SetText), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void Render()
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void Render()", WRAP_MFN(Button, Render), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetSize(int w,int h)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetSize(int _w,int _h)", WRAP_MFN_PR(Button, SetSize,(int,int),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void set_Position(Vector2D vec)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Size(Vector2D _vec)", WRAP_MFN_PR(Button, SetSize,(Vector2D),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetPosition(int x,int y)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetPosition(int _x,int _y)", WRAP_MFN_PR(Button, SetPosition,(int,int),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void set_Position(Vector2D vec)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Position(Vector2D _vec)", WRAP_MFN_PR(Button, SetPosition,(Vector2D),void), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetFont(Font @ font)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetFont(Font @ _font)", WRAP_MFN(Button, SetFont), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetSprite(int index,Atlas @ atlas, string &in name, bool ninePatch = false)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetSprite(int _index,Atlas @ _atlas, string &in _name, bool _ninePatch = false)", WRAP_MFN(Button, SetSprite), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetTextColor(uint8 r=255,uint8 g=255,uint8 b=255,uint8 a=255)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", WRAP_MFN(Button, SetTextColor), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetRotation(float angle)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void set_Rotation(float _angle)", WRAP_MFN(Button, SetRotation), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:float get_Rotation()
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","float get_Rotation()", WRAP_MFN(Button, GetRotation), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:bool Touched(int x,int y)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","bool Touched(int _x,int _y)", WRAP_MFN(Button, Touched), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetScale(double xFactor,double yFactor)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetScale(double _xFactor,double _yFactor)", WRAP_MFN(Button, SetScale), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetEnabled(bool value)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetEnabled(bool _value)", WRAP_MFN(Button, SetEnabled), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetType(int type)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetType(int _type)", WRAP_MFN(Button, SetType), asCALL_GENERIC);SDL_assert( r >= 0 );
	///func:void SetState(int state)
	r = g_app->scriptManager->engine->RegisterObjectMethod("Button","void SetState(int _state)", WRAP_MFN(Button, SetState), asCALL_GENERIC);SDL_assert( r >= 0 );

	///prop:CallbackHandler @on_click_handler
	g_app->scriptManager->engine->RegisterObjectProperty("Button", "CallbackHandler @on_click_handler", asOFFSET(Button, on_click_handler_script));
	///prop:ref @user_data
	g_app->scriptManager->engine->RegisterObjectProperty("Button", "ref @user_data", asOFFSET(Button, user_data_script));

#endif	
}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Button::Button() : state(0),type(0)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Button Constructor\n");
#ifdef TRP_USE_BINDING
	this->user_data_script.Set(NULL,NULL);
	this->sender_script.Set(NULL,NULL);
	on_click_handler_script = NULL;
#endif

	this->user_data = NULL;
	this->sender = NULL;
  	this->on_click_handler = NULL;

	this->SetScale(1,1);
	this->sprite_up.SetScale(1,1);
	this->sprite_down.SetScale(1,1);
	this->sprite_disable.SetScale(1,1);
	this->label.SetScale(1,1);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Button::~Button()
{

	MY_SAFE_RELEASE(this->label.font);
#ifdef TRP_USE_BINDING
	MY_SAFE_RELEASE(this->on_click_handler_script);
#endif
	this->on_click_handler = NULL;
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetUserData(void * userdata)
{
	this->user_data = userdata;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetSender(void * sender)
{
	this->sender = sender;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetClickHandler(on_click_handler_type handler)
{
	this->on_click_handler = handler;
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetTextColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	label.SetColor(_r,_g,_b,_a);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetText(const std::string& _newText)
{
	label.SetText(_newText);
	ApplyTheme();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetType(int _type)
{
	this->type = _type;
	ApplyTheme();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetState(int _state)
{
	this->state = _state;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

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

	case TYPE_RADIOBOX :
		if ((this->sprite_up.entry == NULL) && (g_app->guiManager->radiobox_up != NULL))
			this->sprite_up = *(g_app->guiManager->radiobox_up);
		
		if ((this->sprite_down.entry == NULL) && (g_app->guiManager->radiobox_down != NULL))
			this->sprite_down = *(g_app->guiManager->radiobox_down);
		
		if ((this->sprite_disable.entry == NULL) && (g_app->guiManager->radiobox_disable != NULL))
			this->sprite_disable = *(g_app->guiManager->radiobox_disable);
		break;
	default:
		SDL_assert(0);
		break;
	};
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetFont(Font & _font)
{
	MY_SAFE_RELEASE(this->label.font);
	this->label.font = &_font;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetRotation(float _angle)
{
	Widget::SetRotation(_angle);

	this->sprite_up.SetRotation(_angle);
	this->sprite_down.SetRotation(_angle);
	this->sprite_disable.SetRotation(_angle);
	this->label.SetRotation(_angle);
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Button::OnMouseButtonDown( SDL_Event * event)
{
	

	if (this->enabled == false)
		return false;

	if (this->type == TYPE_CHECKBOX)
		{
		if (this->state == STATE_UP)
			this->state = STATE_DOWN;
		else
			this->state = STATE_UP;
		}
	else
	if (this->type == TYPE_RADIOBOX)
		{
		if (this->state == STATE_UP)
			this->state = STATE_DOWN;
		else
			this->state = STATE_UP;
		}

#ifdef TRP_USE_BINDING
	//Call Callback
	if (this->on_click_handler_script != NULL)
		{
		this->sender_script.Set(this,g_app->scriptManager->engine->GetObjectTypeByName("Button"));
		int ret = g_app->scriptManager->RunCallback(this->on_click_handler_script,&(this->sender_script),&(this->user_data_script));
		this->sender_script.Set(NULL,NULL);
		return ret;
		}
#endif
	if (this->on_click_handler != NULL)
		{
		bool ret = this->on_click_handler(this->sender,this->user_data);
		return ret;
		}		


	return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetPosition(Vector2D _pos)
{
	this->SetPosition((int)_pos.x,(int) _pos.y);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetPosition(int _x,int _y)
{
	switch(this->anchor)
	{
	case ANCHOR_CENTER:
		{
		Widget::SetPosition(_x,_y);

		//Now position the sprite if any
		if (this->type == TYPE_BUTTON)
			{
			this->sprite_up.SetPosition(_x,_y);
			this->sprite_down.SetPosition(_x,_y);
			this->sprite_disable.SetPosition(_x,_y);
			}
		else
		if (this->type == TYPE_CHECKBOX)
			{
			this->sprite_up.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_up.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2));
			this->sprite_down.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_down.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2));
			this->sprite_disable.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_disable.SetPosition(_x,_y+((this->position.h-CHECKBOX_SIZE)/2));
			}
		else
		if (this->type == TYPE_RADIOBOX)
			{
			this->sprite_up.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_up.SetPosition(_x,_y+((this->position.h-RADIOBOX_SIZE)/2));
			this->sprite_down.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_down.SetPosition(_x,_y+((this->position.h-RADIOBOX_SIZE)/2));
			this->sprite_disable.SetAnchor(ANCHOR_TOPLEFT);
			this->sprite_disable.SetPosition(_x,_y+((this->position.h-RADIOBOX_SIZE)/2));
			}

		//Now position the text within the button

		if (this->type == TYPE_BUTTON)
			{
			int centerX = this->position.x + this->position.w/2;
			int centerY = this->position.y + this->position.h/2;
			label.SetPosition(centerX,centerY);
			}
		else
		if (this->type == TYPE_CHECKBOX)
			{
			int centerX = this->position.x + CHECKBOX_SIZE + 20;
			int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
			label.SetAnchor(ANCHOR_TOPLEFT);
			label.SetPosition(centerX,centerY);
			}
		else
		if (this->type == TYPE_RADIOBOX)
			{
			int centerX = this->position.x + RADIOBOX_SIZE + 20;
			int centerY = this->position.y + ((this->position.h-RADIOBOX_SIZE) /4);
			label.SetAnchor(ANCHOR_TOPLEFT);
			label.SetPosition(centerX,centerY);
			}

		}
		break;

	case ANCHOR_TOPLEFT:
		{
		Widget::SetPosition(_x,_y);
		//Now position the sprite
		this->sprite_up.SetPosition(_x,_y);
		this->sprite_down.SetPosition(_x,_y);
		this->sprite_disable.SetPosition(_x,_y);

		//Now position the text within the button
		if (this->type == TYPE_BUTTON)
			{
			int centerX = this->position.x + this->position.w/2;
			int centerY = this->position.y + this->position.h/2;
			label.SetPosition(centerX,centerY);
			}
		else
		if (this->type == TYPE_CHECKBOX)
			{
			int centerX = this->position.x + CHECKBOX_SIZE + 20;
			int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
			label.SetPosition(centerX,centerY);
			}
		else
		if (this->type == TYPE_RADIOBOX)
			{
			int centerX = this->position.x + RADIOBOX_SIZE + 20;
			int centerY = this->position.y + ((this->position.h-RADIOBOX_SIZE) /4);
			label.SetPosition(centerX,centerY);
			}

		}
		break;
	}
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetScale(double _xFactor,double _yFactor)
{
	Widget::SetScale(_xFactor,_yFactor);

	this->sprite_up.SetScale(_xFactor,_yFactor);
	this->sprite_down.SetScale(_xFactor,_yFactor);
	this->sprite_disable.SetScale(_xFactor,_yFactor);

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
	else
	if (this->type == TYPE_RADIOBOX)
		{
		this->sprite_up.position.w = RADIOBOX_SIZE;
		this->sprite_down.position.w = RADIOBOX_SIZE;
		this->sprite_disable.position.w = RADIOBOX_SIZE;
		this->sprite_up.position.h = RADIOBOX_SIZE;
		this->sprite_down.position.h = RADIOBOX_SIZE;
		this->sprite_disable.position.h = RADIOBOX_SIZE;
		
		this->sprite_up.position.y += ((this->position.h-RADIOBOX_SIZE) /2);
		this->sprite_down.position.y += ((this->position.h-RADIOBOX_SIZE) /2);
		this->sprite_disable.position.y += ((this->position.h-RADIOBOX_SIZE) /2);
		
		}
	label.SetScale(_xFactor, _yFactor);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetSize(Vector2D _pos)
{
	this->SetSize((int)_pos.x,(int) _pos.y);	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetSize(int _w,int _h)
{
	//Special for button, set the frame also

	this->frame.w = _w;
	this->frame.h = _h;

	Widget::SetSize(_w,_h);
	//Now reposition the sprite

	this->sprite_up.SetSize(_w,_h);
	this->sprite_down.SetSize(_w,_h);
	this->sprite_disable.SetSize(_w,_h);

	//Now reposition the text because size changed
	if (this->type == TYPE_BUTTON)
		{
		int centerX = this->position.x + this->position.w/2;
		int centerY = this->position.y + this->position.h/2;
		label.SetPosition(centerX,centerY);
		}
	else
	if (this->type == TYPE_CHECKBOX)
		{
		int centerX = this->position.x + CHECKBOX_SIZE + 20;
		int centerY = this->position.y + ((this->position.h-CHECKBOX_SIZE) /4);
		label.SetPosition(centerX,centerY);
		}
	else
	if (this->type == TYPE_RADIOBOX)
		{
		int centerX = this->position.x + RADIOBOX_SIZE + 20;
		int centerY = this->position.y + ((this->position.h-RADIOBOX_SIZE) /4);
		label.SetPosition(centerX,centerY);
		}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Button::SetEnabled(bool _value)
{
	Widget::SetEnabled(_value);
	this->label.SetEnabled(_value);
}
