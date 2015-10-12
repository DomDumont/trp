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
#include "TextBox.h"
#include "Application.h"
#include "Font.h"

#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextBox::TextBox()
{
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"TextBox Constructor\n");
	state = 0; cursor = 0; pan = 0;
	
	this->backup_text_color.r = 0;
	this->backup_text_color.g = 0;
	this->backup_text_color.b = 0;
	this->backup_text_color.a = 255;

	this->color.r = 0;
	this->color.g = 0;
	this->color.b = 0;
	this->color.a = 255;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextBox::~TextBox()
{
	
	MY_SAFE_RELEASE(this->label.font);
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::SetFont(Font & _font)
{
	
	MY_SAFE_RELEASE(this->label.font);
	this->label.font = &_font;
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::Render()
{
	if (this->shown == false)
		return;

	SDL_RenderFillRect(g_app->sdlRenderer,&this->position);
	this->label.Render();
	if(true)
		{
		state = 0;
		//draw a line 1 pixel wide
		SDL_SetRenderDrawColor(g_app->sdlRenderer,255,100,100,255);
		SDL_Rect tempRect;
		tempRect.x = label.position.x + label.position.w;
		tempRect.y = label.position.y;
		tempRect.w = 4;
		tempRect.h = label.position.h;
		SDL_RenderFillRect(g_app->sdlRenderer,&tempRect);
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::SetPosition(int _x,int _y)
{
	
	switch(this->anchor)
	{
	case ANCHOR_CENTER:
		{
		Widget::SetPosition(_x,_y);
	
		//Now position the text within the button
		int centerX = this->position.x + this->position.w/2;
		int centerY = this->position.y + this->position.h/2;
		label.SetPosition(centerX,centerY);
		}
	break;

	case ANCHOR_TOPLEFT:
		{
		Widget::SetPosition(_x,_y);
	
		//Now position the text within the button
		int centerX = this->position.x + this->position.w/2;
		int centerY = this->position.y + this->position.h/2;
		label.SetPosition(centerX,centerY);
		}

	break;

	}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::SetSize(int _w,int _h)
{
	Widget::SetSize(_w,_h);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::SetText(const std::string& _text)
{

	label.SetText(_text);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::SetTextColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	
	backup_text_color.r = _r;
	backup_text_color.g = _g;
	backup_text_color.b = _b;
	backup_text_color.a = _a;
	label.SetColor(_r,_g,_b,_a);
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int TextBox::OnMouseButtonDown( SDL_Event * event)
{
	
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"TextBox OnMouseButtonDown\n");
	SDL_StartTextInput();
	SDL_SetTextInputRect(&(this->position));
	
	return false;
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int TextBox::OnMouseButtonUp( SDL_Event * event)
{
	return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::OnMouseMotion( SDL_Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"TextBox OnMouseMotion\n");
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void TextBox::OnKeyUp( SDL_Event * event)
{

	char c = 0; int key = event->key.keysym.sym;
	
	if(key>=SDLK_a&&key<=SDLK_z)
		{
		//alphabet
		c = key-SDLK_a+'a';
		}
	else if(key>=SDLK_0&&key<=SDLK_9)
		{
		//digit.
		c = key-SDLK_0+'0';
		}
	else if(key==SDLK_DELETE)
		{
		//backspace. special.
		label.SetText(label.text.substr(0,label.text.size()-1));
		}
	if(c!=0)
		{
		//text = text.substr(0,s->cursor)+c+text.substr(s->cursor,text.size()-s->cursor);
		//s->cursor++;
		//s->setText(text);
		label.SetText(label.text + c);
		}
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

std::string TextBox::GetText(int _index)
{
		return label.text;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextBox *TextBox_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new TextBox();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterTextBox()
{
	int r;

#ifndef __EMSCRIPTEN__

	///class:TextBox
	r = g_app->scriptManager->engine->RegisterObjectType("TextBox", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_FACTORY, "TextBox@ f()", asFUNCTION(TextBox_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_ADDREF, "void f()", asMETHOD(TextBox,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_RELEASE, "void f()", asMETHOD(TextBox,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void Render()", asMETHOD(TextBox, Render), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetSize(int _w,int _h)", asMETHODPR(TextBox, SetSize,(int,int),void), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetPosition(int _x,int _y,int _from=0)", asMETHOD(TextBox, SetPosition), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetFont(Font @ _font)", asMETHOD(TextBox, SetFont), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetText(string &in _newText)", asMETHOD(TextBox, SetText), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(TextBox, SetTextColor), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","string GetText()", asMETHOD(TextBox, GetText), asCALL_THISCALL); SDL_assert( r >= 0 );
	///func:float GetRotation()
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","float get_Rotation()", asMETHOD(TextBox, GetRotation), asCALL_THISCALL); SDL_assert( r >= 0 );
	//g_app->scriptManager->RegisterClassMethod("TextBox","void Update(uint64 _elapsed)", asMETHOD(TextBox, Update));

#else
	r = g_app->scriptManager->engine->RegisterObjectType("TextBox", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_FACTORY, "TextBox@ f()", WRAP_FN(TextBox_Factory), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_ADDREF, "void f()", WRAP_MFN(TextBox,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_RELEASE, "void f()", WRAP_MFN(TextBox,Release), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void Render()", WRAP_MFN(TextBox, Render), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetSize(int _w,int _h)", WRAP_MFN_PR(TextBox, SetSize,(int,int),void), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetPosition(int _x,int _y,int _from=0)", WRAP_MFN(TextBox, SetPosition), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetFont(Font @ _font)", WRAP_MFN(TextBox, SetFont), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetText(string &in _newText)", WRAP_MFN(TextBox, SetText), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", WRAP_MFN(TextBox, SetTextColor), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","string GetText()", WRAP_MFN(TextBox, GetText), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectMethod("TextBox","float get_Rotation()", WRAP_MFN(TextBox, GetRotation), asCALL_GENERIC); SDL_assert( r >= 0 );

#endif
}
#endif
