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

#ifndef __WIDGET_H__
#define __WIDGET_H__

class Vector2D;
class Event;

#define ANCHOR_CENTER  0
#define ANCHOR_TOPLEFT 1

#include "Object.h"
#include "Rect.h"

class Widget: public Object
{
public:
	Widget();
	virtual ~Widget();

	virtual void	Render();
	virtual void	SetSize(int _w,int _h);
	virtual void	SetSize(Vector2D _pos);
	virtual Vector2D GetSize();
	virtual void	SetPosition(int _x,int _y);
	virtual void	SetPosition(Vector2D _pos);
	virtual Vector2D GetPosition();
	virtual bool	Touched(int _x,int _y);
	virtual void	SetRotation(float _angle);
	virtual float	GetRotation();
	virtual void	SetScale(float _xFactor, float _yFactor);
	virtual void	Update(unsigned int _elapsed);
	virtual int		OnMouseButtonDown( Event * event);
	virtual int		OnMouseButtonUp( Event * event);
	virtual void	OnMouseMotion( Event * event);
	virtual void	OnKeyUp( Event * event);
	virtual void	SetEnabled(bool _value);
	virtual void	SetAnchor(int _value);
	virtual int		GetAnchor();
	
	void			Show();
	void			Hide();


	
public:
	Rect	frame;
	Rect	position;
	float		angle;
	int		anchor;
	float		xScale;
	float		yScale;
	bool		enabled;
	bool		shown;

};

Widget *Widget_Factory();
void RegisterWidget();

#endif
