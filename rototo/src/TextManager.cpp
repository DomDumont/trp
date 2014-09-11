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



#include "TextManager.h"
#include "Application.h"


/************************************************************************/
/*                                                                      */
/************************************************************************/
TextManager::TextManager()
    {
    }

/************************************************************************/
/*                                                                      */
/************************************************************************/
TextManager::~TextManager()
    {
    }

/************************************************************************/
/*                                                                      */
/************************************************************************/
void TextManager::Init()
    {
    if ( TTF_Init() < 0 ) 
	    {
	    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Couldn't initialize TTF: %s\n",SDL_GetError());
	    }
	    else
	    {
	    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"TTF_Init Ok");
	    }
    }


/************************************************************************/
/*                                                                      */
/************************************************************************/
void TextManager::Shutdown()
    {
	TTF_Quit();
    }
