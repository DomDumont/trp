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


#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "ResourceManager.h"

#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation

#include "Font_p.h"

Font *Font_Factory()
{
    // The class constructor is initializing the reference counter to 1
    return new Font();
}



Font_p::Font_p()
{
	this->texture = NULL;
	this->cdata = NULL;
	this->fontHeight = 32.0f;
}


Font::Font() : font_p(new Font_p),refCount(1)
	{
		
	}

Font::Font(const Font &other)
	{
		this->refCount = 1;
	}

Font::~Font()
{
}


float Font_p::GetFontHeight()
{
	return fontHeight;
}


float Font::GetFontHeight()
{
	return font_p->GetFontHeight();
}

void Font::GetTextExtent(const std::string& _text, float & _x, float &_y)
{
	font_p->GetTextExtent(_text, _x, _y);
}

void Font_p::GetTextExtent(const std::string& _text, float & _x, float &_y)
  {
  float maxX = 0;
  float maxY = 0;

  _x = 0;
  _y = this->fontHeight;

  float xStart = 0;
  int textLen = (int)_text.size();
  for (int i = 0; i < textLen; i++)
    {
    unsigned char ch = _text[i];

    if (ch == '\n')
      {
      _x = xStart;
      _y += this->fontHeight;
      continue;
      }

    if (ch < 32 || ch > 128)
      ch = '?';

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(this->cdata,
                       512,
                       512,
                       ch - 32,  // position of character in font
                       &_x,     // current position
                       &_y,
                       &q,     // resulted quad
                       1);     // '1' for tex coords for opengl ('0' for d3d)
    if (_x > maxX)
      maxX = _x;
    if (_y > maxY)
      maxY = _y ;

    }

  _x = maxX;
  _y = maxY;
  }

void Font::Load(const std::string& _file, int _size, int _flags)
{
	font_p->Load(_file, _size, _flags);
}

void Font_p::Load(const std::string& _file, int _size, int _flags)
{

unsigned char *bitmap = new unsigned char[512 * 512];

SDL_RWops*  tempFlow = ResourceManager::Get().Load(_file, _flags);
if (tempFlow != NULL)
  {
  int flowSize = (int)SDL_RWsize(tempFlow);
  unsigned char *raw = new unsigned char[flowSize];
  SDL_RWread(tempFlow, raw, flowSize, 1);
  
  this->cdata = new stbtt_bakedchar[96];

  this->fontHeight = (float) _size;

  stbtt_BakeFontBitmap((const unsigned char*)raw,
                       0,
                       this->fontHeight,
                       bitmap,
                       512,
                       512,
                       32,
                       96,
                       (stbtt_bakedchar*)(this->cdata));

  delete[] raw;
  SDL_RWclose(tempFlow);
  }

unsigned char* bitmap2 = new unsigned char[512 * 512 * 4];
int ss = 512 * 512;
for (int i = 0; i < ss; i++)
  {
  bitmap2[i * 4 + 0] = 0xff;
  bitmap2[i * 4 + 1] = 0xff;
  bitmap2[i * 4 + 2] = 0xff;
  bitmap2[i * 4 + 3] = bitmap[i];
  }
delete [] bitmap;

SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(bitmap2, 512, 512,
                                                4 * 8, 512 * 4, 0xff, 0xff00, 0xff0000, 0xff000000);
this->texture = SDL_CreateTextureFromSurface(g_app->sdlRenderer, surface);


delete[] bitmap2;

SDL_FreeSurface(surface);

//font = TTF_OpenFontRW(g_app->resourceManager->Load(_file, _flags), 1, _size);

}

void Font::UnLoad()
{
	font_p->UnLoad();
}

void Font_p::UnLoad()
{
  if (this->texture)
    SDL_DestroyTexture(this->texture);
  if (this->cdata != NULL)
    delete[] this->cdata;
}

#ifdef TRP_USE_BINDING

void RegisterFont()
{
    int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		///class:Font
		r = ScriptManager::Get().engine->RegisterObjectType("Font", 0, asOBJ_REF);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font",
			asBEHAVE_FACTORY, "Font@ f()", asFUNCTION(Font_Factory), asCALL_CDECL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font", asBEHAVE_ADDREF,
			"void f()", asMETHOD(Font, AddRef), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font",
			asBEHAVE_RELEASE, "void f()", asMETHOD(Font, Release), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Font",
			"void Load(string &in _name,int _size,int _flags=13)", asMETHOD(Font, Load), asCALL_THISCALL);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Font",
			"void UnLoad()", asMETHOD(Font, UnLoad), asCALL_THISCALL);
		SDL_assert(r >= 0);
	}
	else
	{
		r = ScriptManager::Get().engine->RegisterObjectType("Font", 0, asOBJ_REF);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font",
			asBEHAVE_FACTORY, "Font@ f()", WRAP_FN(Font_Factory), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font", asBEHAVE_ADDREF,
			"void f()", WRAP_MFN(Font, AddRef), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Font",
			asBEHAVE_RELEASE, "void f()", WRAP_MFN(Font, Release), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Font",
			"void Load(string &in _name,int _size,int _flags=13)", WRAP_MFN(Font, Load), asCALL_GENERIC);
		SDL_assert(r >= 0);

		r = ScriptManager::Get().engine->RegisterObjectMethod("Font",
			"void UnLoad()", WRAP_MFN(Font, UnLoad), asCALL_GENERIC);
		SDL_assert(r >= 0);
	}

}
#endif