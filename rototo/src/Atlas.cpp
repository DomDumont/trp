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

#include "Atlas.h"
#include "tinyxml2.h"
#include "Application.h"
#include "Utils.h"

using namespace tinyxml2;

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void RegisterAtlas()
{
    int r;
    ///class:Atlas
    ///func:Atlas()
    r = g_app->scriptManager->engine->RegisterObjectType("Atlas", 0, asOBJ_REF); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Atlas", asBEHAVE_FACTORY, "Atlas@ f()", asFUNCTION(Atlas_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Atlas", asBEHAVE_ADDREF, "void f()", asMETHOD(Atlas,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Atlas", asBEHAVE_RELEASE, "void f()", asMETHOD(Atlas,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
    ///func:void Load(string &in _file,int _flags=13)
    g_app->scriptManager->RegisterClassMethod("Atlas","void Load(string &in _file,int _flags=13)", asMETHOD(Atlas, Load));
    ///func:void LoadFromImage(string &in _file,int _flags=13)
    g_app->scriptManager->RegisterClassMethod("Atlas","void LoadFromImage(string &in _file,int _flags=13)", asMETHOD(Atlas, LoadFromImage));
    ///func:void UnLoad()
    g_app->scriptManager->RegisterClassMethod("Atlas","void UnLoad()", asMETHOD(Atlas, UnLoad));

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Atlas *Atlas_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Atlas();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Atlas::Atlas():texture(NULL),refCount(1)
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas Constructor\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Atlas::~Atlas()
{
    //TODO Remove all entries.
    for(Uint32 i = 0;i < atlasEntries.size();i++)
		{
		delete atlasEntries[i];
		}
    atlasEntries.clear();

    if (this->texture != NULL)
        {
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;
        }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Atlas::UnLoad()
{
    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas::UnLoad\n");
    
    for(Uint32 i = 0;i < atlasEntries.size();i++)
		{
		delete atlasEntries[i];
		}
    atlasEntries.clear();
    
    if (this->texture != NULL)
        {
        SDL_DestroyTexture(this->texture);
        this->texture = NULL;
        }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Atlas::LoadFromImage(const std::string& _file,int _flags)
{

    // First load and create the Texture

    std::string fullPath;

    
    // First load and create the Texture
    fullPath = _file ;

    texture = IMG_LoadTexture_RW(g_app->sdlRenderer,g_app->resourceManager->Load(fullPath,GAMEDATA|BOTH),1);//Todo check freesrc
    if (texture == NULL)
        {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"cannot load texture %s\n",_file.c_str());
        return;
        }
    SDL_QueryTexture(texture,NULL,NULL,&(size.w),&(size.h));

	AtlasEntry *pTemp = new AtlasEntry();
	pTemp->name = _file;
	pTemp->frame.x = 0;
	pTemp->frame.y = 0;
	pTemp->frame.w = size.w;
	pTemp->frame.h = size.h;
	pTemp->atlas = this;

	atlasEntries.push_back(pTemp);


	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas <%s> loaded successfully \n",_file.c_str());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Atlas::Load(const std::string& _file,int _flags)
{

    // First load and create the Texture

    std::string fullPath;
	std::string loadedString;
    
    // First load and create the Texture
    fullPath = _file + ".png";

    texture = IMG_LoadTexture_RW(g_app->sdlRenderer,g_app->resourceManager->Load(fullPath,_flags),1);//Todo check freesrc
    if (texture == NULL)
        {
        
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"cannot load texture %s %s\n",_file.c_str(),SDL_GetError());
        return;
        }
    SDL_QueryTexture(texture,NULL,NULL,&(size.w),&(size.h));

    fullPath = _file + ".xml";
    
    loadedString = LoadTextFile(fullPath,_flags);

	XMLDocument doc;
	doc.Parse( loadedString.c_str());
	XMLElement* root = doc.FirstChildElement();
	for(XMLElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
		{
			AtlasEntry *pTemp = new AtlasEntry();
			pTemp->name = elem->Attribute("name");
			pTemp->frame.x = atoi(elem->Attribute("x"));
			pTemp->frame.y = atoi(elem->Attribute("y"));
			pTemp->frame.w = atoi(elem->Attribute("width"));
			pTemp->frame.h = atoi(elem->Attribute("height"));
			pTemp->atlas = this; 

			atlasEntries.push_back(pTemp);

		}

	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Atlas <%s> loaded successfully \n",_file.c_str());
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

AtlasEntry * Atlas::FindEntry(const std::string& _name)
{
	Uint32 i;
	for(i = 0;i < atlasEntries.size();i++)
		{
		if (atlasEntries[i]->name == _name) 
			{
			return atlasEntries[i];
			}
		}
	if(i == atlasEntries.size())
		return NULL;

	SDL_assert(false); //We should not arrive here
	return NULL;
}


