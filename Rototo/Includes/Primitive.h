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

#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__

#include "Widget.h"
#include "Color.h"

struct SDL_Texture; //TODO remove this

class Primitive : public Widget
{
public:
	Primitive();
	~Primitive();

	void SetShape(int _shape);
	void SetAngles(int _start,int _end);
	void SetSize(int _w,int _h);
  void SetPosition(int _x,int _y);
  void SetScale(float _xFactor, float _yFactor);
	void Render();
	void SetColor(unsigned char _r=255,unsigned char _g=255,unsigned char _b=255,unsigned char _a=255);

private:
	void BuildInternalTexture();
public:

	SDL_Texture *   bgTexture;
	Color		    color;
	Color		    colorBG;
    int             shape;
    int             startAngle;
    int             endAngle;

    


};

Primitive *Primitive_Factory();
void RegisterPrimitive();



#endif

