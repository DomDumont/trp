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
#include "Utils.h"
#include "pugixml.hpp"

#include "SoundManager.h"
#include "TextManager.h"
#include "ResourceManager.h"
#include "GUIManager.h"
#include "WatchManager.h"

#include "Event_p.h"
#include "Event.h"

#include "SDL.h"

Application *g_app;

#ifdef __EMSCRIPTEN__
#include "emscripten/emscripten.h"
#endif


#include "Application_p.h"

Application_p::Application_p()
{
	this->on_init_func = NULL;
	this->on_update_func = NULL;
	this->on_render_func = NULL;
}

void Application_p::Init()
{
#ifdef TRP_USE_BINDING
	//scriptManager->CompileScript("main.rsc");
	ScriptManager::Get().CompileScriptViaBuilder("main.rsc");


	// New register scripts functions before using them
	this->on_init_func = ScriptManager::Get().RegisterScript("void OnInit()", (char*)"");
	this->on_update_func = ScriptManager::Get().RegisterScript("void OnUpdate(uint32 _delta)", (char*)"d");
	this->on_render_func = ScriptManager::Get().RegisterScript("void OnRender(uint32 _delta)", (char*)"d");

	ScriptManager::Get().RunFunctionEntry(this->on_init_func, "");

#endif

}


void Application_p::Update(unsigned int elapsed)
{
#ifdef TRP_USE_BINDING
	ScriptManager::Get().RunFunctionEntry(on_update_func, elapsed);
	ScriptManager::Get().RunFunctionEntry(on_render_func, elapsed);
#endif

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::HandleEvent( Event * event, unsigned short *done)
{
	switch (event->event_p->evt.type)
		{
		case SDL_MULTIGESTURE:
			{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Multi Gesture: x = %f, y = %f, dAng = %f, dR = %f",
				event->event_p->evt.mgesture.x,
				event->event_p->evt.mgesture.y,
				event->event_p->evt.mgesture.dTheta,
				event->event_p->evt.mgesture.dDist);
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "MG: numDownTouch = %i", event->event_p->evt.mgesture.numFingers);

			const double PI = 3.141592653589793; //TODO WTF !!!

#ifdef TRP_USE_BINDING
			ScriptManager::Get().RunScript("void OnMultiGesture(int _numFingers, float _x, float _y, float _theta, float _dist )",

			(char*)"dffff",
			event->event_p->evt.mgesture.numFingers,
			event->event_p->evt.mgesture.x,
			event->event_p->evt.mgesture.y,
			event->event_p->evt.mgesture.dTheta * (180 / PI),
			event->event_p->evt.mgesture.dDist);
#endif
			}
		break;

		case SDL_FINGERDOWN:
			{
			int nbTouchDevices = SDL_GetNumTouchDevices();
			SDL_assert(nbTouchDevices > 0);
			SDL_TouchID touchDevice = SDL_GetTouchDevice(0);
			if (!touchDevice)
					SDL_Log(SDL_GetError());
			int nbDetectedFingers = SDL_GetNumTouchFingers(touchDevice);

			//SDL_FingerID titi = event->event_p->evt.tfinger.fingerId;
			SDL_Log("nb Fingers = %d",nbDetectedFingers);
			}
			break;

#if defined WIN32 || defined TRP_OSX
		case SDL_WINDOWEVENT:
			switch (event->event_p->evt.window.event) 
				{
				case SDL_WINDOWEVENT_CLOSE:
					{
					SDL_Window *window = SDL_GetWindowFromID(event->event_p->evt.window.windowID);
					if (window)
						{
						SDL_DestroyWindow(window);
						}
					}
					break;
				case SDL_WINDOWEVENT_MOVED:
					SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, "Windows Moved (%d,%d)\n", event->event_p->evt.window.data1, event->event_p->evt.window.data2);
					break;
				}
			break;
