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
#include "Application.h"
#include "Widget.h"
#include "Vector2D.h"


#include "binding/aswrappedcall.h"


#include "ScriptManager.h"

#include "SDL.h"



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Widget *Widget_Factory()
{
	return NULL;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef TRP_USE_BINDING
void RegisterWidget()
{
	int r;

	if (strstr(asGetLibraryOptions(), "AS_MAX_PORTABILITY") == 0)
	{
		r = ScriptManager::Get().engine->RegisterObjectType("Widget", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_FACTORY, "Widget@ f()", asFUNCTION(Widget_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_ADDREF, "void f()", asMETHOD(Widget, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_RELEASE, "void f()", asMETHOD(Widget, Release), asCALL_THISCALL); SDL_assert(r >= 0);
	}
	else
	{
		r = ScriptManager::Get().engine->RegisterObjectType("Widget", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_FACTORY, "Widget@ f()", WRAP_FN(Widget_Factory), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Widget, AddRef), asCALL_GENERIC); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Widget", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Widget, Release), asCALL_GENERIC); SDL_assert(r >= 0);

	}

}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Widget::Widget()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget Constructor\n");

	this->position.x	= 0;
	this->position.y	= 0;
	this->position.w	= 0;
	this->position.h	= 0;
	this->frame.x		= 0;
	this->frame.y		= 0;
	this->frame.w		= 0;
	this->frame.h		= 0;
	this->angle		= 0;
	this->anchor		= 0;

	this->xScale		= 1;
	this->yScale		= 1;

	this->SetScale(1,1);

	this->enabled		= true;
	this->shown		= true;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Widget::~Widget()
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget Destructor\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::Show()
{
	this->shown = true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::Hide()
{
	this->shown = false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::Render()
{
	SDL_assert(false);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void	Widget::SetAnchor(int _value)
{
	this->anchor = _value;
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int	Widget::GetAnchor()
{
	return this->anchor;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetSize(Vector2D _pos)
{
	this->SetSize((int) _pos.x, (int) _pos.y);
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetSize(int _w,int _h)
{
	this->position.w = _w;
	this->position.h = _h;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Vector2D Widget::GetSize()
{
	Vector2D temp;
	temp.x = (float) this->position.w;
	temp.y = (float) this->position.h;
	return temp;
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Vector2D Widget::GetPosition()
{
	Vector2D temp;
	temp.x = (float) this->position.x;
	temp.y = (float) this->position.y;
	return temp;
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetPosition(Vector2D _pos)
{
	this->SetPosition((int) _pos.x, (int) _pos.y);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetPosition(int _x,int _y)
{
	switch(this->anchor)
	{
	case ANCHOR_CENTER: //Center
		this->position.x = _x - this->position.w/2;
		this->position.y = _y - this->position.h/2;
		break;

	case ANCHOR_TOPLEFT: //top left
		this->position.x = _x;
		this->position.y = _y;
		break;

	}
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetRotation(float _angle)
{
	this->angle = _angle;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

float Widget::GetRotation()
{
	return this->angle;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetScale(float _xFactor, float _yFactor)
{
	
	int newWidth = (int) (this->frame.w * _yFactor);
	int newHeight = (int) (this->frame.h * _xFactor);
	//this->position.w = this->frame.w * _yFactor;
	//this->position.h = this->frame.h * _xFactor;
	this->position.x  -= (newWidth - this->position.w)/2;
	this->position.y  -= (newHeight - this->position.h)/2;
	
	this->position.w = newWidth;
	this->position.h = newHeight;

	this->xScale = _xFactor;
	this->yScale = _yFactor;
	
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

bool Widget::Touched(int _x,int _y)
{
	int checked_x;
	int checked_y;

	checked_x = _x;
	checked_y = _y;
	if (this->angle != 0)
		{
		//Compute the center of the rectangle
		int center_x = this->position.x + (this->position.w/2);
		int center_y = this->position.y + (this->position.h/2);
		const double PI = 3.141592653589793;

		double c = cos(-this->angle*PI/180);
		double s = sin(-this->angle*PI/180);

		 //UNrotate the point depending on the rotation of the rectangle
		checked_x = (int) ( center_x + c * (_x - center_x) - s * (_y - center_y));
		checked_y = (int) (center_y + s * (_x - center_x) + c * (_y - center_y));
		}
	if ((checked_x > this->position.x) && (checked_x< this->position.x + this->position.w)&&
		(checked_y>this->position.y) && (checked_y< this->position.y + this->position.h))

		return true;
	else
		return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::Update(unsigned int _elapsed)
{

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Widget::OnMouseButtonDown( Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget OnMouseButtonDown\n");
	return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Widget::OnMouseButtonUp( Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget OnMouseButtonUp\n");
	return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::OnMouseMotion( Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget OnMouseMotion\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::OnKeyUp( Event * event)
{
	//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Widget OnMouseMotion\n");
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Widget::SetEnabled(bool _value)
{
	this->enabled = _value;
}
