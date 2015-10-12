#include "Global.h"


#include "Music.h"
#include "Application.h"

#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif

#include "ScriptManager.h"

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

#ifndef __EMSCRIPTEN__

	///class:Music
	r = ScriptManager::Get().engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
	//this->RegisterClass<Music>("Music");
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ConstructMusic), asCALL_CDECL_OBJLAST); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DestructMusic), asCALL_CDECL_OBJLAST); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Load(string &in _file)", asMETHOD(Music, Load), asCALL_THISCALL); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Play(int _nbLoops=-1, int _timeFadeIn=1000)", asMETHOD(Music, Play), asCALL_THISCALL); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void UnLoad()", asMETHOD(Music, UnLoad), asCALL_THISCALL); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Stop()", asMETHOD(Music, Stop), asCALL_THISCALL); SDL_assert(r >= 0);
#else

	r = ScriptManager::Get().engine->RegisterObjectType("Music", sizeof(Music), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_CONSTRUCT, "void f()", WRAP_CON(Music,()), asCALL_GENERIC); SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("Music", asBEHAVE_DESTRUCT, "void f()", WRAP_DES(Music), asCALL_GENERIC);SDL_assert(r >= 0);

	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Load(string &in _file)", WRAP_MFN(Music, Load), asCALL_GENERIC);SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Play(int _nbLoops=-1, int _timeFadeIn=1000)", WRAP_MFN(Music, Play), asCALL_GENERIC);SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void UnLoad()", WRAP_MFN(Music, UnLoad), asCALL_GENERIC);SDL_assert(r >= 0);
	r = ScriptManager::Get().engine->RegisterObjectMethod("Music", "void Stop()", WRAP_MFN(Music, Stop), asCALL_GENERIC);SDL_assert(r >= 0);

#endif	

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
