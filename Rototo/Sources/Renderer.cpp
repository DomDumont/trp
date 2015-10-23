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
#include "Renderer.h"
#include "Application.h"
#include "Font.h"
#include "Font_p.h"
#include "Color.h"

#include "Atlas.h"
#include "Sprite.h"

#include "SDL.h"

Renderer& Renderer::Get() {
	static Renderer renderer;
	return renderer;
}

Renderer::Renderer()
{

}

void Renderer::RenderSprite(Sprite * _sprite)
{


	if (_sprite->shown == false)
		return;
	if ((_sprite->entry) && (_sprite->entry->atlas->texture))
	{
		SDL_SetTextureColorMod(_sprite->entry->atlas->texture, _sprite->color.r, _sprite->color.g, _sprite->color.b);
		SDL_SetTextureAlphaMod(_sprite->entry->atlas->texture, _sprite->color.a);
		if (_sprite->nine_patch == false)
		{
			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, (SDL_Rect*)&_sprite->frame, (SDL_Rect*)&_sprite->position, _sprite->angle, NULL, SDL_FLIP_NONE);
		}
		else
		{
			SDL_Rect src_rect;
			SDL_Rect dst_rect;

			int src_w = _sprite->frame.w - (_sprite->nine_rect.x + _sprite->nine_rect.w);
			int src_h = _sprite->frame.h - (_sprite->nine_rect.y + _sprite->nine_rect.h);

			int dst_w = _sprite->position.w - (_sprite->frame.w - _sprite->nine_rect.w);
			int dst_h = _sprite->position.h - (_sprite->frame.h - _sprite->nine_rect.h);

			// 0  1 2
			// 3  4 5
			// 6  7 8

			//OK 0
			src_rect.x = _sprite->frame.x;
			src_rect.y = _sprite->frame.y;
			src_rect.w = _sprite->nine_rect.x;
			src_rect.h = _sprite->nine_rect.y;

			dst_rect.x = _sprite->position.x;
			dst_rect.y = _sprite->position.y;
			dst_rect.w = _sprite->nine_rect.x;
			dst_rect.h = _sprite->nine_rect.y;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 1
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x;
			src_rect.y = _sprite->frame.y;
			src_rect.w = _sprite->nine_rect.w;
			src_rect.h = _sprite->nine_rect.y;

			dst_rect.x = _sprite->position.x + _sprite->nine_rect.x;
			dst_rect.y = _sprite->position.y;
			dst_rect.w = dst_w;
			dst_rect.h = _sprite->nine_rect.y;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 2
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x + _sprite->nine_rect.w; //OK
			src_rect.y = _sprite->frame.y; //OK
			src_rect.w = src_w; //OK
			src_rect.h = _sprite->nine_rect.y; //OK

			dst_rect.x = _sprite->position.x + _sprite->nine_rect.x + dst_w;
			dst_rect.y = _sprite->position.y;
			dst_rect.w = src_w;
			dst_rect.h = _sprite->nine_rect.y; //OK

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 3
			src_rect.x = _sprite->frame.x; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y; //OK
			src_rect.w = _sprite->nine_rect.x;
			src_rect.h = _sprite->nine_rect.h;

			dst_rect.x = _sprite->position.x; //OK
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y; //OK
			dst_rect.w = _sprite->nine_rect.x;
			dst_rect.h = dst_h;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 4
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y; //OK
			src_rect.w = _sprite->nine_rect.w;
			src_rect.h = _sprite->nine_rect.h;

			dst_rect.x = _sprite->position.x + +_sprite->nine_rect.x; //OK
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y; //OK
			dst_rect.w = dst_w;
			dst_rect.h = dst_h;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 5
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x + _sprite->nine_rect.w; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y; //OK
			src_rect.w = src_w; //OK
			src_rect.h = _sprite->nine_rect.h; //OK

			dst_rect.x = _sprite->position.x + _sprite->nine_rect.x + dst_w;
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y;
			dst_rect.w = src_w;
			dst_rect.h = dst_h; //OK

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 6
			src_rect.x = _sprite->frame.x; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y + _sprite->nine_rect.h; //OK
			src_rect.w = _sprite->nine_rect.x;
			src_rect.h = src_h;

			dst_rect.x = _sprite->position.x; //OK
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y + dst_h; //OK
			dst_rect.w = _sprite->nine_rect.x;
			dst_rect.h = src_h;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 7
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y + _sprite->nine_rect.h; //OK
			src_rect.w = _sprite->nine_rect.w;
			src_rect.h = src_h;

			dst_rect.x = _sprite->position.x + +_sprite->nine_rect.x; //OK
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y + dst_h; //OK
			dst_rect.w = dst_w;
			dst_rect.h = src_h;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);

			//OK 8
			src_rect.x = _sprite->frame.x + _sprite->nine_rect.x + _sprite->nine_rect.w; //OK
			src_rect.y = _sprite->frame.y + _sprite->nine_rect.y + _sprite->nine_rect.h; //OK
			src_rect.w = src_w; //OK
			src_rect.h = src_h;

			dst_rect.x = _sprite->position.x + _sprite->nine_rect.x + dst_w;
			dst_rect.y = _sprite->position.y + _sprite->nine_rect.y + dst_h; //OK
			dst_rect.w = src_w;
			dst_rect.h = src_h;

			SDL_RenderCopyEx(g_app->sdlRenderer, _sprite->entry->atlas->texture, &(src_rect), &(dst_rect), 0, NULL, SDL_FLIP_NONE);
		}

	}

}


SDL_Texture * Renderer::RenderText(Font * _font, const std::string& _text, Color _color)
{
	float extX;
	float extY;
	_font->GetTextExtent(_text, extX, extY);

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
			y += _font->GetFontHeight();
			continue;
		}

		if (ch < 32 || ch > 128)
			ch = '?';

		stbtt_aligned_quad q;
		stbtt_GetBakedQuad(_font->font_p->cdata,
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
		rect.y = (int)(_font->GetFontHeight() + q.y0);
		rect.w = (int)w;
		rect.h = (int)h;

		SDL_Rect tex;
		tex.x = (int)(q.s0 * 512);
		tex.y = (int)(q.t0 * 512);
		tex.w = (int)((q.s1 - q.s0) * 512);
		tex.h = (int)((q.t1 - q.t0) * 512);

		SDL_RenderCopy(g_app->sdlRenderer, _font->font_p->texture, &tex, &rect);
	}

	SDL_RenderCopy(g_app->sdlRenderer, tempTexture, NULL, NULL); // On fait le rendu de la texture
	SDL_SetRenderTarget(g_app->sdlRenderer, NULL); // On repasse en target normale

	return tempTexture;

}