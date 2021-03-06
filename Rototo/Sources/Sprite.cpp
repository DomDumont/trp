/*
  ==============================================================================

   This file is part of the T.R.P. Engine
   Copyright (c) 2015 - Dominique Dumont

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



#include "Sprite.h"

#include "Atlas.h"
#include "Application.h"



#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "SDL.h"

#include "Renderer.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Sprite::SetScale(float _xFactor, float _yFactor)
{
	Widget::SetScale(_xFactor,_yFactor);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void	Sprite::SetSize(int _x,int _y)
{
	Widget::SetSize(_x,_y);
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void	Sprite::SetPosition(int _x,int _y)
{
	Widget::SetPosition(_x,_y);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void	Sprite::SetPosition(Vector2D _pos)
{
	Widget::SetPosition(_pos);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sprite *Sprite_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Sprite();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sprite::Sprite() : nine_patch(false)
{
	Color tempColor = {255,255,255,255 };
	this->color = tempColor;

this->atlas = NULL;
this->entry = NULL;   

this->SetNinePatchRect(0,0,0,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sprite::Sprite(const Sprite &other)
{
	this->atlas = other.atlas;
	this->atlas->AddRef();

	this->entry = other.entry;



	this->color = other.color;
	this->nine_patch = other.nine_patch;
	this->nine_rect = other.nine_rect;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::operator= (const Sprite & other)
{

	Widget::operator=(other);

	this->atlas = other.atlas;
	this->atlas->AddRef();

	this->entry = other.entry;



	this->color = other.color;
	this->nine_patch = other.nine_patch;
	this->nine_rect = other.nine_rect;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sprite::~Sprite()
{
	MY_SAFE_RELEASE(this->atlas);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::SetNinePatch(bool _value)
{
	nine_patch = _value;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::SetNinePatchRect(int _x,int _y,int _w,int _h)
{
	
	this->nine_rect.x = _x;
	this->nine_rect.y = _y;
	this->nine_rect.w = _w;
	this->nine_rect.h = _h;
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::UnLoad()
{
	
	MY_SAFE_RELEASE(this->atlas);
	this->entry = NULL; //Will be deleted by atlas.
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::Load(Atlas * _atlas, const std::string& _name)
{

	//If there was a previous atlas, release it
	MY_SAFE_RELEASE(this->atlas);

	this->atlas = _atlas;	
	this->atlas->AddRef();
	this->angle = 0;

	this->entry = _atlas->FindEntry(_name);
	if (this->entry != NULL)
		{
		this->position = this->frame = this->entry->frame;
		this->position.x = 0;
		this->position.y = 0;
		}
	

}

/*----------------------------------------------------------------------------*/


void Sprite::Render()
{
	//For script only
	Renderer::Get().RenderSprite(this);
}

/*----------------------------------------------------------------------------*/

void Sprite::SetColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	Color tempColor = { _r, _g, _b, _a };
	color = tempColor;
	//if (this->texture)
	//	SDL_SetTextureAlphaMod(this->texture,_a);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING

void RegisterSprite()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{

		///class:Sprite
		r = ScriptManager::Get().engine->RegisterObjectType("Sprite", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite@ f()", asFUNCTION(Sprite_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_ADDREF, "void f()", asMETHOD(Sprite, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_RELEASE, "void f()", asMETHOD(Sprite, Release), asCALL_THISCALL); SDL_assert(r >= 0);

		///func:void Load(Atlas @ atlas, string &in name)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void Load(Atlas @ _atlas, string &in _name)", asMETHOD(Sprite, Load), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetPosition(int x,int y)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetPosition(int _x,int _y)", asMETHODPR(Sprite, SetPosition, (int, int), void), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void set_Position(Vector2D pos)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void set_Position(Vector2D _pos)", asMETHODPR(Sprite, SetPosition, (Vector2D), void), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:bool Touched(int x,int y)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "bool Touched(int _x,int _y)", asMETHOD(Sprite, Touched), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetScale(float xFactor,float yFactor)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetScale(float _xFactor,float _yFactor)", asMETHOD(Sprite, SetScale), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetRotation(float angle)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void set_Rotation(float _angle)", asMETHOD(Sprite, SetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:float get_Rotation()
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "float get_Rotation()", asMETHOD(Sprite, GetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void Render()
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void Render()", asMETHOD(Sprite, Render), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void UnLoad()
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void UnLoad()", asMETHOD(Sprite, UnLoad), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetColor(uint8 r=255,uint8 g=255,uint8 b=255,uint8 a=255)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Sprite, SetColor), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetSize(int w,int h)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetSize(int _w,int _h)", asMETHODPR(Sprite, SetSize, (int, int), void), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetNinePatch(bool value)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetNinePatch(bool _value)", asMETHOD(Sprite, SetNinePatch), asCALL_THISCALL); SDL_assert(r >= 0);
		///func:void SetNinePatchRect(int x,int y,int w,int h)
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetNinePatchRect(int _x,int _y,int _w,int _h)", asMETHOD(Sprite, SetNinePatchRect), asCALL_THISCALL); SDL_assert(r >= 0);
	}
	else
	{
		r = ScriptManager::Get().engine->RegisterObjectType("Sprite", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite@ f()", WRAP_FN(Sprite_Factory), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Sprite, AddRef), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sprite", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Sprite, Release), asCALL_GENERIC); SDL_assert(r >= 0);


		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void Load(Atlas @ _atlas, string &in _name)", WRAP_MFN(Sprite, Load), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetPosition(int _x,int _y)", WRAP_MFN_PR(Sprite, SetPosition, (int, int), void), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void set_Position(Vector2D _pos)", WRAP_MFN_PR(Sprite, SetPosition, (Vector2D), void), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "bool Touched(int _x,int _y)", WRAP_MFN(Sprite, Touched), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetScale(float _xFactor,float _yFactor)", WRAP_MFN(Sprite, SetScale), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void set_Rotation(float _angle)", WRAP_MFN(Sprite, SetRotation), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "float get_Rotation()", WRAP_MFN(Sprite, GetRotation), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void Render()", WRAP_MFN(Sprite, Render), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void UnLoad()", WRAP_MFN(Sprite, UnLoad), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", WRAP_MFN(Sprite, SetColor), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetSize(int _w,int _h)", WRAP_MFN_PR(Sprite, SetSize, (int, int), void), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetNinePatch(bool _value)", WRAP_MFN(Sprite, SetNinePatch), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sprite", "void SetNinePatchRect(int _x,int _y,int _w,int _h)", WRAP_MFN(Sprite, SetNinePatchRect), asCALL_GENERIC); SDL_assert(r >= 0);

	}

}

#endif

