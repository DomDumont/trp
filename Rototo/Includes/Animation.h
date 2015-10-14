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


#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "Widget.h"
#include <vector>

#ifdef TRP_USE_BINDING
#include <angelscript.h>
#include "binding/scripthandle.h"
#endif

#define C_MODE_NORMAL   0
#define C_MODE_REVERSE  1
#define C_MODE_PINGPONG 2


class Atlas;
class Sprite;

class Animation : public Widget
{
public:
	
	Animation();
	~Animation();
	Animation(const Animation &other);
	void Load(Atlas * _atlas, const std::string& _name,const std::string& _format,int _nbFrames);
	void UnLoad();
	void Update(Uint64 _elapsed);
	void Render();

	void SetFPS(int _fps);
	void Play(int _mode = 0, int _nbLoops = 1);
	void Stop(bool _waitEnd = true);
	bool Touched(int _x,int _y);
	void SetPosition(int _x,int _y);
	void SetScale(double _xFactor,double _yFactor);
	void SetRotation(float _angle);

public:
	void SetUserDataScript(void * userdata);
	void SetSenderScript(void *sender);
	void SetOnCompleteHandlerScript(void * handler);

#ifdef TRP_USE_BINDING
	asIScriptFunction *onCompleteHandler;
	CScriptHandle		userData;
	CScriptHandle		sender;
#endif

private:
	Atlas *							atlas;
	std::vector<Sprite *>			sprites;
	std::vector<Sprite *>::iterator spritesIT;
	float							timeElapsed;
	int								currentFrame;
	int								nbFrames;
	float							msPerFrame;
	int								playing;
	int								mode;
	int								sens;
	bool							stopASAP;
	std::string						name;
		
	

};

Animation	*Animation_Factory();
void		RegisterAnimation();

#endif
