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



#include "Animation.h"

#include "Atlas.h"
#include "Application.h"
#include "Sprite.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Animation *Animation_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Animation();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Animation::Animation() 
	:onCompleteHandler(NULL),atlas(NULL),timeElapsed(0),currentFrame(0),nbFrames(0),msPerFrame(1000/12),playing(0)
{
	
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation Constructor %s\n",this->name.c_str());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Animation::~Animation()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation Destructor %s\n",this->name.c_str());
	MY_SAFE_RELEASE(this->atlas);
	MY_SAFE_RELEASE(this->onCompleteHandler);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Animation::Animation(const Animation &other) 
	{
	this->currentFrame = other.currentFrame;
	this->nbFrames = other.nbFrames;
	this->timeElapsed = other.timeElapsed;
	this->msPerFrame = other.msPerFrame;
	this->playing = other.playing;
	this->atlas = other.atlas;
	this->onCompleteHandler = other.onCompleteHandler; //Very Important
	}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::UnLoad()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation UnLoad %s\n",this->name.c_str());
	if (this->atlas != NULL)
		{
		this->atlas->Release(); //We don't use the atlas anymore
		this->atlas = NULL;
		}
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation UnLoad Nb sprites to delete %d\n",sprites.size());
	for (spritesIT = sprites.begin();  spritesIT != sprites.end(); ++spritesIT )
		{
		Sprite *pTemp;
		pTemp = *spritesIT;
	
		delete pTemp; //Sprites created by me (as an animation)
		//pTemp->UnLoad();
		//pTemp->Release();
		}
	sprites.clear();
}

