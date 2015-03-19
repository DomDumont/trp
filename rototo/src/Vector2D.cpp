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



#include "Vector2D.h"
#include "Application.h"
#include <cmath>

Vector2D::Vector2D()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

Vector2D::Vector2D(float _x,float _y)
{
	this->x = _x;
	this->y = _y;
}

Vector2D::~Vector2D()
{
}

Vector2D::Vector2D( const Vector2D& other )
{
	this->x = other.x;
	this->y = other.y;
}

/// Add a vector to this vector.
void Vector2D::operator += (const Vector2D& v)
{
	x += v.x; y += v.y;
}

Vector2D Vector2D::operator+(const Vector2D &v)
{
	return Vector2D(x+v.x,y+v.y);
}

Vector2D Vector2D::operator*(float a)
{
	return Vector2D(x*a,y*a);
}

void Vector2D::operator -= (const Vector2D& v)
{
	x -= v.x; y -= v.y;
}

Vector2D Vector2D::operator-(const Vector2D &v)
{
	return Vector2D(x-v.x,y-v.y);
}

float Vector2D::Length()
{
	return std::sqrt(x * x + y * y);
}

/// Convert this vector into a unit vector. Returns the length.
float Vector2D::Normalize()
{
	float length = Length();
	if (length < FLT_EPSILON)
	{
	return 0.0f;
	}
	float invLength = 1.0f / length;
	x *= invLength;
	y *= invLength;
	
	return length;
}

// Pi radians = 180 degrés. 
// n degres = n * pi/180
void Vector2D::FromPolar(float angle, float magnitude)
{
	float deg = (angle *(float)M_PI )/180.0f;
	//float deg = angle;
	x = magnitude * cos(deg);
	y = magnitude * sin(deg);
}

// Pi radians = 180 degrés. 
// n * radians = n * 180/pi;
float Vector2D::ToAngle()
{
	float temp;
	temp = (float)atan2(this->y,this->x);
	temp = (temp *180.0f/(float)M_PI);
	return temp;
}

void Vector2D::TurnLeft()
{
	float nx = -y;
	float ny = x;

	x = nx;
	y = ny;

}
void Vector2D::TurnRight()
{
	float nx = y;
	float ny = -x;

	x = nx;
	y = ny;
}

bool Vector2D::operator== (const Vector2D  &d2)
{
	if (((d2.x - x) < FLT_EPSILON) && ((d2.y - y) < FLT_EPSILON))
	return true;
	else
	return false;
}

void Vector2D::operator= (const Vector2D & other)
{
	x = other.x;
	y = other.y;
}

void Vector2D::operator *= (float a)
{
	x *= a; y *= a;
}

void ConstructVector2D(Vector2D *thisPointer)
{
	new(thisPointer) Vector2D();
}

void DestructVector2D(Vector2D *thisPointer)
{
	thisPointer->~Vector2D();
}

void CopyConstructorVector2D(Vector2D *thisPointer, const Vector2D& other)
{
	new(thisPointer) Vector2D(other);
}

void ConstructVector2DWithFloats(Vector2D *thisPointer,float _x,float _y)
{
	new(thisPointer) Vector2D(_x,_y);
}
#ifdef TRP_USE_BINDING

void RegisterVector2D()
{
	int r;
	r = g_app->scriptManager->engine->RegisterObjectType("Vector2D", sizeof(Vector2D), asOBJ_VALUE | asOBJ_APP_CLASS_CDK/*asOBJ_POD*/); SDL_assert(r>=0);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Vector2D", asBEHAVE_CONSTRUCT,  "void f()",asFUNCTION(ConstructVector2D), asCALL_CDECL_OBJLAST);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Vector2D", asBEHAVE_CONSTRUCT, "void f(const Vector2D &in)", asFUNCTION(CopyConstructorVector2D),asCALL_CDECL_OBJFIRST);
	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Vector2D", asBEHAVE_CONSTRUCT, "void f(float _x,float _y)", asFUNCTION(ConstructVector2DWithFloats),asCALL_CDECL_OBJFIRST);

	r = g_app->scriptManager->engine->RegisterObjectBehaviour("Vector2D", asBEHAVE_DESTRUCT,   "void f()",asFUNCTION(DestructVector2D),  asCALL_CDECL_OBJLAST);
	r = g_app->scriptManager->engine->RegisterObjectProperty("Vector2D", "float x", asOFFSET(Vector2D,x)); assert( r >= 0 );
	r = g_app->scriptManager->engine->RegisterObjectProperty("Vector2D", "float y", asOFFSET(Vector2D,y)); assert( r >= 0 );
	
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "bool opEquals(const Vector2D &in)", asMETHOD(Vector2D, operator==), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void opMulAssign(float a)", asMETHOD(Vector2D, operator*=), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "Vector2D opMul(float a)", asMETHOD(Vector2D, operator*), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void opAssign(const Vector2D &in)", asMETHOD(Vector2D, operator=), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void opAddAssign(const Vector2D &in)", asMETHOD(Vector2D, operator+=), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "Vector2D opAdd(const Vector2D &in)", asMETHOD(Vector2D, operator+), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void opSubAssign(const Vector2D &in)", asMETHOD(Vector2D, operator-=), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "Vector2D opSub(const Vector2D &in)", asMETHOD(Vector2D, operator-), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "float Length()", asMETHOD(Vector2D, Length), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "float Normalize()", asMETHOD(Vector2D, Normalize), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "float ToAngle()", asMETHOD(Vector2D, ToAngle), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void FromPolar(float _angle, float _magnitude)", asMETHOD(Vector2D, FromPolar), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void TurnLeft()", asMETHOD(Vector2D, TurnLeft), asCALL_THISCALL);
	r = g_app->scriptManager->engine->RegisterObjectMethod("Vector2D", "void TurnRight()", asMETHOD(Vector2D, TurnRight), asCALL_THISCALL);
		
}
#endif
