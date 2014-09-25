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

void RegisterTextBox()
{
	int r;
	///class:TextBox
	r = g_app->scriptManager->engine->RegisterObjectType("TextBox", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_FACTORY, "TextBox@ f()", asFUNCTION(TextBox_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_ADDREF, "void f()", asMETHOD(TextBox,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TextBox", asBEHAVE_RELEASE, "void f()", asMETHOD(TextBox,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	g_app->scriptManager->RegisterClassMethod("TextBox","void Render()", asMETHOD(TextBox, Render));
	g_app->scriptManager->RegisterClassMethod("TextBox","void SetSize(int _w,int _h)", asMETHOD(TextBox, SetSize));
	g_app->scriptManager->RegisterClassMethod("TextBox","void SetPosition(int _x,int _y,int _from=0)", asMETHOD(TextBox, SetPosition));
	g_app->scriptManager->RegisterClassMethod("TextBox","void SetFont(Font @ _font)", asMETHOD(TextBox, SetFont));
	g_app->scriptManager->RegisterClassMethod("TextBox","void SetText(string &in _newText)", asMETHOD(TextBox, SetText));
	g_app->scriptManager->RegisterClassMethod("TextBox","void SetTextColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(TextBox, SetTextColor));
	g_app->scriptManager->RegisterClassMethod("TextBox","string GetText()", asMETHOD(TextBox, GetText));
	///func:double GetRotation()
	g_app->scriptManager->RegisterClassMethod("TextBox","double GetRotation()", asMETHOD(TextBox, GetRotation));
	//g_app->scriptManager->RegisterClassMethod("TextBox","void Update(uint64 _elapsed)", asMETHOD(TextBox, Update));
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TextBox::TextBox()
{
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"TextBox Constructor\n");
	state = 0; cursor = 0; pan = 0;

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

void TextBox::SetPosition(int _x,int _y,int _from)
{
	
	switch(_from)
	{
	case 0:
		{
		Widget::SetPosition(_x,_y,0);
	
		//Now position the text within the button
		int centerX = this->position.x + this->position.w/2;
		int centerY = this->position.y + this->position.h/2;
		label.SetPosition(centerX,centerY,0);
		}
	break;

	case 1:
		{
		Widget::SetPosition(_x,_y,1);
	
		//Now position the text within the button
		int centerX = this->position.x + this->position.w/2;
		int centerY = this->position.y + this->position.h/2;
		label.SetPosition(centerX,centerY,0);
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

void TextBox::OnMouseButtonUp( SDL_Event * event)
{
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
