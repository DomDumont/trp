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

#include "SDL.h"

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
	Color tempColor = {255,255,255,253 };
	this->primary_text_color = tempColor;
	this->disable_text_color = tempColor;
	this->color_background.r = 0;
	this->color_background.b = 0;
	this->color_background.g = 0;
	this->color_background.a = 253;

	this->isDirty = true;
	
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
	Color tempColor = { _r, _g, _b, _a };
	this->primary_text_color = tempColor;
	if (this->texture)
		SDL_SetTextureAlphaMod(this->texture,_a);

	SetText(this->text);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetText(const std::string& _text,bool _justified)
{
	this->text = _text; //Backup for later use
	this->justified = _justified;

	this->isDirty = true;

	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::Render()
{



	//For script only
	Renderer::Get().RenderLabel(this);

}




/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Label::SetEnabled(bool _value)
{
	Widget::SetEnabled(_value);

	
	SetText(this->text); //This updates the background texture
}
