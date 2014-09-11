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
#include "Font.h"
#include "Application.h"

Font *Font_Factory()
{
    // The class constructor is initializing the reference counter to 1
    return new Font();
}


Font::Font() : font(NULL),refCount(1) 
	{

	}

Font::Font(const Font &other)
	{
	this->font = other.font;
	}

Font::~Font()
{
    if (this->font != NULL)
        {
        TTF_CloseFont(font);
        this->font = NULL;
        }
}

void Font::Load(const std::string& _file, int _size,int _flags)
{
    font = TTF_OpenFontRW(g_app->resourceManager->Load(_file,_flags), 1, _size);
    if (font == NULL)
        {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "Couldn't initialize Font: %s\n", SDL_GetError());
        }
}

void Font::UnLoad()
{
    if (this->font != NULL)
        {
        TTF_CloseFont(font);
        this->font = NULL;
        }
}

void RegisterFont()
{
    int r;

    ///class:Font
    r = g_app->scriptManager->engine->RegisterObjectType("Font", 0, asOBJ_REF);
        SDL_assert(r >= 0);

    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Font",
        asBEHAVE_FACTORY, "Font@ f()", asFUNCTION(Font_Factory), asCALL_CDECL);
        SDL_assert(r >= 0);

    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Font", asBEHAVE_ADDREF,
        "void f()", asMETHOD(Font, AddRef), asCALL_THISCALL);
        SDL_assert(r >= 0);

    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Font",
        asBEHAVE_RELEASE, "void f()", asMETHOD(Font, Release), asCALL_THISCALL);
        SDL_assert(r >= 0);

    g_app->scriptManager->RegisterClassMethod("Font",
        "void Load(string &in _name,int _size,int _flags=13)", asMETHOD(Font, Load));

    g_app->scriptManager->RegisterClassMethod("Font",
        "void UnLoad()", asMETHOD(Font, UnLoad));
}
