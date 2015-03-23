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



#include "SoundManager.h"
#include "Application.h"




/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SoundManager::SoundManager()
	{
	rate		= 44100;
	format		= AUDIO_S16;
	channels	= 2;
	buffers		= 1024;
	initialized	= 0;

	}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SoundManager::~SoundManager()
	{
	}

//----------------------------------------------------------------------------
//

void SoundManager::Init()
  {
  initialized = 1;	
  }

//----------------------------------------------------------------------------
//

void SoundManager::SetMusicVolume(int _newVolume)
{
	this->volume = _newVolume;
}

//----------------------------------------------------------------------------
//

void SoundManager::SetSFXVolume(int _newVolume)
{
	this->volumeSFX = _newVolume;
}

//----------------------------------------------------------------------------
//

void SoundManager::Shutdown()
{

}

//----------------------------------------------------------------------------
//

#ifdef TRP_USE_BINDING
void RegisterSoundManager()
{

	///sect:Sound
	///glob:void SND_SetMusicVolume(int newVolume)
	g_app->scriptManager->RegisterGlobalFunction("void SND_SetMusicVolume(int _newVolume)", asMETHOD(SoundManager,SetMusicVolume), asCALL_THISCALL_ASGLOBAL, g_app->soundManager);
	///glob:void SND_SetSFXVolume(int newVolume)
	g_app->scriptManager->RegisterGlobalFunction("void SND_SetSFXVolume(int _newVolume)", asMETHOD(SoundManager,SetSFXVolume), asCALL_THISCALL_ASGLOBAL, g_app->soundManager);

}
#endif

//-----------------------------------------------------------------------------
// Music
//-----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ConstructMusic(Music *thisPointer)
{
	new(thisPointer) Music();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void DestructMusic(Music *thisPointer)
{
	thisPointer->~Music();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING

void RegisterMusic()
{
	int r;
	///class:Music
	r = g_app->scriptManager->engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r>=0);
	//this->RegisterClass<Music>("Music");
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT,  "void f()",asFUNCTION(ConstructMusic), asCALL_CDECL_OBJLAST); SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Music", asBEHAVE_DESTRUCT,   "void f()",asFUNCTION(DestructMusic),  asCALL_CDECL_OBJLAST);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Music","void Load(string &in _file)", asMETHOD(Music, Load), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Music","void Play(int _nbLoops=-1, int _timeFadeIn=1000)", asMETHOD(Music, Play), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Music","void UnLoad()", asMETHOD(Music, UnLoad), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Music","void Stop()", asMETHOD(Music, Stop), asCALL_THISCALL);SDL_assert(r>=0);

}
#endif


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Music::Music()
{

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Music::~Music()
  {
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::UnLoad()
  {
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Load(const std::string & _file)
  {
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Play(int _nbLoops, int _timeFadeIn)
{

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Stop()
{

}
//-----------------------------------------------------------------------------
// Sound
//-----------------------------------------------------------------------------

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ConstructSound(Sound *thisPointer)
{
	new(thisPointer) Sound();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void DestructSound(Sound *thisPointer)
{
	thisPointer->~Sound();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sound::Sound(): channel(-1)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sound::~Sound()
{
}

//----------------------------------------------------------------------------
// 

void Sound::UnLoad()
  {
  }



//----------------------------------------------------------------------------
// Load a sample

void Sound::Load
  (
  const std::string & _file
  )

  {
  /* this->sample = Mix_LoadWAV_RW(g_app->resourceManager->Load(_file,GAMEDATA|BOTH),1); //todo check this FreeSrc ?
  this->sample = nullptr;
  if (sample == nullptr)
		  {
		  SDL_Log("Cannot load sample %s %s",_file.c_str(),SDL_GetError());
		  return;
		  }
    */
	  SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Sample %s loaded sucessfully",_file.c_str());
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
static Uint64 lasttime32; 
void Sound::Play(int _nbLoops)
{
	Uint64 now = SDL_GetTicks();

	lasttime32 = now ;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::Stop()
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::SetVolume(int _newVolume)
{
	this->volume = _newVolume;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterSound()
{
	int r;

	///class:Sound
	r = g_app->scriptManager->engine->RegisterObjectType("Sound", sizeof(Sound) , asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Sound", asBEHAVE_CONSTRUCT,  "void f()",asFUNCTION(ConstructSound), asCALL_CDECL_OBJLAST);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Sound", asBEHAVE_DESTRUCT,   "void f()",asFUNCTION(DestructSound),  asCALL_CDECL_OBJLAST);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Sound","void Load(string &in _file)", asMETHOD(Sound, Load), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Sound","void Play(int _nbLoops=0)", asMETHOD(Sound, Play), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Sound","void UnLoad()", asMETHOD(Sound, UnLoad), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Sound","void Stop()", asMETHOD(Sound, Stop), asCALL_THISCALL);SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Sound","void SetVolume(int _newVolume)", asMETHOD(Sound, SetVolume), asCALL_THISCALL);SDL_assert(r>=0);

}

#endif
