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


#include "PhysicsManager.h"
#include "Application.h"

#include "Primitives.h"

#ifdef TRP_USE_PHYSICS

#define     C_RATIO  (LOGICAL_SIZE_X/10) // le monde box2d fait 10m


Body *Body_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Body();
}

#ifdef TRP_USE_BINDING

void RegisterBody()
{

    int r;
    ///class:Body
    r = g_app->scriptManager->engine->RegisterObjectType("Body", 0, asOBJ_REF); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Body", asBEHAVE_FACTORY, "Body@ f()", asFUNCTION(Body_Factory), asCALL_CDECL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Body", asBEHAVE_ADDREF, "void f()", asMETHOD(Body,AddRef), asCALL_THISCALL); SDL_assert( r >= 0 );
    r = g_app->scriptManager->engine->RegisterObjectBehaviour("Body", asBEHAVE_RELEASE, "void f()", asMETHOD(Body,Release), asCALL_THISCALL); SDL_assert( r >= 0 );
    g_app->scriptManager->RegisterClassMethod("Body","void Create()", asMETHOD(Body, Create));
    g_app->scriptManager->RegisterClassMethod("Body","void Destroy()", asMETHOD(Body, Destroy));
    g_app->scriptManager->RegisterClassMethod("Body","void SetType(int _type)", asMETHOD(Body, SetType));
    g_app->scriptManager->RegisterClassMethod("Body","void SetCenterPosition(int _x,int _y)", asMETHOD(Body, SetCenterPosition));
    g_app->scriptManager->RegisterClassMethod("Body","void SetShapeCircle(int _radius)", asMETHOD(Body, SetShapeCircle));
    g_app->scriptManager->RegisterClassMethod("Body","void SetShapeBox(int _w,int _h)", asMETHOD(Body, SetShapeBox));
    g_app->scriptManager->RegisterClassMethod("Body","void SetLinearVelocity(int _x,int _y)", asMETHOD(Body, SetLinearVelocity));
    g_app->scriptManager->RegisterClassMethod("Body","void SetAngularVelocity(float _omega)", asMETHOD(Body, SetAngularVelocity));	
    g_app->scriptManager->RegisterClassMethod("Body","void SetRestitution(float _value)", asMETHOD(Body, SetRestitution));
    g_app->scriptManager->RegisterClassMethod("Body","void GetPosition(int &out _x,int &out _y)", asMETHOD(Body, GetPosition));
    g_app->scriptManager->RegisterClassMethod("Body","float GetAngle()", asMETHOD(Body, GetAngle));
    g_app->scriptManager->RegisterClassMethod("Body","void SetAngle(float _value)", asMETHOD(Body, SetAngle));
    g_app->scriptManager->RegisterClassMethod("Body","void ApplyForce(float _x,float _y)", asMETHOD(Body, ApplyForce));
    g_app->scriptManager->RegisterClassMethod("Body","void ApplyImpulse(float _x,float _y)", asMETHOD(Body, ApplyImpulse));
    


}
#endif

Body::Body() : refCount(1),body(NULL),shape(NULL)
{
}

Body::~Body()
{
	if (body != NULL)
		{
		g_app->physicsManager->world->DestroyBody(body);
		body = NULL;
		}
	if (this->shape != NULL)
		{
		delete this->shape;
		this->shape = NULL;
		}
}

void	Body::SetType(int _type)
{
	bodyDef.type = (b2BodyType) _type;
}

void	Body::SetCenterPosition(int _x,int _y)
{
	bodyDef.position.Set((float32)_x/C_RATIO,(float32) _y/C_RATIO);
}

void	Body::SetShapeCircle(int _radius)
{
	if (this->shape != NULL)
		{
		delete this->shape;
		this->shape = NULL;
		}

	b2CircleShape * pTemp = new b2CircleShape();
	pTemp->m_radius = (float32) _radius/C_RATIO;
	this->shape = pTemp;
}
void	Body::SetShapeBox(int _w,int _h)
{
	if (this->shape != NULL)
		{
		delete this->shape;
		this->shape = NULL;
		}

	b2PolygonShape *pTemp = new b2PolygonShape();
	
	pTemp->SetAsBox((float32)(_w/2)/C_RATIO,(float32) (_h/2)/C_RATIO);
	this->shape = pTemp;
}

void	Body::SetLinearVelocity(int _x,int _y)
{
	body->SetLinearVelocity(b2Vec2((float)_x/C_RATIO,(float)_y/C_RATIO));
}

