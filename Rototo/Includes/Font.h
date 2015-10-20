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



#ifndef __FONT_H__
#define __FONT_H__

#include <string>
#include <memory>



class Font_p;
class Font
{
	friend class Renderer;
 public:
    Font();
    ~Font();
    Font(const Font &other);
    void Load(const std::string& _file, int _size, int _flags = 13 /*GAMEDATA|BOTH*/);
    void UnLoad();


	void AddRef();
	void Release();


	void GetTextExtent(const std::string& _text, float & _x, float &_y);
	float GetFontHeight();

private:
	std::unique_ptr<Font_p> font_p; // opaque type here
  
  

 private:
    int               refCount;

 
};


Font *Font_Factory();
void RegisterFont();

#endif
