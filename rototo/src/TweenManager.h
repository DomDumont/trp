#ifndef __TWEEN_MANAGER_H__
#define __TWEEN_MANAGER_H__

#include <vector>

#ifdef TRP_USE_BINDING
#include <angelscript.h>
#include "scripthandle/scripthandle.h"
#endif

#ifndef C_PI
#define C_PI 3.1415926535897932384626433832795
#endif

   enum 
	{
        E_LINEAR_EFFECT, //0
        E_SINE_EFFECT,
        E_QUINT_EFFECT,
        E_QUART_EFFECT,
        E_QUAD_EFFECT,
        E_EXPO_EFFECT,
        E_ELASTIC_EFFECT, //6
        E_CUBIC_EFFECT,
        E_CIRC_EFFECT,
        E_BOUNCE_EFFECT,
        E_BACK_EFFECT
		
    };
    enum 
	{
        E_EASE_IN,
        E_EASE_OUT,
        E_EASE_IN_OUT
	};

class CEasing 
	{
	public:
		CEasing() 
			{
			
			}

        virtual ~CEasing()
            {

            }
		//pure virtual
		virtual float EaseIn(float t,float b , float c, float d)=0;
		virtual float EaseOut(float t,float b , float c, float d)=0;
		virtual float EaseInOut(float t,float b , float c, float d)=0;
		
    };

class CBackEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	

class CBounceEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CCircEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CCubicEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CElasticEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CExpoEffect : public CEasing
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CQuadEffect : public CEasing 
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
	
class CQuartEffect : public CEasing 
	{
		
	public:
		
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CQuintEffect : public CEasing 
	{
        public :
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CSineEffect : public CEasing 
	{
        public :
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	
class CLinearEffect : public CEasing 
	{
        public :
		float EaseNone(float t,float b , float c, float d);
		float EaseIn(float t,float b , float c, float d);
		float EaseOut(float t,float b , float c, float d);
		float EaseInOut(float t,float b , float c, float d);
    };
	

	//TODO c'est pourri ces trucs en static là
	


class TweenedFloat
{
public :
	TweenedFloat()
		{
		// Let the constructor initialize the reference counter to 1
		refCount = 1;
		}

	void AddRef()
	{
		// Increase the reference counter
		refCount++;
	}
	void Release()
	{
		// Decrease ref count and delete if it reaches 0
        refCount --;
		if (refCount == 0 )
			delete this;
        else
        if (refCount > 0 )
  		    SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"AddRef : nb active TweenedFloat  = %d\n",refCount);
        else
            SDL_assert(0);

	}

public:
	float initialValue;
	float value;
	float targetValue;

private:
	int refCount;
	
};

TweenedFloat *TweenedFloat_Factory();


class Tween
{

public:
	Tween()
	{
	timeElapsed = 0;
	duration = 0;
	transition = 0;
	equation = 0;
#ifdef TRP_USE_BINDING
	this->onCompleteHandler = NULL; //Very Important
#endif
	//this->userData = NULL;
	// Let the constructor initialize the reference counter to 1
	refCount = 1;

	}

	Tween(const Tween &other)
	{
	this->timeElapsed = other.timeElapsed;
	this->duration = other.duration;
#ifdef TRP_USE_BINDING
	this->onCompleteHandler = other.onCompleteHandler;
	// Let the constructor initialize the reference counter to 1
  this->sender.Set(NULL,NULL);
  this->userData.Set(NULL,NULL);
#endif
	refCount = 1;
	}

	//Assign
	/*
	Tween &operator=(const Tween &other)
	{
		this->timeElapsed = other.timeElapsed;
		this->duration = other.duration;

		return *this;
	}
	*/



	~Tween()
	{
#ifdef TRP_USE_BINDING      
	if (this->onCompleteHandler != NULL)
		{
		this->onCompleteHandler->Release();
		}
#endif

	// Clean all properties.
	for (this->propsIT = this->properties.begin();  this->propsIT != this->properties.end(); ++(this->propsIT) ) 
				{
				TweenedFloat *pFloat = *(this->propsIT);
				// We can release the prop as we don't use it anymore
				pFloat->Release();
				}
	this->properties.clear();
	}

	void AddRef()
	{
		// Increase the reference counter
		refCount++;
		SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"AddRef : nb active tweens = %d\n",refCount);
	}
	void Release()
	{
		// Decrease ref count and delete if it reaches 0
        refCount--;
		if ( refCount == 0 )
			delete this;
		else
        if ( refCount > 0 )
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Release : nb active tweens = %d\n",refCount);
        else
            SDL_assert(0);
	}
	void Init(float _duration,int _effect,int _easeMode);

	void AddProp(TweenedFloat * _toto)
	{
	//TEST _toto->AddRef(); //TODO check if necesseray
	properties.push_back(_toto);
	}



public:
	float timeElapsed;
	float duration;
	short transition;
	short equation;

	std::vector<TweenedFloat *>  properties;
	std::vector<TweenedFloat *>::iterator propsIT;

#ifdef TRP_USE_BINDING
	asIScriptFunction *onCompleteHandler;
	CScriptHandle		userData;
	CScriptHandle		sender;
#endif

private:
	int refCount;


};

Tween *Tween_Factory();
/*
void ConstructTween(Tween *thisPointer);
void DestructTween(Tween *thisPointer);
void CopyConstructTween(const Tween &other, Tween *thisPointer);
*/


class TweenManager
{

public:
	TweenManager()
	{
		this->funcs[E_LINEAR_EFFECT] = new CLinearEffect();
		this->funcs[E_SINE_EFFECT]  = new CSineEffect();
		this->funcs[E_QUINT_EFFECT] = new CQuintEffect();
		this->funcs[E_QUART_EFFECT] = new CQuartEffect();
		this->funcs[E_QUAD_EFFECT] = new CQuadEffect();
		this->funcs[E_EXPO_EFFECT] = new CExpoEffect();
		this->funcs[E_ELASTIC_EFFECT] = new CElasticEffect();
		this->funcs[E_CUBIC_EFFECT] = new CCubicEffect();
		this->funcs[E_CIRC_EFFECT] =  new CCircEffect();
		this->funcs[E_BOUNCE_EFFECT] =  new CBounceEffect();
		this->funcs[E_BACK_EFFECT] =  new CBackEffect();
	}
	~TweenManager()
	{
		delete this->funcs[E_LINEAR_EFFECT];
		delete this->funcs[E_SINE_EFFECT];
		delete this->funcs[E_QUINT_EFFECT];
		delete this->funcs[E_QUART_EFFECT];
		delete this->funcs[E_QUAD_EFFECT];
		delete this->funcs[E_EXPO_EFFECT];
		delete this->funcs[E_ELASTIC_EFFECT];
		delete this->funcs[E_CUBIC_EFFECT];
		delete this->funcs[E_CIRC_EFFECT];
		delete this->funcs[E_BOUNCE_EFFECT];
		delete this->funcs[E_BACK_EFFECT];
	}

	void Init();
	void Update(Uint64 _elapsed);
	void Shutdown();

	void AddTween(Tween * _tween);
	void RemoveTween(Tween  *_tween);

	float RunEquation(int transition,int equation, float t,float b , float c, float d);

private:
	std::vector<Tween *> tweens;
	std::vector<Tween *>::iterator tweensIT;
	CEasing *funcs[11];




};
#endif
