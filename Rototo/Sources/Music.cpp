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


#include "Music.h"
#include "Application.h"


#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "SDL.h"

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

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{

		///class:Music
		r = ScriptManager::Get().engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
		//this->RegisterClass<Music>("Music");
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructMusic), asCALL_CDECL_OBJLAST); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructMusic), asCALL_CDECL_OBJLAST); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Load(string &in _file)", asMETHOD(Music, Load), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Play(int _nbLoops=-1, int _timeFadeIn=1000)", asMETHOD(Music, Play), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void UnLoad()", asMETHOD(Music, UnLoad), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Stop()", asMETHOD(Music, Stop), asCALL_THISCALL); SDL_assert(r >= 0);
	}
	else
	{

		r = ScriptManager::Get().engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT, "void f()", WRAP_CON(Music, ()), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_DESTRUCT, "void f()", WRAP_DES(Music), asCALL_GENERIC); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Load(string &in _file)", WRAP_MFN(Music, Load), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Play(int _nbLoops=-1, int _timeFadeIn=1000)", WRAP_MFN(Music, Play), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void UnLoad()", WRAP_MFN(Music, UnLoad), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Stop()", WRAP_MFN(Music, Stop), asCALL_GENERIC); SDL_assert(r >= 0);

	}

}
#endif


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
#ifdef TRP_USE_AUDIO 		
		Mix_FreeMusic(music);
#endif	
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
#ifdef TRP_USE_AUDIO 		
		Mix_FreeMusic(music);
#endif	
		music = NULL;
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Load(const std::string & _file)
{
#ifdef TRP_USE_AUDIO   	
	music = Mix_LoadMUS_RW(g_app->resourceManager->Load(_file, GAMEDATA | BOTH)); //todo check this FreeSrc ?
#endif	

	if (music == NULL)
	{
		SDL_Log("Cannot load music %s %s", _file.c_str(), SDL_GetError());
	}
	else
	{
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Music %s loaded sucessfully", _file.c_str());
	}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Play(int _nbLoops, int _timeFadeIn)
{
#ifdef TRP_USE_AUDIO 	
	Mix_PlayMusic(music, _nbLoops);
#endif	
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Music::Stop()
{
#ifdef TRP_USE_AUDIO 	
	Mix_HaltMusic();
#endif	
}
