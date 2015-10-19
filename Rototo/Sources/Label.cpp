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
#include "Label.h"
#include "Application.h"
#include "Font.h"
#include "Renderer.h"
#include "GUIManager.h"

#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Label *Label_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Label();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Label::SetScale(float _xFactor, float _yFactor)
{
	Widget::SetScale(_xFactor,_yFactor);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


void Label::SetPosition(int _x,int _y)
{
	Widget::SetPosition(_x,_y);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterLabel()
{
	int r;
	
	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		///class:Label
		r = ScriptManager::Get().engine->RegisterObjectType("Label", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_FACTORY, "Label@ f()", asFUNCTION(Label_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_ADDREF, "void f()", asMETHOD(Label, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_RELEASE, "void f()", asMETHOD(Label, Release), asCALL_THISCALL); SDL_assert(r >= 0);

		///func:void SetText(string &in _newText,bool _justified = true)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetText(string &in _newText,bool _justified = true)", asMETHOD(Label, SetText), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void Render()
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void Render()", asMETHOD(Label, Render), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetFont(Font @ font)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetFont(Font @ _font)", asMETHOD(Label, SetFont), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Label, SetColor), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetPosition(int x,int y)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetPosition(int _x,int _y)", asMETHODPR(Label, SetPosition, (int, int), void), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:bool Touched(int _x,int _y)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "bool Touched(int _x,int _y)", asMETHOD(Label, Touched), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetRotation(float angle)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void set_Rotation(float _angle)", asMETHOD(Label, SetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:float GetRotation()
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "float get_Rotation()", asMETHOD(Label, GetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
	}
	else
	{

		r = ScriptManager::Get().engine->RegisterObjectType("Label", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_FACTORY, "Label@ f()", WRAP_FN(Label_Factory), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Label, AddRef), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Label", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Label, Release), asCALL_GENERIC); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetText(string &in _newText,bool _justified = true)", WRAP_MFN(Label, SetText), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void Render()", WRAP_MFN(Label, Render), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetFont(Font @ _font)", WRAP_MFN(Label, SetFont), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", WRAP_MFN(Label, SetColor), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void SetPosition(int _x,int _y)", WRAP_MFN_PR(Label, SetPosition, (int, int), void), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "bool Touched(int _x,int _y)", WRAP_MFN(Label, Touched), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "void set_Rotation(float _angle)", WRAP_MFN(Label, SetRotation), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Label", "float get_Rotation()", WRAP_MFN(Label, GetRotation), asCALL_GENERIC); SDL_assert(r >= 0);

	}

}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Label::Label() : font(NULL),texture(NULL)
{
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Label Constructor\n");

	//TODO change this magic number 253 for alpha
	SDL_Color tempColor = {255,255,255,253 };
	this->primary_text_color = tempColor;
	this->disable_text_color = tempColor;
	this->color_background.r = 0;
	this->color_background.b = 0;
	this->color_background.g = 0;
	this->color_background.a = 253;
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Label::~Label()
{

	MY_SAFE_RELEASE(this->font);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetFont(Font & _font)
{

	MY_SAFE_RELEASE(this->font);
	this->font  = &_font;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	SDL_Color tempColor = { _r, _g, _b, _a };
	this->primary_text_color = tempColor;
	if (this->texture)
		SDL_SetTextureAlphaMod(this->texture,_a);

	SetText(this->text);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::BuildInternalTexture(const std::string& _text,bool _justified)
{
	SDL_Color tempColor;
	std::string text = _text;
	std::size_t len = text.size();
	std::vector<SDL_Surface*> lines;
	int width = 0;
	int height = 0;

	if (this->enabled)
		tempColor = this->primary_text_color;
	else
		tempColor = this->disable_text_color;

  this->texture = 	Renderer::Get().RenderText(this->font,_text, tempColor);

  this->frame.x = 0;
  this->frame.y = 0;
  SDL_QueryTexture(this->texture, NULL, NULL, &(this->frame.w), &(this->frame.h));

  SDL_SetTextureAlphaMod(this->texture, tempColor.a);
  SDL_SetTextureBlendMode(this->texture, SDL_BLENDMODE_BLEND);


  /*
	if (len > 0)
		{
		for (std::size_t off = 0; off != std::string::npos;)
			{
			std::size_t next_off = text.find("\n");
			if (next_off != std::string::npos)
				{
				text[next_off] = '\0';
				++next_off;
				if (next_off >= len)
					next_off = std::string::npos;
				}
			SDL_Surface* line;
		
      
      line = this->font->Render(&text[off], tempColor);

			if (line)
				{
				SDL_SetSurfaceBlendMode(line, SDL_BLENDMODE_NONE);
				//SDL_SetSurfaceBlendMode(line, SDL_BLENDMODE_BLEND);
				//SDL_SetSurfaceAlphaMod(line,255);
					
				if (line->w > width)
					width = line->w;
				height += line->h;
				}
			lines.push_back(line);
			off = next_off;
			}
		} //Len > 0
	
	
	SDL_Surface* surface;


	
	if (lines.empty())
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	else
		surface = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	
	if (surface)
		{
		SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
		//SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 0, 0, 0,0));
		//SDL_SetSurfaceAlphaMod(surface,this->color.a);
		
		SDL_Rect dstrect = {0};
		std::vector<SDL_Surface*>::const_iterator it = lines.begin();
		std::vector<SDL_Surface*>::const_iterator end = lines.end();
		while (it != end)
			{
			SDL_Surface* line = *it;
      if (line != NULL)
        {
        if (_justified)
          dstrect.x = (width - line->w) / 2;
        SDL_BlitSurface(line, NULL, surface, &dstrect);
        dstrect.y += line->h;
        }
			SDL_FreeSurface(line); // clean up as we go
			++it;
			}
		
		}
	
	
	this->texture = SDL_CreateTextureFromSurface(g_app->sdlRenderer, surface);
	SDL_SetTextureAlphaMod(this->texture,tempColor.a);
	SDL_SetTextureBlendMode(this->texture,SDL_BLENDMODE_BLEND);
	this->frame.x = 0;
	this->frame.y = 0;
	this->frame.w = width;
	this->frame.h = height;
	
	SDL_FreeSurface(surface);
  */
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetText(const std::string& _text,bool _justified)
{
	this->text = _text; //Backup for later use

	// If the font is not set, try to set it to the theme font.
	if (this->font == NULL)
		{
			if (GUIManager::Get().font != NULL)
			{
				this->font = GUIManager::Get().font;
			this->font->AddRef();
			}
		else
			return;
		}

	if (this->primary_text_color.a == 253) //TODO remove this magic number
		this->primary_text_color = GUIManager::Get().primary_text_color;

	if (this->disable_text_color.a == 253) //TODO remove this magic number
		this->disable_text_color = GUIManager::Get().disable_text_color;


	BuildInternalTexture(_text,_justified);

	//Update only size not position
	int difW = this->position.w - this->frame.w;
	int difH = this->position.h - this->frame.h;
	this->position.x  += (difW/2);
	this->position.y += (difH/2);
	this->position.h = this->frame.h;
	this->position.w = this->frame.w;	
	//this->angle = 0;

	//Restore Previous Scale
	this->SetScale(this->xScale,this->yScale);
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::Render()
{
	if ((this->shown == false)|| (this->texture == NULL))
		return;

	SDL_RenderCopyEx(g_app->sdlRenderer,this->texture , &this->frame, &this->position,this->angle,NULL,SDL_FLIP_NONE); 
}




/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetEnabled(bool _value)
{
	Widget::SetEnabled(_value);

	
	SetText(this->text); //This updates the background texture
}
