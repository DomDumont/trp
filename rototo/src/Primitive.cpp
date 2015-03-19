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
#include "Primitive.h"
#include "Application.h"
#include "Primitives.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Primitive *Primitive_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Primitive();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterPrimitive()
{
	int r;
	///class:Primitive
	r = g_app->scriptManager->engine->RegisterObjectType("Primitive", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Primitive", asBEHAVE_FACTORY, "Primitive@ f()", asFUNCTION(Primitive_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Primitive", asBEHAVE_ADDREF, "void f()", asMETHOD(Primitive,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Primitive", asBEHAVE_RELEASE, "void f()", asMETHOD(Primitive,Release), asCALL_THISCALL); SDL_assert( r >= 0 );

	///func:void Render()
	g_app->scriptManager->RegisterClassMethod("Primitive","void Render()", asMETHOD(Primitive, Render));
	///func:void SetColor(uint8 r=255,uint8 g=255,uint8 b=255,uint8 a=255)
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetColor(uint8 _r=255,uint8 _g=255,uint8 _b=255,uint8 _a=255)", asMETHOD(Primitive, SetColor));
	///func:void SetPosition(int x,int y)
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetPosition(int _x,int _y)", asMETHODPR(Primitive, SetPosition,(int,int),void));
	g_app->scriptManager->RegisterClassMethod("Primitive","bool Touched(int _x,int _y)", asMETHOD(Primitive, Touched));
	g_app->scriptManager->RegisterClassMethod("Primitive","void set_Rotation(float _angle)", asMETHOD(Primitive, SetRotation));
	///func:float GetRotation()
	g_app->scriptManager->RegisterClassMethod("Primitive","float get_Rotation()", asMETHOD(Primitive, GetRotation));
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetSize(int _w,int _h)", asMETHOD(Primitive, SetSize));
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetScale(double _xFactor,double _yFactor)", asMETHOD(Primitive, SetScale));
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetShape(int _shape)", asMETHOD(Primitive, SetShape));
	g_app->scriptManager->RegisterClassMethod("Primitive","void SetAngles(int _start,int _end)", asMETHOD(Primitive, SetAngles));
	
}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Primitive::Primitive() : bgTexture(NULL),shape(0)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Primitive Constructor\n");

	SDL_Color tempColor = {255,255,255,255 };
	this->color = tempColor;
	this->colorBG.r = 0;
	this->colorBG.b = 0;
	this->colorBG.g = 0;
	this->colorBG.a = 255;
	this->startAngle = 0;
	this->endAngle = 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Primitive::~Primitive()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Primitive Destructor\n");

	if (this->bgTexture != NULL)
		{
		SDL_DestroyTexture(this->bgTexture); // et on la vire
		this->bgTexture = NULL;
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::SetColor(unsigned char _r,unsigned char _g,unsigned char _b,unsigned char _a)
{
	SDL_Color tempColor = { _r, _g, _b, _a };
	this->color = tempColor;
	BuildInternalTexture();
	/*
	if (this->bgTexture)
		SDL_SetTextureAlphaMod(this->bgTexture,_a);*/


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::SetShape(int _shape)
{
	this->shape = _shape;
	BuildInternalTexture();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::BuildInternalTexture()
{
	if (this->bgTexture != NULL)
		{
		SDL_DestroyTexture(this->bgTexture); // et on la vire
		}
	this->bgTexture = SDL_CreateTexture(g_app->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, this->position.w, this->position.h);

	SDL_SetTextureBlendMode(this->bgTexture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(g_app->sdlRenderer, this->bgTexture);


	SDL_SetRenderDrawColor(g_app->sdlRenderer, 0, 0, 0, 0);
	SDL_RenderClear(g_app->sdlRenderer);

	switch(this->shape)
	{
	case 0:
		boxRGBA(g_app->sdlRenderer, 0, 0, this->frame.w, this->frame.h, this->color.r,this->color.g,this->color.b,this->color.a);
		break;
	case 1:
		
		filledEllipseRGBA(g_app->sdlRenderer, this->position.w/2, this->position.h/2, (this->frame.w/2)-2,(this->frame.h/2)-2, this->color.r,this->color.g,this->color.b,this->color.a);
		aaellipseRGBA(g_app->sdlRenderer, this->frame.w/2, this->frame.h/2, (this->frame.w/2)-2,(this->frame.h/2)-2, this->color.r,this->color.g,this->color.b,this->color.a);
		
		break;
	case 2:
		filledPieRGBA(g_app->sdlRenderer, this->frame.w/2, this->frame.h/2, this->frame.w/2,startAngle,endAngle, this->color.r,this->color.g,this->color.b,this->color.a);
		break;
	}
	//roundedBoxRGBA(g_app->sdlRenderer, 0, 0, this->position.w, this->position.h, 10 /*rad*/, this->color.r,this->color.g,this->color.b,this->color.a);


	SDL_RenderCopy(g_app->sdlRenderer, this->bgTexture, NULL, NULL); // On fait le rendu de la texture
	SDL_SetRenderTarget(g_app->sdlRenderer, NULL); // On repasse en target normale

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::SetScale(double _xFactor,double _yFactor)
{
	Widget::SetScale(_xFactor,_yFactor);
	//BuildInternalTexture();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::SetAngles(int _start,int _end)
{
	startAngle	= _start;
	endAngle	= _end;
	BuildInternalTexture();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::Render()
{
	if ((this->shown == false)|| (this->bgTexture == NULL))
		return;

	SDL_SetTextureBlendMode(this->bgTexture, SDL_BLENDMODE_BLEND);


	SDL_RenderCopyEx(g_app->sdlRenderer,this->bgTexture, &this->frame,&this->position,this->angle,NULL,SDL_FLIP_NONE);

	SDL_SetRenderDrawBlendMode(g_app->sdlRenderer, SDL_BLENDMODE_NONE);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Primitive::SetSize(int _w,int _h)
{
	
	this->frame.w = _w;
	this->frame.h = _h;

	Widget::SetSize(_w,_h);
	//BuildInternalTexture();
	
}