// Cette fonction est appelée par le script
// Donc l'atlas va voir sa reference s'incrementer
// Il faut garder l'atlas pour pouvoir faire un release dessus
// Sinon il ne sera jamais libéré

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::Load(Atlas * _atlas, const std::string& _name,const std::string& _format,int _nbFrames)
{
	this->atlas = _atlas;
	this->angle = 0;

	this->name = _name;

	this->currentFrame = 0;
	this->timeElapsed = 0;
	this->nbFrames = _nbFrames;
	for (int i=0;i<_nbFrames;i++)
		{
		char tempName[256];
		sprintf(tempName,_format.c_str()/*"%s_%05d.tga"*/,_name.c_str(),i);
		Sprite *tempSprite = new Sprite();
		tempSprite->Load(_atlas,tempName);
		//LOad is not called by script so we must add the reference manually
		_atlas->AddRef();
		sprites.push_back(tempSprite);
		}
	

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::SetFPS(int _fps)
{
	this->msPerFrame = (1000/(float)_fps);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::Update(Uint64 _elapsed)
{
if (this->playing != 0)
	{
	timeElapsed+=_elapsed;
	if (this->sens == 1)
	{
	// SENS POSITIF
		
	this->currentFrame = (int)((float)this->timeElapsed/(float)this->msPerFrame);
	if (this->currentFrame >= this->nbFrames)
		{
		// We are at the end of the animation
		//There are 2 cases
		// if mode is normal, it's finished
		// if mode is pingpong, we just reverse the sens
		if (this->mode == C_MODE_NORMAL)
		{
		//Call Callback
		if (this->onCompleteHandler != NULL)
			{
			g_app->scriptManager->RunCallback(this->onCompleteHandler,&(this->sender),&(this->userData));
			}
		
		if (this->playing != -1)
			{
			this->playing --;
			this->currentFrame = this->nbFrames-1;
			this->timeElapsed = 0;
			
			}
		else
			{
			//endless loop
			this->currentFrame = this->nbFrames-1;
			this->timeElapsed = 0;
			}
		if (this->stopASAP == true)
		this->playing = 0;
		}
		else if (this->mode == C_MODE_PINGPONG)
		{
			this->sens = -1;
			this->currentFrame = this->nbFrames-1;
			this->timeElapsed = 0;
		}
	
	
		}
		
	}
	else
	{
	// SENS NEGATIF
		
	this->currentFrame = (this->nbFrames-1) - (int)((float)this->timeElapsed/(float)this->msPerFrame);
	if (this->currentFrame <= 0)
		{
	
		//Call Callback
		if (this->onCompleteHandler != NULL)
		{
		g_app->scriptManager->RunCallback(this->onCompleteHandler,&(this->sender),&(this->userData));
		}
		
		if (this->mode == C_MODE_PINGPONG)
		{
		this->sens = +1;
		}
		
		if (this->playing != -1)
		{
		this->playing --;
		this->currentFrame = 0;
		this->timeElapsed = 0;
		}
		else
		{
		//endless loop
		this->currentFrame = 0;
		this->timeElapsed = 0;
		}
		
		if (this->stopASAP == true)
		this->playing = 0;

		}
	}
	
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::Render()
{
	Widget::Render();
	if (this->shown == false)
		return;

	sprites[this->currentFrame]->Render();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::Play(int _mode, int _nbLoops)
{
	this->timeElapsed = 0;
	this->stopASAP = false;
	this->playing = _nbLoops;
	this->mode = _mode;
	if ((this->mode == C_MODE_NORMAL)||(this->mode == C_MODE_PINGPONG))
		{
		this->sens = 1;
		this->currentFrame = 0;
		}
	else
		{
		this->sens = -1;
		this->currentFrame = this->nbFrames-1;
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::Stop(bool _waitEnd)
{
	if (_waitEnd == true)
		this->stopASAP = true;
	else
		this->playing = 0;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

bool Animation::Touched(int _x,int _y)
{
	return sprites[this->currentFrame]->Touched(_x,_y);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void  Animation::SetPosition(int _x,int _y,int _from)
{
	switch(_from)
	{
	case 0:
		{
		for (int i=0;i<this->nbFrames;i++)
			{
			sprites[i]->SetPosition(_x,_y,0);
			}
		}
	break;
	case 1:
		{
		for (int i=0;i<this->nbFrames;i++)
			{
			sprites[i]->SetPosition(_x,_y,1);
			}
		}
	break;

	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::SetScale(double _xFactor,double _yFactor)
{
	
	for (int i=0;i<this->nbFrames;i++)
		{
		sprites[i]->SetScale(_xFactor,_yFactor);
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::SetRotation(float _angle)
{
	
	for (int i=0;i<this->nbFrames;i++)
		{
		sprites[i]->SetRotation(_angle);
		}

}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterAnimation()
{
	int r;

	///class:Animation
	r = g_app->scriptManager->engine->RegisterObjectType("Animation", 0, asOBJ_REF); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Animation", asBEHAVE_FACTORY, "Animation@ f()", asFUNCTION(Animation_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Animation", asBEHAVE_ADDREF, "void f()", asMETHOD(Animation,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Animation", asBEHAVE_RELEASE, "void f()", asMETHOD(Animation,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	///func:void Load(Atlas @ _atlas, string &in _name,string &in _format,int _nbFrames)
	///role: Load an animation sheet from an atlas given a specific format and a frame number
	///arg: _atlas : Reference to the atlas containing the animation
	///arg: _name : name of the animation
	///sample:int toto\ntoto = Load(titi);\ntata
	g_app->scriptManager->RegisterClassMethod("Animation","void Load(Atlas @ _atlas, string &in _name,string &in _format,int _nbFrames)", asMETHOD(Animation, Load));
	///func:void SetFPS(int fps)
	///role: Set the speed of the animation in frames per second
	///arg: _fps : frames per second
	g_app->scriptManager->RegisterClassMethod("Animation","void SetFPS(int _fps)", asMETHOD(Animation, SetFPS));
	///func:void Update(uint64 elapsed)
	g_app->scriptManager->RegisterClassMethod("Animation","void Update(uint64 _elapsed)", asMETHOD(Animation, Update));
	///func:void Render()
	g_app->scriptManager->RegisterClassMethod("Animation","void Render()", asMETHOD(Animation, Render));
	///func:void Play(int mode = 0, int nbLoops = 1)
	g_app->scriptManager->RegisterClassMethod("Animation","void Play(int _mode = 0, int _nbLoops = 1)", asMETHOD(Animation, Play));
	///func:void Stop(bool waitEnd = true)
	g_app->scriptManager->RegisterClassMethod("Animation","void Stop(bool _waitEnd = true)", asMETHOD(Animation, Stop));
	///func:void SetPosition(int x,int y,int from = 0)
	g_app->scriptManager->RegisterClassMethod("Animation","void SetPosition(int _x,int _y,int _from = 0)", asMETHOD(Animation, SetPosition));
	///func:bool Touched(int x,int y)
	g_app->scriptManager->RegisterClassMethod("Animation","bool Touched(int _x,int _y)", asMETHOD(Animation, Touched));
	///func:void SetScale(double xFactor,double yFactor)
	g_app->scriptManager->RegisterClassMethod("Animation","void SetScale(double _xFactor,double _yFactor)", asMETHOD(Animation, SetScale));
	///func:void SetRotation(float angle)
	g_app->scriptManager->RegisterClassMethod("Animation","void SetRotation(float _angle)", asMETHOD(Animation, SetRotation));
	///func:double GetRotation()
	g_app->scriptManager->RegisterClassMethod("Animation","double GetRotation()", asMETHOD(Animation, GetRotation));
	///func:void UnLoad()
	g_app->scriptManager->RegisterClassMethod("Animation","void UnLoad()", asMETHOD(Animation, UnLoad));
	///prop:CallbackHandler @onComplete
	g_app->scriptManager->RegisterObjectProperty("Animation", "CallbackHandler @onComplete", asOFFSET(Animation, onCompleteHandler));
	///prop:ref @userData
	g_app->scriptManager->RegisterObjectProperty("Animation", "ref @userData", asOFFSET(Animation, userData));

}
