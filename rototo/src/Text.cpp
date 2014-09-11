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



#include "Text.h"
#include "Application.h"
/*
void ConstructCText(CText *thisPointer)
{
	new(thisPointer) CText();
}

void DestructCText(CText *thisPointer)
{
	thisPointer->~CText();
}


CText::CText()
{
	this->font = g_app->textManager->font;
	this->texture = NULL;
	SDL_Color tempColor = {255,255,255,255 };
	this->color = tempColor;
}

CText::~CText()
{
	if (this->texture != NULL)
		{
		SDL_DestroyTexture(this->texture);
        this->texture = NULL;
		}
}
void CText::UnLoad()
{
 	if (this->texture != NULL)
		{
		SDL_DestroyTexture(this->texture);
        this->texture = NULL;
		}
}
void CText::SetColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	SDL_Color tempColor = { _r, _g, _b, _a };
	color = tempColor;
}
void CText::Prepare(const std::string& _text)
{
	
	
	
	//SDL_Surface *text = TTF_RenderText_Solid(this->font, _text.c_str(), tempColor);
	//TTF_SetFontStyle(this->font,TTF_STYLE_ITALIC);
	SDL_Surface *text = TTF_RenderText_Blended(this->font, _text.c_str(), this->color);
	
	this->texture = SDL_CreateTextureFromSurface(g_app->sdlRenderer, text);
	this->frame.x = 0;
    this->frame.y = 0;
    this->frame.w = text->w;
    this->frame.h = text->h;
	this->position = this->frame;
	this->angle = 0;

	SDL_FreeSurface(text);
}
void CText::Render()
{
	CGameObject::Render();
	SDL_RenderCopyEx(g_app->sdlRenderer,this->texture , &this->frame, &this->position,this->angle,NULL,SDL_FLIP_NONE); 
}

*/