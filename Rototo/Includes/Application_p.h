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


#ifndef __APPLICATION_P_H__
#define __APPLICATION_P_H__

//TODO replace this with forward declarations ?

#include "SoundManager.h"
#include "TextManager.h"
#include "ScriptManager.h"
#include "TweenManager.h"
#include "ResourceManager.h"
#include "WatchManager.h"
#include "NetworkManager.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "Settings.h"

#define ORIENTATION_PORTRAIT 1
#define ORIENTATION_PAYSAGE  2

#define PHYSICAL_SIZE_X   1024
#define PHYSICAL_SIZE_Y   768

#define LOGICAL_SIZE_X   1024
#define LOGICAL_SIZE_Y   768

#define DONECODE_NOT_DONE 0
#define DONECODE_REAL_QUIT 1
#define DONECODE_RESTART_ONLY 2

class Application
{
public:
	Application();
	virtual ~Application();
	
	void	Init();

	virtual void OnInit() = 0;
	virtual void OnUpdate(unsigned int elapsed) = 0;
	virtual void OnRender(unsigned int elapsed) = 0;

	int		Run();
	void	Shutdown();
	void	HandleEvent( SDL_Event * event, Uint32 *done);

#ifdef TRP_ANDROID
	void	PrepareAndroidEnvironment();
#endif
#ifdef TRP_IOS
	void	PrepareIOSEnvironment();
#endif

	std::string GetTitle() const { return title; }
	void		SetTitle(const std::string& newTitle) { title = newTitle; }



	
public:
#ifdef TRP_USE_PHYSICS
	PhysicsManager		*physicsManager;
#endif
	SoundManager		*soundManager;
	TextManager			*textManager;
#ifdef TRP_USE_BINDING
	ScriptManager		*scriptManager;
#endif
	ResourceManager		*resourceManager;
	WatchManager		*watchManager;
#ifdef TRP_USE_NETWORK
	NetworkManager		*networkManager;
#endif
	GUIManager			*guiManager;

	SDL_Window			*sdlWindow;
	SDL_Renderer		*sdlRenderer;
	TweenManager		*tweenManager; //TODO rename to tweenManager

	std::string			platform;
	
	Uint32				doneCode;

	int					orientation;

	int					capFPS;

	Settings			settings;

private:
	SDL_Event			event;
	std::string			title;

#ifdef TRP_USE_BINDING
	FunctionEntry		*on_init_func;
	FunctionEntry		*on_update_func;
	FunctionEntry		*on_render_func;
#endif
};

#endif