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


#include "TiledMap.h"

#include "Atlas.h"


#include "pugixml.hpp"
#include "Application_p.h"
#include "Utils.h"



/*
void ConstructSprite(Sprite *thisPointer)
{
	new(thisPointer) Sprite();
}

void DestructSprite(Sprite *thisPointer)
{
	thisPointer->~Sprite();
}
*/

TiledMap *TiledMap_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new TiledMap();
}

TiledMap::TiledMap()
{

this->atlas = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
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

	r = g_app->scriptManager->engine->RegisterObjectType("TiledMap", 0, asOBJ_REF); SDL_assert( r >= 0 );
	
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_FACTORY, "TiledMap@ f()", asFUNCTION(TiledMap_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_ADDREF, "void f()", asMETHOD(TiledMap,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("TiledMap", asBEHAVE_RELEASE, "void f()", asMETHOD(TiledMap,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
	
	g_app->scriptManager->RegisterClassMethod("TiledMap","void Load(string &in _fullPath)", asMETHOD(TiledMap, Load));	
	g_app->scriptManager->RegisterClassMethod("TiledMap","void Render()", asMETHOD(TiledMap, Render));
    g_app->scriptManager->RegisterClassMethod("TiledMap","void UnLoad()", asMETHOD(TiledMap, UnLoad));	
}

#endif
