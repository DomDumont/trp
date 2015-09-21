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

#define ANCHOR_CENTER  0
#define ANCHOR_TOPLEFT 1

class Widget
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
	virtual void	SetScale(double _xFactor,double _yFactor);
	virtual void	Update(Uint64 _elapsed);
	virtual int	OnMouseButtonDown( SDL_Event * event);
	virtual int	OnMouseButtonUp( SDL_Event * event);
	virtual void	OnMouseMotion( SDL_Event * event);
	virtual void	OnKeyUp( SDL_Event * event);
	virtual void	SetEnabled(bool _value);
	virtual void	SetAnchor(int _value);
	virtual int	GetAnchor();
	
	void Show();
	void Hide();


	void AddRef()
	{
		// Increase the reference counter
		refCount++;
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget 0x%x AddRef : nb active ref = %d\n",(unsigned int)this,refCount);
	}

	void Release()
	{
		// Decrease ref count and delete if it reaches 0
	refCount --;
		if ( refCount == 0 )
			delete this;
		else
	if ( refCount > 0 )
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget 0x%x Release : nb active ref = %d\n",(unsigned int)this, refCount);
	else
		SDL_assert(0);
	}
	
public:
	SDL_Rect	frame;
	SDL_Rect	position;
	float		angle;
	int		anchor;
	double		xScale;
	double		yScale;
	bool		enabled;
	bool		shown;

protected:

	int refCount;
};

Widget *Widget_Factory();
void RegisterWidget();

#endif
