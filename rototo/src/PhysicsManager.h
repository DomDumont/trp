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

#ifndef __PHYSICS_MANAGER_H__
#define __PHYSICS_MANAGER_H__

#ifdef TRP_USE_PHYSICS
#include "Box2D/Box2D.h"


class Body
{
public:
	Body();
	~Body();
	void AddRef()
	{
		// Increase the reference counter
		refCount++;
		//SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"AddRef : nb active body = %d\n",refCount);
        SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"AddRef : nb active body = %d\n",refCount);
	}

	void Release()
	{
		// Decrease ref count and delete if it reaches 0
        refCount--;
		if ( refCount == 0 )
			delete this;
		else
        if ( refCount > 0 )
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Release : nb active body = %d\n",refCount);
        else
            SDL_assert(0);
            
	}

	void	Create();
    void	Destroy();
	void	SetType(int _type);
	void	SetCenterPosition(int _x,int _y);
	void	SetShapeCircle(int _radius);
	void	SetShapeBox(int _w,int _h);
	void	SetLinearVelocity(int _x,int _y);
	void	SetAngularVelocity(float _omega);
	void	SetRestitution(float _value);
	void	GetPosition(int &_x,int &_y);
	float	GetAngle();
	void	SetAngle(float _value);
    void    ApplyForce(float _x,float _y);
    void    ApplyImpulse(float _x,float _y);


private:
	int refCount;
	b2BodyDef	bodyDef;
	b2Body*		body;
	b2Shape*	shape;
	b2FixtureDef fixtureDef;
};

Body *Body_Factory();
void RegisterBody();



class CPhysicsDebugDraw : public b2Draw
{
public:
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
	void DrawTransform(const b2Transform& xf);
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
    void DrawString(int x, int y, const char* string, ...); 
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

 class CContactListener : public b2ContactListener
  {
    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
  };

class PhysicsManager
{
public:

	PhysicsManager();
	~PhysicsManager();

	void Init();
	void Update(Uint64 _elapsed);
	void Render();
	void Shutdown();
	void ShowDebugDraw(bool _value);
    Body * GetContactA();
    Body * GetContactB();
    void SetGravity(float _x,float _y);

public:
	b2World* world;
    Body*   contactA;
    Body*   contactB;

    CContactListener myContactListener;
	int32 velocityIterations;
	int32 positionIterations;
	CPhysicsDebugDraw m_debugDraw;
	bool showDebugDraw;
};

#endif
#endif