#endif
		case SDL_KEYUP:
		
			//F6 Restart

			if ((event->event_p->evt.key.keysym.mod & KMOD_CTRL) && (event->event_p->evt.key.keysym.scancode == SDL_SCANCODE_R))
				{
				*done = DONECODE_RESTART_ONLY; //! \TODO change this
#ifdef TRP_USE_NETWORK
				NetworkManager::Get().SendMessageToAllClients("R>\n"); //Restart
#endif
				}

			// F7 Change Screen Orientation

			else
				if ((event->event_p->evt.key.keysym.mod & KMOD_CTRL) && (event->event_p->evt.key.keysym.scancode == SDL_SCANCODE_O))
				{
				if (this->orientation == ORIENTATION_PORTRAIT)
					{
					this->orientation = ORIENTATION_PAYSAGE;
					SDL_RenderSetLogicalSize(sdlRenderer, LOGICAL_SIZE_X,LOGICAL_SIZE_Y);					
					}
				else
					{
					this->orientation = ORIENTATION_PORTRAIT;
					SDL_RenderSetLogicalSize(sdlRenderer, LOGICAL_SIZE_Y, LOGICAL_SIZE_X);
					}
				*done = 2; //! \TODO change this
#ifdef TRP_USE_NETWORK
				NetworkManager::Get().SendMessageToAllClients("R>\n"); //Restart
#endif
				}
			else
			// F12 Launch GameDataFolder ?
			if ((event->event_p->evt.key.keysym.mod & KMOD_CTRL) && (event->event_p->evt.key.keysym.scancode == SDL_SCANCODE_F))
				{
#ifdef WIN32
				{
				char currentDir[512];
				GetBundlePath(currentDir,512);
					
				std::string tempPath;
				tempPath = "explorer ";
				tempPath += currentDir;
				tempPath = tempPath + "\\"+g_app->settings.gamedataURL+"\\";
				system(tempPath.c_str());
				}
#endif
#ifdef TRP_OSX
				{
				char currentDir[512];
				GetBundlePath(currentDir,512);
			
				std::string tempPath;
				std::string command;
				tempPath = currentDir;
				tempPath+= "//" + g_app->settings_gamedataURL + "//";
				command = "osascript -e 'tell app \"Finder\" to open POSIX file \""+ tempPath+"\"'";
				system(command.c_str());
				command = "osascript -e 'tell application \"Finder\" to activate'";
				system(command.c_str());
				}
#endif
				}
			else
				if ((event->event_p->evt.key.keysym.mod & KMOD_CTRL) && (event->event_p->evt.key.keysym.scancode == SDL_SCANCODE_H))
				{
#ifdef WIN32
				{
				char currentDir[512];
				GetBundlePath(currentDir,512);
					
				std::string tempPath;
				tempPath = "explorer ";
				tempPath += currentDir;
				tempPath = tempPath + "\\doc\\en\\trp-manual.html";
				system(tempPath.c_str());
				}
#endif
#ifdef TRP_OSX
				{
				char currentDir[512];
				GetBundlePath(currentDir,512);
				
				std::string tempPath;
				std::string command;
				tempPath = currentDir;
				tempPath+= "//doc//en//trp-manual.html";
				command = "osascript -e 'tell app \"Finder\" to open POSIX file \""+ tempPath+"\"'";
				system(command.c_str());
				//command = "osascript -e 'tell application \"Finder\" to activate'";
				//system(command.c_str());
				}
#endif
				}
			else
				if (event->event_p->evt.key.keysym.scancode == SDL_SCANCODE_S)
				{
				std::string pathToScan = ".//"+g_app->settings.gamedataURL+"//";
				ScanGameData(pathToScan);
				}

#ifdef TRP_USE_BINDING
			ScriptManager::Get().RunScript("void OnKeyUp(uint32 _scancode)", (char*)"d", event->event_p->evt.key.keysym.scancode);
#endif
			
		break;

		case SDL_MOUSEBUTTONDOWN:

			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Mouse button %d pressed at (%d,%d)\n",
				event->event_p->evt.button, event->event_p->evt.button.x, event->event_p->evt.button.y);

#ifdef TRP_USE_BINDING
			ScriptManager::Get().RunScript("void OnTouch(uint32 _button,uint32 _x,uint32 _y)", (char*)"ddd", event->event_p->evt.button, event->event_p->evt.button.x, event->event_p->evt.button.y);
