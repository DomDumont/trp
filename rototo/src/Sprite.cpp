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



#include "Sprite.h"

#include "Atlas.h"
#include "Application.h"




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
	SDL_Color tempColor = {255,255,255,255 };
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
    this->refCount = 1;
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
    this->refCount = 1;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::Render()
{
Widget::Render();
if (this->shown == false)
    return;
if ((this->entry) && (this->entry->atlas->texture))
    {
	SDL_SetTextureColorMod(this->entry->atlas->texture,this->color.r,this->color.g,this->color.b);
	SDL_SetTextureAlphaMod(this->entry->atlas->texture,this->color.a);
    if (nine_patch == false)
        {
        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &this->frame, &this->position,this->angle,NULL,SDL_FLIP_NONE);
        }
    else
        {
        SDL_Rect src_rect;
        SDL_Rect dst_rect;

        int src_w = this->frame.w - (this->nine_rect.x + this->nine_rect.w);
        int src_h = this->frame.h - (this->nine_rect.y + this->nine_rect.h);

        int dst_w = this->position.w - (this->frame.w - this->nine_rect.w);
        int dst_h = this->position.h - (this->frame.h - this->nine_rect.h);

        // 0  1 2
        // 3  4 5
        // 6  7 8

        //OK 0
        src_rect.x = this->frame.x;
        src_rect.y = this->frame.y;
        src_rect.w = this->nine_rect.x;
        src_rect.h = this->nine_rect.y;

        dst_rect.x = this->position.x;
        dst_rect.y = this->position.y;
        dst_rect.w = this->nine_rect.x;
        dst_rect.h = this->nine_rect.y;

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 1       
        src_rect.x = this->frame.x + this->nine_rect.x;
        src_rect.y = this->frame.y;
        src_rect.w = this->nine_rect.w;
        src_rect.h = this->nine_rect.y;

        dst_rect.x = this->position.x + this->nine_rect.x;
        dst_rect.y = this->position.y;
        dst_rect.w = dst_w;
        dst_rect.h = this->nine_rect.y;

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 2
        src_rect.x = this->frame.x + this->nine_rect.x + this->nine_rect.w; //OK
        src_rect.y = this->frame.y; //OK
        src_rect.w = src_w; //OK
        src_rect.h = this->nine_rect.y; //OK

        dst_rect.x = this->position.x + this->nine_rect.x + dst_w;
        dst_rect.y = this->position.y;
        dst_rect.w = src_w ;
        dst_rect.h = this->nine_rect.y; //OK

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 3
        src_rect.x = this->frame.x; //OK
        src_rect.y = this->frame.y+ this->nine_rect.y; //OK
        src_rect.w = this->nine_rect.x; 
        src_rect.h = this->nine_rect.h; 

        dst_rect.x = this->position.x ; //OK
        dst_rect.y = this->position.y+ this->nine_rect.y; //OK
        dst_rect.w = this->nine_rect.x;
        dst_rect.h = dst_h; 

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 4
        src_rect.x = this->frame.x+ this->nine_rect.x; //OK
        src_rect.y = this->frame.y+ this->nine_rect.y; //OK
        src_rect.w = this->nine_rect.w; 
        src_rect.h = this->nine_rect.h; 

        dst_rect.x = this->position.x + + this->nine_rect.x; //OK
        dst_rect.y = this->position.y+ this->nine_rect.y; //OK
        dst_rect.w = dst_w;
        dst_rect.h = dst_h; 

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 5
        src_rect.x = this->frame.x + this->nine_rect.x + this->nine_rect.w; //OK
        src_rect.y = this->frame.y  + this->nine_rect.y; //OK
        src_rect.w = src_w; //OK
        src_rect.h = this->nine_rect.h; //OK

        dst_rect.x = this->position.x + this->nine_rect.x + dst_w;
        dst_rect.y = this->position.y  + this->nine_rect.y;
        dst_rect.w = src_w ;
        dst_rect.h = dst_h; //OK

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 6
        src_rect.x = this->frame.x; //OK
        src_rect.y = this->frame.y+ this->nine_rect.y + this->nine_rect.h; //OK
        src_rect.w = this->nine_rect.x; 
        src_rect.h = src_h; 

        dst_rect.x = this->position.x ; //OK
        dst_rect.y = this->position.y+ this->nine_rect.y + dst_h; //OK
        dst_rect.w = this->nine_rect.x;
        dst_rect.h = src_h; 

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 7
        src_rect.x = this->frame.x+ this->nine_rect.x; //OK
        src_rect.y = this->frame.y+ this->nine_rect.y + this->nine_rect.h; //OK
        src_rect.w = this->nine_rect.w; 
        src_rect.h = src_h; 

        dst_rect.x = this->position.x + + this->nine_rect.x; //OK
        dst_rect.y = this->position.y+ this->nine_rect.y + dst_h; //OK
        dst_rect.w = dst_w;
        dst_rect.h = src_h; 

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);

        //OK 8
        src_rect.x = this->frame.x + this->nine_rect.x + this->nine_rect.w; //OK
        src_rect.y = this->frame.y+ this->nine_rect.y + this->nine_rect.h; //OK
        src_rect.w = src_w; //OK
        src_rect.h = src_h; 

        dst_rect.x = this->position.x + this->nine_rect.x + dst_w;
        dst_rect.y = this->position.y+ this->nine_rect.y + dst_h; //OK
        dst_rect.w = src_w ;
        dst_rect.h = src_h; 

        SDL_RenderCopyEx(g_app->sdlRenderer, this->entry->atlas->texture , &(src_rect), &(dst_rect),0,NULL,SDL_FLIP_NONE);
        }
    
    }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sprite::SetColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	SDL_Color tempColor = { _r, _g, _b, _a };
	color = tempColor;
	//if (this->texture)
	//	SDL_SetTextureAlphaMod(this->texture,_a);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterSprite()
{
	int r;

    ///class:Sprite
	r = g_app->scriptManager->engine->RegisterObjectType("Sprite", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Sprite", asBEHAVE_FACTORY, "Sprite@ f()", asFUNCTION(Sprite_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Sprite", asBEHAVE_ADDREF, "void f()", asMETHOD(Sprite,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Sprite", asBEHAVE_RELEASE, "void f()", asMETHOD(Sprite,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
    ///func:void Load(Atlas @ atlas, string &in name)
	g_app->scriptManager->RegisterClassMethod("Sprite","void Load(Atlas @ _atlas, string &in _name)", asMETHOD(Sprite, Load));
    ///func:void SetPosition(int x,int y,int from = 0)
	g_app->scriptManager->RegisterClassMethod("Sprite","void SetPosition(int _x,int _y,int _from = 0)", asMETHOD(Sprite, SetPosition));
    ///func:bool Touched(int x,int y)
	g_app->scriptManager->RegisterClassMethod("Sprite","bool Touched(int _x,int _y)", asMETHOD(Sprite, Touched));
    ///func:void SetScale(double xFactor,double yFactor)
	g_app->scriptManager->RegisterClassMethod("Sprite","void SetScale(double _xFactor,double _yFactor)", asMETHOD(Sprite, SetScale));
    ///func:void SetRotation(float angle)
	g_app->scriptManager->RegisterClassMethod("Sprite","void SetRotation(float _angle)", asMETHOD(Sprite, SetRotation));
    g_app->scriptManager->RegisterClassMethod("Sprite","double GetRotation()", asMETHOD(Sprite, GetRotation));
	g_app->scriptManager->RegisterClassMethod("Sprite","void Render()", asMETHOD(Sprite, Render));
    g_app->scriptManager->RegisterClassMethod("Sprite","void UnLoad()", asMETHOD(Sprite, UnLoad));
	g_app->scriptManager->RegisterClassMethod("Sprite","void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Sprite, SetColor));
    g_app->scriptManager->RegisterClassMethod("Sprite","void SetSize(int _w,int _h)", asMETHOD(Sprite, SetSize));
    g_app->scriptManager->RegisterClassMethod("Sprite","void SetNinePatch(bool _value)", asMETHOD(Sprite, SetNinePatch));
    g_app->scriptManager->RegisterClassMethod("Sprite","void SetNinePatchRect(int _x,int _y,int _w,int _h)", asMETHOD(Sprite, SetNinePatchRect));

}


