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

#ifdef TRP_USE_AUDIO
  dfgsdg
	//#include <SDL/SDL_Mixer.h>
#endif



#include "binding/aswrappedcall.h"


#include "ScriptManager.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void SND_SetMusicVolume(int _newVolume)
{
	g_app->soundManager->SetMusicVolume(_newVolume);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void SND_SetSFXVolume(int _newVolume)
{
	g_app->soundManager->SetSFXVolume(_newVolume);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SoundManager::SoundManager()
	{
	//TODO should be elsewhere

#ifdef TRP_USE_AUDIO

	rate		= 44100;
	format		= AUDIO_S16;
	channels	= 2;
	buffers		= 1024;
	volume		= MIX_MAX_VOLUME;
	volumeSFX	= MIX_MAX_VOLUME;
#else
		rate = 0;
		format = 0;
		channels = 0;
		buffers = 0;
		volume = 0;
		volumeSFX = 0;

#endif

	initialized	= 0;

	}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SoundManager::~SoundManager()
	{
	}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::Init()
  {
#ifdef TRP_USE_AUDIO  	
	if (Mix_OpenAudio(rate, format, channels, buffers) < 0)
		{
		SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
		}
	else
		{
		/*
		Mix_QuerySpec(&rate, &format, &channels);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Opened audio at %d Hz %d bit %s (%s), %d bytes audio buffer\n", rate,
			(format&0xFF),
			(channels > 2) ? "surround" : (channels > 1) ? "stereo" : "mono",
			(format&0x1000) ? "BE" : "LE",
			buffers );
		*/
		}
	
  
	Mix_VolumeMusic(volume);

	// allocate 16 mixing channels
	Mix_AllocateChannels(16);
#endif	
	initialized = 1;
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::SetMusicVolume(int _newVolume)
{
	this->volume = _newVolume;
#ifdef TRP_USE_AUDIO 	
	Mix_VolumeMusic(this->volume);
#endif	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::SetSFXVolume(int _newVolume)
{
	this->volumeSFX = _newVolume;
#ifdef TRP_USE_AUDIO 	
	Mix_Volume(-1, this->volumeSFX);
#endif	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::Shutdown()
{
#ifdef TRP_USE_AUDIO 	
	Mix_CloseAudio();
#endif	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING
void RegisterSoundManager()
{

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		///sect:Sound
		///glob:void SND_SetMusicVolume(int newVolume)	
		ScriptManager::Get().RegisterGlobalFunction("void SND_SetMusicVolume(int _newVolume)", asFUNCTION(SND_SetMusicVolume), asCALL_CDECL);
		///glob:void SND_SetSFXVolume(int newVolume)
		ScriptManager::Get().RegisterGlobalFunction("void SND_SetSFXVolume(int _newVolume)", asFUNCTION(SND_SetSFXVolume), asCALL_CDECL);
	}
	else
	{
		ScriptManager::Get().RegisterGlobalFunction("void SND_SetMusicVolume(int _newVolume)", WRAP_FN(SND_SetMusicVolume), asCALL_GENERIC);
		ScriptManager::Get().RegisterGlobalFunction("void SND_SetSFXVolume(int _newVolume)", WRAP_FN(SND_SetSFXVolume), asCALL_GENERIC);
	}

}
#endif

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

Sound::Sound(): sample(NULL),channel(-1)
{

#ifdef TRP_USE_AUDIO 	
	volume = MIX_MAX_VOLUME;
#else
	volume = 0;
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sound::~Sound()
{
	if (sample != NULL)
		{
#ifdef TRP_USE_AUDIO 			
		Mix_FreeChunk(sample);
#endif		
		sample = NULL;
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::UnLoad()
  {
	if (sample != NULL)
		{
#ifdef TRP_USE_AUDIO 			
		Mix_FreeChunk(sample);
#endif		
		sample = NULL;
		}
  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::Load(const std::string & _file)
  {
#ifdef TRP_USE_AUDIO 
	this->sample = Mix_LoadWAV_RW(g_app->resourceManager->Load(_file,GAMEDATA|BOTH),1); //todo check this FreeSrc ?
#endif	
	
	if ( sample == NULL ) 
		  {
		  SDL_Log("Cannot load sample %s %s",_file.c_str(),SDL_GetError());
		  return;
		  }
	  SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Sample %s loaded sucessfully",_file.c_str());
	
#ifdef TRP_USE_AUDIO 	
	Mix_VolumeChunk(this->sample,this->volume);
#endif


  }

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
static Uint64 lasttime32; 
void Sound::Play(int _nbLoops)
{
	Uint64 now = SDL_GetTicks();

	//SDL_Log("play at %lu\n", now- lasttime32);
	lasttime32 = now ;

#ifdef TRP_USE_AUDIO 	
	this->channel = Mix_PlayChannel(-1,this->sample,_nbLoops);
	//SDL_Log("Mix_PlayChannel returns : %d\n", this->channel);
	Mix_Volume(this->channel, g_app->soundManager->volumeSFX);
#endif	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::Stop()
{

#ifdef TRP_USE_AUDIO	
	Mix_HaltChannel(this->channel);
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::SetVolume(int _newVolume)
{
	this->volume = _newVolume;

#ifdef TRP_USE_AUDIO	
	Mix_VolumeChunk(this->sample,this->volume);
#endif
	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifdef TRP_USE_BINDING

void RegisterSound()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		///class:Sound
		r = ScriptManager::Get().engine->RegisterObjectType("Sound", sizeof(Sound), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sound", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructSound), asCALL_CDECL_OBJLAST);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sound", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructSound), asCALL_CDECL_OBJLAST);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Load(string &in _file)", asMETHOD(Sound, Load), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Play(int _nbLoops=0)", asMETHOD(Sound, Play), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void UnLoad()", asMETHOD(Sound, UnLoad), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Stop()", asMETHOD(Sound, Stop), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void SetVolume(int _newVolume)", asMETHOD(Sound, SetVolume), asCALL_THISCALL); SDL_assert(r >= 0);
	}
	else
	{
		r = ScriptManager::Get().engine->RegisterObjectType("Sound", sizeof(Sound), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sound", asBEHAVE_CONSTRUCT, "void f()", WRAP_CON(Sound, ()), asCALL_GENERIC);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Sound", asBEHAVE_DESTRUCT, "void f()", WRAP_DES(Sound), asCALL_GENERIC);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Load(string &in _file)", WRAP_MFN(Sound, Load), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Play(int _nbLoops=0)", WRAP_MFN(Sound, Play), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void UnLoad()", WRAP_MFN(Sound, UnLoad), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void Stop()", WRAP_MFN(Sound, Stop), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Sound", "void SetVolume(int _newVolume)", WRAP_MFN(Sound, SetVolume), asCALL_GENERIC); SDL_assert(r >= 0);

	}

}

#endif
