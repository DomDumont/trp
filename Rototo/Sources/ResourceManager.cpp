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



#include "ResourceManager.h"
#include "Application.h"


#include "SDL.h"

/*----------------------------------------------------------------------------*/

ResourceManager& ResourceManager::Get()
{
	static ResourceManager foo;
	return foo;
}

/*----------------------------------------------------------------------------*/

ResourceManager::ResourceManager()
{
    
}

/*----------------------------------------------------------------------------*/

ResourceManager::~ResourceManager()
{
    
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ResourceManager::Init()
{

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void ResourceManager::Shutdown()
{
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SDL_RWops*  ResourceManager::Load(const std::string & _file, unsigned int _flags)
{
#if defined WIN32 || defined TRP_LINUX || defined TRP_OSX || defined TRP_EMSCRIPTEN
    // On desktop platforms, Internal and external are the same
    // So check only FROM_FULLPATH or FROM_GAMEDATA
    std::string tempFile = "";

    if  (_flags & FULLPATH)
        {
        SDL_assert((_flags & GAMEDATA) == 0);
        tempFile = g_app->settings.configURL + "//" + _file;
        }
    else
        {        
        SDL_assert((_flags & FULLPATH) == 0);
        tempFile = g_app->settings.configURL + "//" + g_app->settings.gamedataURL + "//";
        tempFile += _file;
        }
    
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"tempfile = %s",tempFile.c_str());
    return SDL_RWFromFile(tempFile.c_str(),"rb");

#endif


#ifdef TRP_IOS
    std::string tempFile = "";
    if (_flags & EXTERNAL) // IT means also when BOTH search first in external
        {
        if  (_flags & FULLPATH)
            {
            SDL_assert((_flags & GAMEDATA) == 0);
            tempFile = "..//Documents//" + _file;
            }
        else
            {
            SDL_assert((_flags & FULLPATH) == 0);
            tempFile = "..//Documents//" + g_app->settings_gamedataURL+"//";
            tempFile += _file;
            }
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"tempfile = %s",tempFile.c_str());
        SDL_RWops *tempops;
        tempops = SDL_RWFromFile(tempFile.c_str(),"rb");
        if  ((tempops == NULL) && ((_flags & INTERNAL) != 0))
            {
            // On n'a pas trouvé en externe et on peut chercher en interne
            if  (_flags & FULLPATH)
                {
                SDL_assert((_flags & GAMEDATA) == 0);
                tempFile = _file;
                }
            else
                {
                SDL_assert((_flags & FULLPATH) == 0);
                tempFile = ".//"+g_app->settings_gamedataURL+"//";
                tempFile += _file;
                }
            SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"tempfile = %s",tempFile.c_str());
            return SDL_RWFromFile(tempFile.c_str(),"rb");
            }
        else
            return tempops;
        }
    else
        SDL_assert(false);
#endif

#ifdef TRP_ANDROID
   SDL_RWops* tempRWOPS = NULL;
   std::string tempFile = "";
    if (_flags & EXTERNAL) // IT means also when BOTH search first in external
        {
        if  (_flags & FULLPATH)
            {
            SDL_assert((_flags & GAMEDATA) == 0);
            tempFile = SDL_AndroidGetExternalStoragePath();
            tempFile += "/" + _file;
            }
        else
            {
            SDL_assert((_flags & FULLPATH) == 0);
            tempFile = SDL_AndroidGetExternalStoragePath();
            tempFile += "/"+g_app->settings_gamedataURL+"/" + _file;
            }
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"tempfile = %s",tempFile.c_str());
        SDL_RWops *tempops;
        tempops = SDL_RWFromFile(tempFile.c_str(),"rb");
        if  ((tempops == NULL) && ((_flags & INTERNAL) != 0))
            {
            // On n'a pas trouvé en externe et on peut chercher en interne
        	SDL_Log("cannot find in extern %s so try in intern now ",_file.c_str());
            if  (_flags & FULLPATH)
                {
                SDL_assert((_flags & GAMEDATA) == 0);

                //tempFile = SDL_AndroidGetInternalStoragePath();
                //tempFile += "/" + _file;
                tempFile =  _file;
                }
            else
                {
                SDL_assert((_flags & FULLPATH) == 0);
                //tempFile = SDL_AndroidGetInternalStoragePath();
                //tempFile += "/"+g_app->settings_gamedataURL+"/" + _file;
                tempFile = g_app->settings_gamedataURL+"/" + _file;
                }
            SDL_Log("tempfile = %s",tempFile.c_str());
            tempRWOPS =  SDL_RWFromFile(tempFile.c_str(),"rb");
            SDL_Log("tempRWOPS = 0x%x",tempRWOPS);
            return tempRWOPS;
            }
        else
            return tempops;
        } //external
    else
        SDL_assert(false);       
#endif    
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

SDL_RWops*  ResourceManager::Save(const std::string & _file, unsigned int _flags)
{
#if defined WIN32 || defined TRP_LINUX || defined TRP_OSX || defined TRP_EMSCRIPTEN
    // On desktop platforms Internal and external are the same
    // So check only FROM_FULLPATH or FROM_GAMEDATA
    std::string tempFile;

    if  (_flags & FULLPATH)
        {
        SDL_assert((_flags & GAMEDATA) == 0);
        tempFile = g_app->settings.configURL + "//" + _file;
        }
    else
        {        
        SDL_assert((_flags & FULLPATH) == 0);
        tempFile = g_app->settings.configURL + "//" + g_app->settings.gamedataURL + "//";
        tempFile += _file;
        }

    return SDL_RWFromFile(tempFile.c_str(),"wb"); 
#endif


#ifdef TRP_IOS
    // We can only save to external
    std::string tempFile;
    
    if  (_flags & FULLPATH)
        {
        SDL_assert((_flags & GAMEDATA) == 0);
        tempFile = "..//Documents//" + _file;
        }
    else
        {
        SDL_assert((_flags & FULLPATH) == 0);
        tempFile = "..//Documents//" +g_app->settings_gamedataURL+"//";
        tempFile += _file;
        }
    
    return SDL_RWFromFile(tempFile.c_str(),"wb");
#endif

#ifdef TRP_ANDROID
    // We can only save to external
    std::string tempFile;
    
    if  (_flags & FULLPATH)
        {
        SDL_assert((_flags & GAMEDATA) == 0);
        tempFile = SDL_AndroidGetExternalStoragePath();
        tempFile += "/" + _file;
        }
    else
        {
        SDL_assert((_flags & FULLPATH) == 0);
        tempFile = SDL_AndroidGetExternalStoragePath();
        tempFile += "/"+g_app->settings_gamedataURL+"/" + _file;
        }
    
    return SDL_RWFromFile(tempFile.c_str(),"wb");
#endif    
}