void	Body::SetAngularVelocity(float _omega)
{
	body->SetAngularVelocity(_omega);
}
void Body::SetRestitution(float _value)
{
		fixtureDef.restitution = _value;
}

void Body::GetPosition(int &_x,int &_y)
{
	b2Vec2 position = body->GetPosition();
	_x = (int)(position.x*C_RATIO);
	_y = (int)(position.y*C_RATIO);
}

float Body::GetAngle()
{
	return 180 * body->GetAngle() / b2_pi ;
}

void	Body::SetAngle(float _value)
{
	bodyDef.angle = b2_pi * (_value) / 180 ;
}


void Body::Create()
{
	
	//bodyDef.type = b2_dynamicBody;
	//
	
	this->body = g_app->physicsManager->world->CreateBody(&(this->bodyDef));
    this->body->SetUserData(this);




	fixtureDef.shape = this->shape;
	fixtureDef.density = 20.0f;


	//fixtureDef.density = 1.0f;
	//fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
	SDL_assert(body != NULL);
}

void Body::Destroy()
{
    g_app->physicsManager->world->DestroyBody(this->body);
    this->body = NULL;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Body::ApplyForce(float _x,float _y)
{
    b2Vec2 toto(_x,_y);
    this->body->ApplyForce(toto,this->body->GetWorldCenter(),true); //TODO check last param wake == true
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void Body::ApplyImpulse(float _x,float _y)
{
    b2Vec2 toto(_x,_y);
    this->body->ApplyLinearImpulse(toto,this->body->GetWorldCenter(),true); //TODO check last param wake == true
}


//-----------------------------------------------------------------------------
//
// CContactListener
//
//-----------------------------------------------------------------------------



/************************************************************************/
/*                                                                      */
/************************************************************************/
void CContactListener::BeginContact(b2Contact* contact)
	{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Begin Contact");

	//Set variables before calling script

	g_app->physicsManager->contactA = (Body *) contact->GetFixtureA()->GetBody()->GetUserData();
	g_app->physicsManager->contactB = (Body *) contact->GetFixtureB()->GetBody()->GetUserData();

	//call the script
#ifdef TRP_USE_BINDING
	g_app->scriptManager->RunScript("void OnCollide()",(char*)"");
#endif
	}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CContactListener::EndContact(b2Contact* contact)
	{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"End Contact");
	//Set variables before calling script

	g_app->physicsManager->contactA = (Body *) contact->GetFixtureA()->GetBody()->GetUserData();
	g_app->physicsManager->contactB = (Body *) contact->GetFixtureB()->GetBody()->GetUserData();

	//call the script
#ifdef TRP_USE_BINDING
	g_app->scriptManager->RunScript("void OnCollide()",(char*)"");
#endif
	}


//-----------------------------------------------------------------------------
//
// CPhysicsManager
//
//-----------------------------------------------------------------------------

PhysicsManager::PhysicsManager()
{
	showDebugDraw = false;
    this->contactA = NULL;
    this->contactB = NULL;
}

PhysicsManager::~PhysicsManager()
{
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void PhysicsManager::Init()
{
showDebugDraw = false;

// Define the gravity vector.

b2Vec2 gravity(0.0f, +10.0f);
this->velocityIterations = 6; //8
this->positionIterations = 2; // 3


world = new b2World(gravity);
world->SetDebugDraw(&m_debugDraw);
world->SetContactListener(&(this->myContactListener));
m_debugDraw.SetFlags(b2Draw::e_shapeBit|b2Draw::e_aabbBit|b2Draw::e_centerOfMassBit);
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void PhysicsManager::SetGravity(float _x,float _y)
{
b2Vec2 new_gravity(_x, _y);
world->SetGravity(new_gravity);
}

Body * PhysicsManager::GetContactA()
{
    this->contactA->AddRef();
    return this->contactA;
}

Body * PhysicsManager::GetContactB()
{
    this->contactB->AddRef();
    return this->contactB;
}

void PhysicsManager::Update(unsigned int _elapsed)
{
float32 timeStep = (float32) _elapsed/60.0f;
//float32 timeStep = 1.0f / 120.0f;
world->Step(timeStep/10, this->velocityIterations, this->positionIterations);
}

void PhysicsManager::ShowDebugDraw(bool _value)
{
showDebugDraw = _value;
}
void PhysicsManager::Render()
{
if (showDebugDraw == true)
	world->DrawDebugData();
}
void PhysicsManager::Shutdown()
{
delete world;
world = NULL;
}


void CPhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	Sint16 *PX = new Sint16[vertexCount];
	Sint16 *PY = new Sint16[vertexCount];
	for (int32 i = 0; i < vertexCount; i++)
		{
		PX[i] = (Sint16) (vertices[i].x*C_RATIO);
		PY[i] = (Sint16) (vertices[i].y*C_RATIO);
		//PY[i] = (Sint16) (C_LOGICAL_SIZE_Y -vertices[i].y);
		}
	polygonRGBA(g_app->sdlRenderer,PX,PY,vertexCount,(Uint8)(color.r * 255),(Uint8)(color.g * 255),(Uint8)(color.b * 255),255);
    
	delete []PX;
	delete []PY;
}

void CPhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{

	
	Sint16 *PX = new Sint16[vertexCount];
	Sint16 *PY = new Sint16[vertexCount];
	for (int32 i = 0; i < vertexCount; i++)
		{
            Sint16 toto;
            toto = (Sint16) (vertices[i].x * C_RATIO);
            PX[i] = toto;
            toto = (Sint16) (vertices[i].y *C_RATIO);
            PY[i] = toto;
		//PY[i] = (Sint16) (C_LOGICAL_SIZE_Y -vertices[i].y);
		}
	filledPolygonRGBA(g_app->sdlRenderer,PX,PY,vertexCount,(Uint8)(color.r * 255),(Uint8)(color.g * 255),(Uint8)(color.b * 255),255);
    
	delete []PX;
	delete []PY;

}

void CPhysicsDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	/*
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	//glColor3f(color.r, color.g, color.b);
	//glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
		{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
	//	glVertex2f(v.x, v.y);
		theta += k_increment;
		}
	//glEnd();
	*/
}

void CPhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	filledCircleRGBA(g_app->sdlRenderer,(Sint16)(center.x*C_RATIO),(Sint16)(center.y*C_RATIO),(Sint16)(radius*C_RATIO),(Uint8)(color.r * 255),(Uint8)(color.g * 255),(Uint8)(color.b * 255),200);
	b2Vec2 p = center + radius * axis;
	
	lineRGBA(g_app->sdlRenderer,(Sint16)(center.x*C_RATIO),(Sint16)(center.y*C_RATIO),(Sint16)(p.x*C_RATIO),(Sint16)(p.y*C_RATIO),(Uint8)(color.r * 255),(Uint8)(color.g * 255),(Uint8)(color.b * 255),255);

}

void CPhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	//glColor3f(color.r, color.g, color.b);
	//glBegin(GL_LINES);
	//glVertex2f(p1.x, p1.y);
	//glVertex2f(p2.x, p2.y);
	//glEnd();
	SDL_RenderDrawLine(g_app->sdlRenderer,(int)(p1.x*C_RATIO), (int)(p1.y*C_RATIO),(int)(p2.x*C_RATIO),(int)( p2.y*C_RATIO));
}

void CPhysicsDebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	//glBegin(GL_LINES);
	//
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	//glVertex2f(p2.x, p2.y);

	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	//glVertex2f(p2.x, p2.y);

	//glEnd();
}

void CPhysicsDebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	//SDL_SetRenderDrawColor(g_app->sdlRenderer,color.r,color.g,color.b,255);
	SDL_RenderDrawPoint(g_app->sdlRenderer,(int)(p.x),(int)(p.y));

	//glPointSize(size);
	//glBegin(GL_POINTS);
	//glColor3f(color.r, color.g, color.b);
	//glVertex2f(p.x, p.y);
	//glEnd();
	//glPointSize(1.0f);
}

void CPhysicsDebugDraw::DrawString(int x, int y, const char *string, ...)
{
	//char buffer[128];

	//va_list arg;
	//va_start(arg, string);
	//vsprintf(buffer, string, arg);
	//va_end(arg);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//int w = glutGet(GLUT_WINDOW_WIDTH);
	//int h = glutGet(GLUT_WINDOW_HEIGHT);
	//gluOrtho2D(0, w, h, 0);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

	//glColor3f(0.9f, 0.6f, 0.6f);
	//glRasterPos2i(x, y);
	//int32 length = (int32)strlen(buffer);
	//for (int32 i = 0; i < length; ++i)
	//{
	//	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
	//}

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
}

void CPhysicsDebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	//glColor3f(c.r, c.g, c.b);
	//glBegin(GL_LINE_LOOP);
	//glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	//glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	//glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	//glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	//glEnd();
}

#endif