#endif

			break;


		case SDL_QUIT:
			*done = 1;
			break;

		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Application::Application() : application_p(new Application_p)
{
#if defined WIN32 || defined TRP_LINUX
	this->settings.configURL = ".";
#else
	
#if defined TRP_OSX
#if defined DEBUG
	this->settings_configURL = ".";
#else
	{
	char currentDir[512];
	GetBundlePath(currentDir,512);
	this->settings_configURL = currentDir;
	}
#endif
#endif
	
#endif

	
	this->settings.gamedataURL = "gamedata";
	this->settings.autorestart = 1;
	this->settings.verbose = 1;
	this->settings.logtofile = 0;
	this->settings.winpos_x = 50;
	this->settings.winpos_y = 50;
	this->settings.winsize_h = 576;
	this->settings.winsize_w = 768;
	this->settings.serverIP="";
	
	platform = SDL_GetPlatform();
	if (platform == "iOS")
		{
		SDL_SetHint("SDL_HINT_IDLE_TIMER_DISABLED", "1");
		}






#ifdef TRP_USE_PHYSICS
	this->physicsManager  = new PhysicsManager();
#endif	
	doneCode = DONECODE_NOT_DONE;
	
	this->orientation = ORIENTATION_PAYSAGE;
	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeRight" );
	//this->orientation = C_ORIENTATION_PORTRAIT;

	this->title = "Rototo";
	this->sdlWindow = NULL;
	this->sdlRenderer = NULL;
	this->capFPS = -1; //Desactivated //TODO remove this hardcoded value

	this->lasttimeEmscripten = 0;

	this->event = new Event();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Application::~Application()
{
#ifdef TRP_USE_PHYSICS
	delete physicsManager;
#endif


}

#ifdef TRP_ANDROID

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::PrepareAndroidEnvironment()
{
	std::string loadedString;

	loadedString = LoadTextFile("settings.xml",FULLPATH|EXTERNAL);
	if (loadedString.empty())
		{
		SDL_Log("cannot find settings.xml in external storage");
		// we must at least copy the write-here file
		int ret;
		char coucou[10] = "coucou";
		SDL_RWops *out = g_app->resourceManager->Save("writehere.xml",FULLPATH|EXTERNAL);
		SDL_RWwrite(out,coucou,7,1);
		SDL_RWclose(out);
		return;
		}
}

#endif

#ifdef TRP_IOS

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::PrepareIOSEnvironment()
	{

	}
#endif

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::Init()
{
	
#ifdef TRP_ANDROID
	this->PrepareAndroidEnvironment();
#endif

#ifdef TRP_IOS
	this->PrepareIOSEnvironment();
#endif


	settings.Read();


	if (doneCode != DONECODE_RESTART_ONLY)
		{

		if ( SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS ) != 0 )
			{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Couldn't initialize SDL: %s\n",SDL_GetError());
			}
		if (this->settings.verbose != 0)
			SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

		if (this->settings.logtofile != 0)
			UTI_CreateLogFile();

		SoundManager::Get().Init();
		TextManager::Get().Init();	
		ResourceManager::Get().Init();
		WatchManager::Get().Init();
#ifdef TRP_USE_NETWORK
		NetworkManager::Get().Init();
#endif
#ifdef TRP_USE_PHYSICS
		physicsManager->Init();
#endif

#if (defined TRP_IOS) || defined (TRP_ANDROID)

		SDL_CreateWindowAndRenderer(0, 0,SDL_WINDOW_FULLSCREEN_DESKTOP|SDL_WINDOW_BORDERLESS,&sdlWindow, &sdlRenderer);
		SDL_Rect tempRect;
		SDL_RenderGetViewport(this->sdlRenderer, &tempRect);

#else

		if (this->orientation == ORIENTATION_PAYSAGE)
			SDL_CreateWindowAndRenderer(this->settings.winsize_w,this->settings.winsize_h,0, &sdlWindow, &sdlRenderer);
		else
			SDL_CreateWindowAndRenderer(this->settings.winsize_h,this->settings.winsize_w,0, &sdlWindow, &sdlRenderer);

		SDL_SetWindowPosition(sdlWindow,this->settings.winpos_x,this->settings.winpos_y);

#endif

		SDL_SetWindowTitle(sdlWindow, this->title.c_str());
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
		if (this->orientation == ORIENTATION_PAYSAGE)
			SDL_RenderSetLogicalSize(sdlRenderer, LOGICAL_SIZE_X,LOGICAL_SIZE_Y);
		else
			SDL_RenderSetLogicalSize(sdlRenderer, LOGICAL_SIZE_Y, LOGICAL_SIZE_X);

#ifdef TRP_USE_NETWORK
    if ((g_app->platform == "Windows") || (g_app->platform == "Mac OS X") || (g_app->platform == "Linux"))
      {
		  NetworkManager::Get().CreateServer(); //To Test
      }
		else
			{
			if (this->settings.serverIP.empty() == false)
				{
					NetworkManager::Get().ConnectToServer(this->settings.serverIP);
				}
			//	networkManager->ConnectToServer("192.168.0.17");
			}
#endif	

		} // if (doneCode != DONECODE_RESTART_ONLY)

	this->capFPS = -1;

	doneCode = DONECODE_NOT_DONE;
#ifdef TRP_USE_BINDING
	ScriptManager::Get().Init();
#endif
	GUIManager::Get().Init();
	TweenManager::Get().Init();
	application_p->Init();
	this->OnInit();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Application::Run()
{
	/* Main render loop */
#ifndef __EMSCRIPTEN__
	unsigned int lasttime = SDL_GetTicks(); 
	unsigned int beginLoop;
	unsigned int endLoop;
	while (doneCode == DONECODE_NOT_DONE)

		{
		beginLoop = SDL_GetTicks();
#else
	if (lasttimeEmscripten == 0)
		lasttimeEmscripten = emscripten_get_now(); 
	double beginLoop;
	double endLoop;
	beginLoop = emscripten_get_now();
#endif		

		/* Check for events */
		while (SDL_PollEvent(&(event->event_p->evt)))
			{
				GUIManager::Get().HandleEvent(event);
			this->HandleEvent(event, &doneCode);
			}

#ifndef __EMSCRIPTEN__
		unsigned int now = SDL_GetTicks(); 
		unsigned int elapsed = now - lasttime;
		lasttime = now;
#else
		double now =  emscripten_get_now();	
		unsigned int elapsed = (unsigned int) (now -lasttimeEmscripten);
		lasttimeEmscripten = now;
#endif		
			
		WatchManager::Get().Update(elapsed);
#ifdef TRP_USE_NETWORK
		NetworkManager::Get().Update();
#endif
		TweenManager::Get().Update(elapsed);
#ifdef TRP_USE_PHYSICS
		physicsManager->Update(elapsed);
#endif

		application_p->Update(elapsed);

		this->OnUpdate(elapsed);
		this->OnRender(elapsed);

#ifdef TRP_USE_PHYSICS
		physicsManager->Render();
#endif
		SDL_RenderPresent(this->sdlRenderer);
	
#ifndef __EMSCRIPTEN__	
		endLoop = SDL_GetTicks();
			
		// Try to cap fps
		// If we loop too fast, wait a little bit

		if (this->capFPS != -1)
			{
			Uint32 testFPS = (1000/this->capFPS);

			if ((endLoop-beginLoop)<testFPS)
				{
				Uint32 msToWait = (Uint32)(testFPS-(endLoop-beginLoop));
				SDL_Delay(msToWait);
				}
			}
		
		}
#endif

	return doneCode; //TODO 1 = quit 2= restart  change this

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::Shutdown()
{
#ifdef TRP_USE_BINDING
	ScriptManager::Get().RunScript("void OnShutdown()",(char*)"");
#endif

	// Shutdown always done, even in restart only

	TweenManager::Get().Shutdown(); //Must be here to remove potential pending tweens and release them.
#ifdef TRP_USE_BINDING
	ScriptManager::Get().Shutdown();
#endif
	GUIManager::Get().Shutdown(); // Must be here to release the widgets.

	if (doneCode != DONECODE_RESTART_ONLY)
		{
		// Shutdown done only in real quit
#ifdef TRP_USE_PHYSICS
		physicsManager->Shutdown();//TODO check if it must be here
#endif
#ifdef TRP_USE_NETWORK
		NetworkManager::Get().Shutdown();
#endif
		WatchManager::Get().Shutdown();
		ResourceManager::Get().Shutdown();
		TextManager::Get().Shutdown();
		SoundManager::Get().Shutdown();
		SDL_Quit();
		settings.Write(); //TODO check if it must be here

		if (this->settings.logtofile != 0)
		UTI_CloseLogFile();

		}
}

