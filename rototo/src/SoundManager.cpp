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
	//TODO should be elsewhere

	//rate		= 22050;
	rate		= 44100;
	format		= AUDIO_S16;
	channels	= 2;
	buffers		= 1024;
	volume		= MIX_MAX_VOLUME;
	volumeSFX	= MIX_MAX_VOLUME;
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
	if (Mix_OpenAudio(rate, format, channels, buffers) < 0)
		{
		SDL_Log("Couldn't open audio: %s\n", SDL_GetError());
		}
	else
		{
		Mix_QuerySpec(&rate, &format, &channels);
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Opened audio at %d Hz %d bit %s (%s), %d bytes audio buffer\n", rate,
			(format&0xFF),
			(channels > 2) ? "surround" : (channels > 1) ? "stereo" : "mono",
			(format&0x1000) ? "BE" : "LE",
			buffers );
		}
	
	initialized = 1;
	Mix_VolumeMusic(volume);

	// allocate 16 mixing channels
	Mix_AllocateChannels(16);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::SetMusicVolume(int _newVolume)
{
	this->volume = _newVolume;
	Mix_VolumeMusic(this->volume);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::SetSFXVolume(int _newVolume)
{
	this->volumeSFX = _newVolume;
	Mix_Volume(-1, this->volumeSFX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void SoundManager::Shutdown()
{
	Mix_CloseAudio();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterSoundManager()
{

	///sect:Sound
	///glob:void SND_SetMusicVolume(int newVolume)
	g_app->scriptManager->RegisterGlobalFunction("void SND_SetMusicVolume(int _newVolume)", asMETHOD(SoundManager,SetMusicVolume), asCALL_THISCALL_ASGLOBAL, g_app->soundManager);
	///glob:void SND_SetSFXVolume(int newVolume)
	g_app->scriptManager->RegisterGlobalFunction("void SND_SetSFXVolume(int _newVolume)", asMETHOD(SoundManager,SetSFXVolume), asCALL_THISCALL_ASGLOBAL, g_app->soundManager);

}

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
	/*
	this->RegisterClassMethod("Music","void Load(string &in _file)", asMETHOD(Music, Load));
	this->RegisterClassMethod("Music","void Play(int _nbLoops=-1, int _timeFadeIn=1000)", asMETHOD(Music, Play));
	this->RegisterClassMethod("Music","void UnLoad()", asMETHOD(Music, UnLoad));
	*/

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Music::Music()
{
	music = NULL;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Music::~Music()
{
if (music != NULL)
	{
	Mix_FreeMusic(music);
	music = NULL;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::UnLoad()
{
if (music != NULL)
	{
	Mix_FreeMusic(music);
	music = NULL;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Load(const std::string & _file)
{
	music = Mix_LoadMUS_RW(g_app->resourceManager->Load(_file,GAMEDATA|BOTH),1); //todo check this FreeSrc ?

	if ( music == NULL ) 
		{
		SDL_Log("Cannot load music %s %s",_file.c_str(),SDL_GetError());
		}
	else
		{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Music %s loaded sucessfully",_file.c_str());
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Play(int _nbLoops, int _timeFadeIn)
{
	Mix_FadeInMusic(music,_nbLoops,_timeFadeIn);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Stop()
{
	Mix_HaltMusic();
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

Sound::Sound(): sample(NULL),channel(-1),volume(MIX_MAX_VOLUME)
{
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Sound::~Sound()
{
	if (sample != NULL)
		{
		Mix_FreeChunk(sample);
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
		Mix_FreeChunk(sample);
		sample = NULL;
		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::Load(const std::string & _file)
{
	this->sample = Mix_LoadWAV_RW(g_app->resourceManager->Load(_file,GAMEDATA|BOTH),1); //todo check this FreeSrc ?
	
	if ( sample == NULL ) 
		{
		SDL_Log("Cannot load sample %s %s",_file.c_str(),SDL_GetError());
		return;
		}
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Sample %s loaded sucessfully",_file.c_str());
	
	Mix_VolumeChunk(this->sample,this->volume);

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
	this->channel = Mix_PlayChannel(-1,this->sample,_nbLoops);
	//SDL_Log("Mix_PlayChannel returns : %d\n", this->channel);
	Mix_Volume(this->channel, g_app->soundManager->volumeSFX);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::Stop()
{
	Mix_HaltChannel(this->channel);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Sound::SetVolume(int _newVolume)
{
	this->volume = _newVolume;
	Mix_VolumeChunk(this->sample,this->volume);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

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
