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


#include "TiledMap.h"

#include "Atlas.h"


#include "pugixml.hpp"
#include "Application.h"
#include "Utils.h"



#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "SDL.h"

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

TiledMap *TiledMap_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new TiledMap();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
TiledMap::TiledMap()
{

	this->atlas = NULL;
	this->tileOffsetX = 0;
	this->tileOffsetY = 0;
	this->tileWidth = 0;
	this->tileHeight = 0;
	this->width = 0;
	this->height = 0;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
TiledMap::~TiledMap()
{
    MY_SAFE_RELEASE(this->atlas);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void TiledMap::UnLoad()
{
    MY_SAFE_RELEASE(this->atlas);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void TiledMap::Load(const std::string& _fullPath)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_string(_fullPath.c_str());
	if (result.status != pugi::status_ok)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "cannot parse xml file %s %s\n", _fullPath.c_str(), SDL_GetError());
	}
}



void TiledMap::Render()
{
    Widget::Render();
    if (this->shown == false)
        return;

}

#ifdef TRP_USE_BINDING

void RegisterTiledMap()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{

		r = ScriptManager::Get().engine->RegisterObjectType("TiledMap", 0, asOBJ_REF); SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_FACTORY, "TiledMap@ f()", asFUNCTION(TiledMap_Factory), asCALL_CDECL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_ADDREF, "void f()", asMETHOD(TiledMap, AddRef), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_RELEASE, "void f()", asMETHOD(TiledMap, Release), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap", "void Load(string &in _fullPath)", asMETHOD(TiledMap, Load), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap", "void Render()", asMETHOD(TiledMap, Render), asCALL_THISCALL);
		SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap", "void UnLoad()", asMETHOD(TiledMap, UnLoad), asCALL_THISCALL);
		SDL_assert(r >= 0);
	}
else
	{
	r = ScriptManager::Get().engine->RegisterObjectType("TiledMap", 0, asOBJ_REF); SDL_assert( r >= 0 );
  
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_FACTORY, "TiledMap@ f()", WRAP_FN(TiledMap_Factory), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_ADDREF, "void f()", WRAP_MFN(TiledMap,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
	r = ScriptManager::Get().engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_RELEASE, "void f()", WRAP_MFN(TiledMap,Release), asCALL_GENERIC); SDL_assert( r >= 0 );
  
	r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap","void Load(string &in _fullPath)", WRAP_MFN(TiledMap, Load),asCALL_GENERIC); SDL_assert( r >= 0 );
	r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap","void Render()", WRAP_MFN(TiledMap, Render),asCALL_GENERIC);SDL_assert( r >= 0 );
	r = ScriptManager::Get().engine->RegisterObjectMethod("TiledMap","void UnLoad()", WRAP_MFN(TiledMap, UnLoad),asCALL_GENERIC); SDL_assert( r >= 0 );
 
	}

}

#endif
