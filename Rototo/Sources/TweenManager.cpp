#include "Global.h"



#include "TweenManager.h"
#include "Application.h"


#ifdef __EMSCRIPTEN__
#include "binding\aswrappedcall.h"
#endif

#include "ScriptManager.h"


TweenManager& TweenManager::Get()
{
	static TweenManager foo;
	return foo;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void 	TWN_AddTween(Tween * _tween)
{
	TweenManager::Get().AddTween(_tween);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void 	TWN_CancelTweens()
{
	TweenManager::Get().Shutdown();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
TweenedFloat *TweenedFloat_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new TweenedFloat();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Tween *Tween_Factory()
{
	// The class constructor is initializing the reference counter to 1
	return new Tween();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Tween::Init(float _duration,int _effect,int _easeMode)
{
	this->duration = _duration;
    this->equation = _easeMode;
    this->transition = _effect ;
}


/*----------------------------------------------------------------------------*/
void Tween::SetOnCompleteHandlerScript(void * cb)
{

}

/*----------------------------------------------------------------------------*/
void Tween::SetUserDataScript(void * ud)
{

}

/*
void ConstructTween(Tween *thisPointer)
{
	new (thisPointer) Tween();
}

void DestructTween(Tween *thisPointer)
{
	thisPointer->~Tween();
}

void CopyConstructTween(const Tween &other, Tween *thisPointer)
{
	new(thisPointer) Tween(other);
}
*/

void TweenManager::Init()
{
}

void TweenManager::Shutdown()
{
	for (tweensIT = tweens.begin();  tweensIT != tweens.end(); ++tweensIT ) 
		{
		RemoveTween(*tweensIT);
			tweensIT = tweens.begin(); //TODO I don't like this but ...
			if (tweens.size()==0)
				break;
		}
}

void TweenManager::Update(Uint64 _elapsed)
{

	for (tweensIT = tweens.begin();  tweensIT != tweens.end(); ++tweensIT ) 
		{
		Tween *pTemp;
		pTemp = *tweensIT;
		//Update timeElapsed
		(pTemp->timeElapsed)+=_elapsed;
		if (pTemp->timeElapsed < pTemp->duration  ) 
			{
			//there is still something to do because timeelapsed < duration
			for (pTemp->propsIT = pTemp->properties.begin();  pTemp->propsIT != pTemp->properties.end(); ++(pTemp->propsIT) ) 
				{
				TweenedFloat *pFloat = *(pTemp->propsIT);
                float  res   = this->RunEquation(
                                            pTemp->transition,
                                            pTemp->equation,
                                            pTemp->timeElapsed,
                                            pFloat->initialValue,
                                            (pFloat->targetValue - pFloat->initialValue ),
                                            pTemp->duration);
				pFloat->value = res;
				}
			}
		else
			{
			//Call Callback
#ifdef TRP_USE_BINDING
			if (pTemp->onCompleteHandler != NULL)
				{
					pTemp->sender.Set(pTemp, ScriptManager::Get().engine->GetObjectTypeByName("Tween"));
					ScriptManager::Get().RunCallback(pTemp->onCompleteHandler, &(pTemp->sender), &(pTemp->userData));
        pTemp->sender.Set(NULL,NULL);
				}
#endif
			RemoveTween(*tweensIT);
			tweensIT = tweens.begin(); //TODO I don't like this but ...
			if (tweens.size()==0)
				break;
			}
		}
}


void TweenManager::AddTween(Tween* _tween) 
{
//TEST	_tween->AddRef();//TODO check if necessary

	_tween->timeElapsed = 0; //Reset the time elapsed on this tween


	tweens.push_back(_tween);


}

void TweenManager::RemoveTween(Tween  *param) 
{

	int total_tweens = tweens.size();
	tweensIT = tweens.begin();
	for (int i=0; i <  total_tweens; i++,tweensIT++) 
		{
		if (param == (*tweensIT)) 
			{
			//We don't use the tween anymore, we can release it
			param->Release();
			//TODO (*tweensIT).CleanProperties();
			tweens.erase(tweensIT);						
			break;
			}
		}

}

float TweenManager::RunEquation(int transition,int equation, float t,float b , float c, float d) 
{

	float result = 0;
	if (equation == E_EASE_IN)
        {
		result = funcs[transition]->EaseIn(t,b,c,d);
        }
    else if (equation == E_EASE_OUT)
        {
		result = funcs[transition]->EaseOut(t,b,c,d);
        }
    else if (equation == E_EASE_IN_OUT)
        {
		result = funcs[transition]->EaseInOut(t,b,c,d);
        }

	return result;
}



	/***** LINEAR ****/
	float CLinearEffect::EaseNone (float t,float b , float c, float d) {
		return c*t/d + b;
	}
	float CLinearEffect::EaseIn (float t,float b , float c, float d) {
		return c*t/d + b;
	}
	float CLinearEffect::EaseOut(float t,float b , float c, float d) {
		return c*t/d + b;
	}

	float CLinearEffect::EaseInOut(float t,float b , float c, float d) {
		return c*t/d + b;
	}

	/***** SINE ****/

	float CSineEffect::EaseIn (float t,float b , float c, float d) {
		return (float)(-c * cos(t/d * (C_PI/2)) + c + b);
	}
	float CSineEffect::EaseOut(float t,float b , float c, float d) {
		return (float)(c * sin(t/d * (C_PI/2)) + b);
	}

	float CSineEffect::EaseInOut(float t,float b , float c, float d) {
		return (float)(-c/2 * (cos(C_PI*t/d) - 1) + b);
	}

	/**** Quint ****/

	float CQuintEffect::EaseIn (float t,float b , float c, float d) {
		t = t/d;
		return c*t*t*t*t*t + b;
	}
	float CQuintEffect::EaseOut(float t,float b , float c, float d) {
		t=t/d-1;
		return c*(t*t*t*t*t + 1) + b;
	}

	float CQuintEffect::EaseInOut(float t,float b , float c, float d) {
		if ((t/=d/2) < 1) 
			return c/2*t*t*t*t*t + b;
		else
		{
		t-=2;
		return c/2*(t*t*t*t*t + 2) + b;
		}
	}

	/**** Quart ****/
	float CQuartEffect::EaseIn (float t,float b , float c, float d) {
		t/=d;
		return c*t*t*t*t + b;
	}
	float CQuartEffect::EaseOut(float t,float b , float c, float d) {
		t=t/d-1;
		return -c * (t*t*t*t - 1) + b;
	}

	float CQuartEffect::EaseInOut(float t,float b , float c, float d) {
		if ((t/=d/2) < 1) 
			return c/2*t*t*t*t + b;
		else
		{
		t-=2;
		return -c/2 * (t*t*t*t - 2) + b;
		}
	}

	/**** Quad ****/
	float CQuadEffect::EaseIn (float t,float b , float c, float d) {
		t/=d;
		return c*t*t + b;
	}
	float CQuadEffect::EaseOut(float t,float b , float c, float d) {
		t/=d;
		return -c *t*(t-2) + b;
	}

	float CQuadEffect::EaseInOut(float t,float b , float c, float d) {
		if ((t/=d/2) < 1) 
			return ((c/2)*(t*t)) + b;
		else
		{
		--t;
		return -c/2 * (((t-2)*t) - 1) + b;
		}
		/*
		 originally return -c/2 * (((t-2)*(--t)) - 1) + b;

		 I've had to swap (--t)*(t-2) due to diffence in behaviour in
		 pre-increment operators between java and c++, after hours
		 of joy
		 */

	}

	/**** Expo ****/

	float CExpoEffect::EaseIn (float t,float b , float c, float d) {
		return (float)((t==0) ? b : c * pow(2, 10 * (t/d - 1)) + b);
	}
	float CExpoEffect::EaseOut(float t,float b , float c, float d) {
		return (float)((t == d) ? b + c : c * (-pow(2, -10 * t / d) + 1) + b);
	}

	float CExpoEffect::EaseInOut(float t,float b , float c, float d) {
		if (t==0) return b;
		if (t==d) return b+c;
		if ((t/=d/2) < 1) 
			return (float) (c/2 * pow(2, 10 * (t - 1)) + b);
		return (float)(c/2 * (-pow(2, -10 * --t) + 2) + b);
	}


	/****  Elastic ****/

	float CElasticEffect::EaseIn (float t,float b , float c, float d) {
		if (t==0) return b;  if ((t/=d)==1) return b+c;
		float p=d*.3f;
		float a=c;
		float s=p/4;
		float postFix =(float)(a*pow(2,10*(t-=1))); // this is a fix, again, with post-increment operators
		return (float)(-(postFix * sin((t*d-s)*(2*C_PI)/p )) + b);
	}

	float CElasticEffect::EaseOut(float t,float b , float c, float d) {
		if (t==0) return b;  if ((t/=d)==1) return b+c;
		float p=d*.3f;
		float a=c;
		float s=p/4;
		return (float)((a*pow(2,-10*t) * sin( (t*d-s)*(2*C_PI)/p ) + c + b));
	}

	float CElasticEffect::EaseInOut(float t,float b , float c, float d) {
		if (t==0) return b;  if ((t/=d/2)==2) return b+c;
		float p=d*(.3f*1.5f);
		float a=c;
		float s=p/4;

		if (t < 1) {
			float postFix =(float)(a*pow(2,10*(t-=1))); // postIncrement is evil
			return (float)(-.5f*(postFix* sin( (t*d-s)*(2*C_PI)/p )) + b);
		}
		float postFix = (float)( a*pow(2,-10*(t-=1))); // postIncrement is evil
		return (float)(postFix * sin( (t*d-s)*(2*C_PI)/p )*.5f + c + b);
	}

	/****  Cubic ****/
	float CCubicEffect::EaseIn (float t,float b , float c, float d) 
	{
		t/=d;
		return c*t*t*t + b;
	}
	float CCubicEffect::EaseOut(float t,float b , float c, float d) 
	{
		t=t/d-1;
		return c*(t*t*t + 1) + b;
	}

	float CCubicEffect::EaseInOut(float t,float b , float c, float d) 
	{
		if ((t/=d/2) < 1) 
			return c/2*t*t*t + b;
		else
		{
		t-=2;
		return c/2*(t*t*t + 2) + b;
		}
	}

	/*** Circ ***/

	float CCircEffect::EaseIn (float t,float b , float c, float d) 
	{
		t/=d;
		return -c * (sqrt(1 - t*t) - 1) + b;
	}
	float CCircEffect::EaseOut(float t,float b , float c, float d) 
	{
		t=t/d-1;
		return c * sqrt(1 - t*t) + b;
	}

	float CCircEffect::EaseInOut(float t,float b , float c, float d) 
	{
		if ((t/=d/2) < 1)
			{ 
			return -c/2 * (sqrt(1 - t*t) - 1) + b;
			}
		else
			{
			t-=2;
			return c/2 * (sqrt(1 - t*t) + 1) + b;
			}
	}

	/****  Bounce ****/
	float CBounceEffect::EaseIn (float t,float b , float c, float d) 
	{
		return c - EaseOut (d-t, 0, c, d) + b;
	}
	float CBounceEffect::EaseOut(float t,float b , float c, float d) 
	{
		if ((t/=d) < (1/2.75f)) {
			return c*(7.5625f*t*t) + b;
		} else if (t < (2/2.75f)) {
			float postFix = t-=(1.5f/2.75f);
			return c*(7.5625f*(postFix)*t + .75f) + b;
		} else if (t < (2.5/2.75)) {
			float postFix = t-=(2.25f/2.75f);
			return c*(7.5625f*(postFix)*t + .9375f) + b;
		} else {
			float postFix = t-=(2.625f/2.75f);
			return c*(7.5625f*(postFix)*t + .984375f) + b;
		}
	}

	float CBounceEffect::EaseInOut(float t,float b , float c, float d) 
	{
		if (t < d/2) return EaseIn (t*2, 0, c, d) * .5f + b;
		else return EaseOut (t*2-d, 0, c, d) * .5f + c*.5f + b;
	}

	/**** Back *****/

	float CBackEffect::EaseIn (float t,float b , float c, float d) 
	{
		float s = 1.70158f;
		float postFix = t/=d;
		return c*(postFix)*t*((s+1)*t - s) + b;
	}
	float CBackEffect::EaseOut(float t,float b , float c, float d) 
	{
		float s = 1.70158f;
		t=t/d-1;
		return c*(t*t*((s+1)*t + s) + 1) + b;
	}

	float CBackEffect::EaseInOut(float t,float b , float c, float d) 
	{
		float s = 1.70158f;
		if ((t/=d/2) < 1) 
			{
			s*=(1.525f);
			return c/2*(t*t*((s+1)*t - s)) + b;
			}
		else
			{
			float postFix = t-=2;
			s*=(1.525f);
			return c/2*((postFix)*t*((s+1)*t + s) + 2) + b;
			}
	}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
	
void RegisterTween()
{
		int r;

#ifndef __EMSCRIPTEN__

		r = ScriptManager::Get().engine->RegisterObjectType("TweenedFloat", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_FACTORY, "TweenedFloat@ f()", asFUNCTION(TweenedFloat_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_ADDREF, "void f()", asMETHOD(TweenedFloat, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_RELEASE, "void f()", asMETHOD(TweenedFloat, Release), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float value", asOFFSET(TweenedFloat, value));
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float targetValue", asOFFSET(TweenedFloat, targetValue)); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float initialValue", asOFFSET(TweenedFloat, initialValue)); SDL_assert(r >= 0);


		r = ScriptManager::Get().engine->RegisterObjectType("Tween", 0, asOBJ_REF); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_FACTORY, "Tween@ f()", asFUNCTION(Tween_Factory), asCALL_CDECL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_ADDREF, "void f()", asMETHOD(Tween, AddRef), asCALL_THISCALL); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_RELEASE, "void f()", asMETHOD(Tween, Release), asCALL_THISCALL); SDL_assert(r >= 0);


		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween", "void SetOnCompleteHandler(CallbackHandler @)", asMETHOD(Tween, SetOnCompleteHandlerScript), asCALL_THISCALL);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween", "void SetUserData(ref @)", asMETHOD(Tween, SetUserDataScript), asCALL_THISCALL);

		/*
		r = ScriptManager::Get().engine->RegisterObjectProperty("Tween", "CallbackHandler @onComplete", asOFFSET(Tween, onCompleteHandler)); SDL_assert(r >= 0);
		r = ScriptManager::Get().engine->RegisterObjectProperty("Tween", "ref @userData", asOFFSET(Tween, userData)); SDL_assert(r >= 0);
		*/

		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween", "void AddProp(TweenedFloat @)", asMETHOD(Tween, AddProp), asCALL_THISCALL);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween", "void Init(float _duration,int _effect,int _easeMode)", asMETHOD(Tween, Init), asCALL_THISCALL);

	///sect:Tweens
	///glob:void TWN_AddTween(Tween @)
		r = ScriptManager::Get().engine->RegisterGlobalFunction("void TWN_AddTween(Tween @)", asFUNCTION(TWN_AddTween), asCALL_CDECL);
	///glob:void TWN_CancelTweens()
		r = ScriptManager::Get().engine->RegisterGlobalFunction("void TWN_CancelTweens()", asFUNCTION(TWN_CancelTweens), asCALL_CDECL);

#else

		r = ScriptManager::Get().engine->RegisterObjectType("TweenedFloat", 0, asOBJ_REF); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_FACTORY, "TweenedFloat@ f()", WRAP_FN(TweenedFloat_Factory), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_ADDREF, "void f()", WRAP_MFN(TweenedFloat,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("TweenedFloat", asBEHAVE_RELEASE, "void f()", WRAP_MFN(TweenedFloat,Release), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float value", asOFFSET(TweenedFloat,value));
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float targetValue", asOFFSET(TweenedFloat,targetValue)); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectProperty("TweenedFloat", "float initialValue", asOFFSET(TweenedFloat,initialValue)); SDL_assert( r >= 0 );


		r = ScriptManager::Get().engine->RegisterObjectType("Tween", 0, asOBJ_REF); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_FACTORY, "Tween@ f()", WRAP_FN(Tween_Factory), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_ADDREF, "void f()", WRAP_MFN(Tween,AddRef), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectBehaviour("Tween", asBEHAVE_RELEASE, "void f()", WRAP_MFN(Tween,Release), asCALL_GENERIC); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectProperty("Tween", "CallbackHandler @onComplete", asOFFSET(Tween, onCompleteHandler)); SDL_assert( r >= 0 );
		r = ScriptManager::Get().engine->RegisterObjectProperty("Tween", "ref @userData", asOFFSET(Tween, userData)); SDL_assert( r >= 0 );

		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween","void AddProp(TweenedFloat @)", WRAP_MFN(Tween, AddProp), asCALL_GENERIC);
		r = ScriptManager::Get().engine->RegisterObjectMethod("Tween","void Init(float _duration,int _effect,int _easeMode)", WRAP_MFN(Tween, Init), asCALL_GENERIC);

	///sect:Tweens
	///glob:void TWN_AddTween(Tween @)
		r = ScriptManager::Get().engine->RegisterGlobalFunction("void TWN_AddTween(Tween @)", WRAP_FN(TWN_AddTween), asCALL_GENERIC);
	///glob:void TWN_CancelTweens()
		r = ScriptManager::Get().engine->RegisterGlobalFunction("void TWN_CancelTweens()", WRAP_FN(TWN_CancelTweens), asCALL_GENERIC);

#endif

}