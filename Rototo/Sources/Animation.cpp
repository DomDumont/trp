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


#include "binding/aswrappedcall.h"



#include "ScriptManager.h"

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
	:atlas(NULL),timeElapsed(0),currentFrame(0),nbFrames(0),playing(0)
{
#ifdef TRP_USE_BINDING
	onCompleteHandler= nullptr;
#endif
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation Constructor %s\n",this->name.c_str());
	mode = 0;
	sens = 0;
	stopASAP = false;
	this->msPerFrame = (float)(1000.0 / 12.0);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Animation::~Animation()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Animation Destructor %s\n",this->name.c_str());
	MY_SAFE_RELEASE(this->atlas);
#ifdef TRP_USE_BINDING
	MY_SAFE_RELEASE(this->onCompleteHandler);
#endif
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
#ifdef TRP_USE_BINDING
	this->onCompleteHandler = other.onCompleteHandler; //Very Important
#endif

	this->mode = other.mode;
	this->sens = other.sens;
	this->stopASAP = other.stopASAP;

	}


void Animation::SetUserDataScript(CScriptHandle userdata)
{

}

void Animation::SetOnCompleteHandlerScript(asIScriptFunction * handler)
{
	if ((this->onCompleteHandler) && (this->onCompleteHandler != handler))
	{
		this->onCompleteHandler->Release();
	}
	if (handler != NULL)
	{
		this->onCompleteHandler = (asIScriptFunction *)handler;
		handler->AddRef();
	}
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

void Animation::Update(unsigned int _elapsed)
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
#ifdef TRP_USE_BINDING
		//Call Callback
		if (this->onCompleteHandler != NULL)
			{
			ScriptManager::Get().RunCallback(this->onCompleteHandler,&(this->sender),&(this->userData));
			}
#endif
		
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
#ifdef TRP_USE_BINDING	
		//Call Callback
		if (this->onCompleteHandler != NULL)
		{
		ScriptManager::Get().RunCallback(this->onCompleteHandler,&(this->sender),&(this->userData));
		}
#endif
		
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

void  Animation::SetPosition(int _x,int _y)
{

	for (int i=0;i<this->nbFrames;i++)
		{
		sprites[i]->SetPosition(_x,_y);
		}

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Animation::SetScale(float _xFactor, float _yFactor)
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
#ifdef TRP_USE_BINDING
void RegisterAnimation()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{

	///class:Animation
	r = ScriptManager::Get().engine->RegisterObjectType("Animation", 0, asOBJ_REF); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_FACTORY, "Animation@ f()", asFUNCTION(Animation_Factory), asCALL_CDECL); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_ADDREF, "void f()", asMETHOD(Animation, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_RELEASE, "void f()", asMETHOD(Animation, Release), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void Load(Atlas @ _atlas, string &in _name,string &in _format,int _nbFrames)
	///role: Load an animation sheet from an atlas given a specific format and a frame number
	///arg: _atlas : Reference to the atlas containing the animation
	///arg: _name : name of the animation
	///sample:int toto\ntoto = Load(titi);\ntata
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Load(Atlas @ _atlas, string &in _name,string &in _format,int _nbFrames)", asMETHOD(Animation, Load), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void SetFPS(int fps)
	///role: Set the speed of the animation in frames per second
	///arg: _fps : frames per second
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetFPS(int _fps)", asMETHOD(Animation, SetFPS), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void Update(unsigned int elapsed)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Update(uint32 _elapsed)", asMETHOD(Animation, Update), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void Render()
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Render()", asMETHOD(Animation, Render), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void Play(int mode = 0, int nbLoops = 1)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Play(int _mode = 0, int _nbLoops = 1)", asMETHOD(Animation, Play), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void Stop(bool waitEnd = true)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Stop(bool _waitEnd = true)", asMETHOD(Animation, Stop), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void SetPosition(int x,int y)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetPosition(int _x,int _y)", asMETHOD(Animation, SetPosition), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:bool Touched(int x,int y)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "bool Touched(int _x,int _y)", asMETHOD(Animation, Touched), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void SetScale(float xFactor,float yFactor)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetScale(float _xFactor,float _yFactor)", asMETHOD(Animation, SetScale), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void SetRotation(float angle)
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void set_Rotation(float _angle)", asMETHOD(Animation, SetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:float get_Rotation()
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "float get_Rotation()", asMETHOD(Animation, GetRotation), asCALL_THISCALL); SDL_assert(r >= 0);
	///func:void UnLoad()
	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void UnLoad()", asMETHOD(Animation, UnLoad), asCALL_THISCALL); SDL_assert(r >= 0);

	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetUserData( ref @userdata)", asMETHOD(Animation, SetUserDataScript), asCALL_THISCALL);
	SDL_assert(r >= 0);

	r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetOnCompleteHandler( CallbackHandler @ch)", asMETHOD(Animation, SetOnCompleteHandlerScript), asCALL_THISCALL);
	SDL_assert(r >= 0);
	}
	else
	{

		r = ScriptManager::Get().engine->RegisterObjectType("Animation", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_FACTORY, "Animation@ f()", WRAP_FN(Animation_Factory), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Animation, AddRef), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Animation", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Animation, Release), asCALL_GENERIC); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Load(Atlas @ _atlas, string &in _name,string &in _format,int _nbFrames)", WRAP_MFN(Animation, Load), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetFPS(int _fps)", WRAP_MFN(Animation, SetFPS), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Update(uint32 _elapsed)", WRAP_MFN(Animation, Update), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Render()", WRAP_MFN(Animation, Render), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Play(int _mode = 0, int _nbLoops = 1)", WRAP_MFN(Animation, Play), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void Stop(bool _waitEnd = true)", WRAP_MFN(Animation, Stop), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetPosition(int _x,int _y)", WRAP_MFN(Animation, SetPosition), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "bool Touched(int _x,int _y)", WRAP_MFN(Animation, Touched), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetScale(float _xFactor,float _yFactor)", WRAP_MFN(Animation, SetScale), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void set_Rotation(float _angle)", WRAP_MFN(Animation, SetRotation), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "float get_Rotation()", WRAP_MFN(Animation, GetRotation), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void UnLoad()", WRAP_MFN(Animation, UnLoad), asCALL_GENERIC); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetUserData( ref @userdata)", WRAP_MFN(Animation, SetUserDataScript), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Animation", "void SetOnCompleteHandler( CallbackHandler @ch)", WRAP_MFN(Animation, SetOnCompleteHandlerScript), asCALL_GENERIC);
		SDL_assert(r >= 0);
	}
}
#endif