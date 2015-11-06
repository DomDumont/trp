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


#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//TODO replace this with forward declarations ?


#include "NetworkManager.h"
#include "PhysicsManager.h"
#include "Settings.h"




#include <memory>


#define ORIENTATION_PORTRAIT 1
#define ORIENTATION_PAYSAGE  2

#define PHYSICAL_SIZE_X   1024
#define PHYSICAL_SIZE_Y   768

#define LOGICAL_SIZE_X   1024
#define LOGICAL_SIZE_Y   768

#define DONECODE_NOT_DONE 0
#define DONECODE_REAL_QUIT 1
#define DONECODE_RESTART_ONLY 2

class Application_p;

struct SDL_Window; //TODO change this
struct SDL_Renderer;//TODO change this

class Event;

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
	void	HandleEvent( Event * event, unsigned short *done);

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




	


	SDL_Window			*sdlWindow;
	SDL_Renderer		*sdlRenderer;

	std::string			platform;
	
	unsigned short		doneCode;

	int					orientation;

	int					capFPS;

	Settings			settings;



private:
	Event				*event;
	std::string			title;


	unsigned int lasttimeEmscripten;

private:
	std::unique_ptr<Application_p> application_p; // opaque type here
};

#endif