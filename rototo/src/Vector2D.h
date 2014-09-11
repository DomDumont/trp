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

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

class Vector2D
{
    public:
        Vector2D();
        Vector2D(float _x,float _y);
        ~Vector2D();
        Vector2D( const Vector2D& other );

        float   Length();
        float   Normalize();
        float   ToAngle();
        void    FromPolar(float angle, float magnitude);
        void    TurnLeft();
        void    TurnRight();
    
    
    void operator *= (float a);
    void operator += (const Vector2D& v);
    void operator -= (const Vector2D& v);
    Vector2D operator+(const Vector2D &v);
    Vector2D operator-(const Vector2D &v);
    Vector2D operator*(float a);
    void operator= (const Vector2D & other);
    bool operator== (const Vector2D  &d2);

    public:
        float x;
        float y;
};

void ConstructVector2D(Vector2D *thisPointer);
void ConstructVector2DWithFloats(Vector2D *thisPointer,float _x,float _y);
void CopyConstructorVector2D(Vector2D *thisPointer, const Vector2D& other);

void DestructVector2D(Vector2D *thisPointer);
void RegisterVector2D();

#endif