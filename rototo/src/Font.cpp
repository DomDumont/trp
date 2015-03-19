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


#define STB_TRUETYPE_IMPLEMENTATION  // force following include to generate implementation
#include "stb_truetype.h"

Font *Font_Factory()
{
    // The class constructor is initializing the reference counter to 1
    return new Font();
}


Font::Font() : refCount(1) 
	{

	}

Font::Font(const Font &other)
	{

	}

Font::~Font()
{
}

void Font::GetTextExtent(const std::string& _text, float & _x, float &_y)
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

SDL_Texture * Font::Render(const std::string& _text, SDL_Color _color)
  {
  float extX;
  float extY;
  GetTextExtent(_text, extX, extY);

  SDL_Texture * tempTexture = SDL_CreateTexture(g_app->sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, (int)extX, (int)extY);
  SDL_SetTextureColorMod(tempTexture, _color.r, _color.g, _color.b);
  SDL_SetTextureBlendMode(tempTexture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(g_app->sdlRenderer, tempTexture);

  float x = 0;
  float y = -2;
  int textLen = (int)_text.size();
  for (int i = 0; i < textLen; i++)
    {
    unsigned char ch = _text[i];

    if (ch == '\n')
      {
      x = 0;
      y += this->fontHeight;
      continue;
      }

    if (ch < 32 || ch > 128)
      ch = '?';

    stbtt_aligned_quad q;
    stbtt_GetBakedQuad(this->cdata,
                       512,
                       512,
                       ch - 32,  // position of character in font
                       &x,     // current position
                       &y,
                       &q,     // resulted quad
                       1);     // '1' for tex coords for opengl ('0' for d3d)

    float w = q.x1 - q.x0;
    float h = q.y1 - q.y0;



    SDL_Rect rect;
    rect.x = (int)q.x0;
    rect.y = (int)(this->fontHeight + q.y0);
    rect.w = (int)w;
    rect.h = (int)h;

    SDL_Rect tex;
    tex.x = (int)(q.s0 * 512);
    tex.y = (int)(q.t0 * 512);
    tex.w = (int)((q.s1 - q.s0) * 512);
    tex.h = (int)((q.t1 - q.t0) * 512);

    SDL_RenderCopy(g_app->sdlRenderer, this->texture, &tex, &rect);
    }

  SDL_RenderCopy(g_app->sdlRenderer, tempTexture, NULL, NULL); // On fait le rendu de la texture
  SDL_SetRenderTarget(g_app->sdlRenderer, NULL); // On repasse en target normale

  return tempTexture;
  }

void Font::Load(const std::string& _file, int _size,int _flags)
{

unsigned char *bitmap = new unsigned char[512 * 512];

SDL_RWops*  tempFlow = g_app->resourceManager->Load(_file, _flags);
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
  if (this->texture)
    SDL_DestroyTexture(this->texture);
  if (this->cdata != NULL)
    delete[] this->cdata;
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
