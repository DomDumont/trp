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

#include "Application.h"
#include "Utils.h"
#include "pugixml.hpp"

Application *g_app;



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::HandleEvent( SDL_Event * event, Uint32 *done)
{
	switch (event->type)
		{
		case SDL_MULTIGESTURE:
			{
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Multi Gesture: x = %f, y = %f, dAng = %f, dR = %f",
			event->mgesture.x,
			event->mgesture.y,
			event->mgesture.dTheta,
			event->mgesture.dDist);
			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"MG: numDownTouch = %i",event->mgesture.numFingers);

			const double PI = 3.141592653589793; //TODO WTF !!!

#ifdef TRP_USE_BINDING
			scriptManager->RunScript("void OnMultiGesture(int _numFingers, double _x, double _y, double _theta, double _dist )",

			(char*)"dffff",
			event->mgesture.numFingers,
			event->mgesture.x,
			event->mgesture.y,
			event->mgesture.dTheta * (180/PI),
			event->mgesture.dDist);
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

			//SDL_FingerID titi = event->tfinger.fingerId;
			SDL_Log("nb Fingers = %d",nbDetectedFingers);
			}
			break;

#if defined WIN32 || defined TRP_OSX
		case SDL_WINDOWEVENT:
			switch (event->window.event) 
				{
				case SDL_WINDOWEVENT_CLOSE:
					{
					SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
					if (window)
						{
						SDL_DestroyWindow(window);
						}
					}
					break;
				case SDL_WINDOWEVENT_MOVED:
					SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Windows Moved (%d,%d)\n",event->window.data1,event->window.data2);
					break;
				}
			break;
#endif
		case SDL_KEYUP:
		
			//F6 Restart

			if ((event->key.keysym.mod & KMOD_CTRL) && (event->key.keysym.scancode == SDL_SCANCODE_R))
				{
				*done = DONECODE_RESTART_ONLY; //! \TODO change this
#ifdef TRP_USE_NETWORK
				networkManager->SendMessageToAllClients("R>\n"); //Restart
#endif
				}

			// F7 Change Screen Orientation

			else
			if ((event->key.keysym.mod & KMOD_CTRL) &&(event->key.keysym.scancode == SDL_SCANCODE_O))
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
				networkManager->SendMessageToAllClients("R>\n"); //Restart
#endif
				}
			else
			// F12 Launch GameDataFolder ?
			if ((event->key.keysym.mod & KMOD_CTRL) &&(event->key.keysym.scancode == SDL_SCANCODE_F))
				{
#ifdef WIN32
				{
				char currentDir[512];
				GetBundlePath(currentDir,512);
					
				std::string tempPath;
				tempPath = "explorer ";
				tempPath += currentDir;
				tempPath = tempPath + "\\"+g_app->settings_gamedataURL+"\\";
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
			if ((event->key.keysym.mod & KMOD_CTRL) &&(event->key.keysym.scancode == SDL_SCANCODE_H))
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
			if (event->key.keysym.scancode == SDL_SCANCODE_S)
				{
				std::string pathToScan = ".//"+g_app->settings_gamedataURL+"//";
				ScanGameData(pathToScan);
				}

#ifdef TRP_USE_BINDING
			scriptManager->RunScript("void OnKeyUp(uint32 _scancode)",(char*)"d",event->key.keysym.scancode);
#endif
			
		break;

		case SDL_MOUSEBUTTONDOWN:

			SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION,"Mouse button %d pressed at (%d,%d)\n",
					event->button.button, event->button.x, event->button.y);

#ifdef TRP_USE_BINDING
			scriptManager->RunScript("void OnTouch(uint32 _button,uint32 _x,uint32 _y)",(char*)"ddd",event->button.button, event->button.x, event->button.y);
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

Application::Application()
{
#if defined WIN32 || defined TRP_LINUX
	this->settings_configURL = ".";
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

	
	this->settings_gamedataURL = "gamedata";
	this->settings_autorestart = 1;
	this->settings_verbose = 1;
	this->settings_logtofile = 0;
	this->settings_winpos_x = 50;
	this->settings_winpos_y = 50;
	this->settings_winsize_h = 576;
	this->settings_winsize_w = 768;
	this->settings_serverIP="";
	
	platform = SDL_GetPlatform();
	if (platform == "iOS")
		{
		SDL_SetHint("SDL_HINT_IDLE_TIMER_DISABLED", "1");
		}

	this->soundManager	= new SoundManager();
	this->textManager	= new TextManager();
#ifdef TRP_USE_BINDING
	this->scriptManager	= new ScriptManager();
#endif
	this->tweenManager	= new TweenManager();
	this->resourceManager = new ResourceManager(); 
	this->watchManager	= new WatchManager();
#ifdef TRP_USE_NETWORK
	this->networkManager  = new NetworkManager();
#endif
	this->guiManager		= new GUIManager();

#ifdef TRP_USE_PHYSICS
	this->physicsManager  = new PhysicsManager();
#endif	
	doneCode = DONECODE_NOT_DONE;
	
	this->orientation = ORIENTATION_PAYSAGE;
	SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeRight" );
	//this->orientation = C_ORIENTATION_PORTRAIT;

	this->title = "Rototo";
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

Application::~Application()
{
#ifdef TRP_USE_PHYSICS
	delete physicsManager;
#endif
	delete guiManager;
	delete soundManager;
	delete textManager;
#ifdef TRP_USE_BINDING
	delete scriptManager;
#endif
	delete tweenManager;
	delete resourceManager;
	delete watchManager;
#ifdef TRP_USE_NETWORK
	delete networkManager;
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


	this->ReadSettings();


	if (doneCode != DONECODE_RESTART_ONLY)
		{

		if ( SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) != 0 )
			{
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,"Couldn't initialize SDL: %s\n",SDL_GetError());
			}
		if (this->settings_verbose != 0)
			SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

		if (this->settings_logtofile != 0)
			UTI_CreateLogFile();

		soundManager->Init();
		textManager->Init();	
		resourceManager->Init();
		watchManager->Init();
#ifdef TRP_USE_NETWORK
		networkManager->Init();	
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
			SDL_CreateWindowAndRenderer(this->settings_winsize_w,this->settings_winsize_h,0, &sdlWindow, &sdlRenderer);
		else
			SDL_CreateWindowAndRenderer(this->settings_winsize_h,this->settings_winsize_w,0, &sdlWindow, &sdlRenderer);

		SDL_SetWindowPosition(sdlWindow,this->settings_winpos_x,this->settings_winpos_y);

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
      networkManager->CreateServer(); //To Test
      }
		else
			{
			if (this->settings_serverIP.empty() == false)
				{
				networkManager->ConnectToServer(this->settings_serverIP);
				}
			//	networkManager->ConnectToServer("192.168.0.17");
			}
#endif	

		} // if (doneCode != DONECODE_RESTART_ONLY)

	this->capFPS = -1;

	doneCode = DONECODE_NOT_DONE;
#ifdef TRP_USE_BINDING
	scriptManager->Init();
#endif
	guiManager->Init();
	tweenManager->Init();
#ifdef TRP_USE_BINDING
	//scriptManager->CompileScript("main.rsc");
	scriptManager->CompileScriptViaBuilder("main.rsc");


	// New register scripts functions before using them
	on_init_func = scriptManager->RegisterScript("void OnInit()",(char*)"");
	on_update_func = scriptManager->RegisterScript("void OnUpdate(uint64 _delta)",(char*)"L");
	on_render_func = scriptManager->RegisterScript("void OnRender(uint64 _delta)",(char*)"L");

	scriptManager->RunFunctionEntry(on_init_func,"");

#endif

	this->OnInit();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

int Application::Run()
{
	/* Main render loop */

	Uint64 lasttime = SDL_GetTicks(); 
	Uint64 beginLoop;
	Uint64 endLoop;
	while (doneCode == DONECODE_NOT_DONE)
		{
		beginLoop = SDL_GetTicks();

		/* Check for events */
		while (SDL_PollEvent(&event))
			{
			guiManager->HandleEvent(&event);
			this->HandleEvent(&event, &doneCode);
			}

		Uint64 now = SDL_GetTicks(); 
		Uint64 elapsed = now - lasttime;
		lasttime = now;
			
		watchManager->Update(elapsed);
#ifdef TRP_USE_NETWORK
		networkManager->Update();
#endif
		tweenManager->Update(elapsed);
#ifdef TRP_USE_PHYSICS
		physicsManager->Update(elapsed);
#endif

#ifdef TRP_USE_BINDING
		scriptManager->RunFunctionEntry(on_update_func,elapsed);
		scriptManager->RunFunctionEntry(on_render_func,elapsed);
#endif

		this->OnUpdate(elapsed);
		this->OnRender(elapsed);

#ifdef TRP_USE_PHYSICS
		physicsManager->Render();
#endif
		SDL_RenderPresent(this->sdlRenderer);
	
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


	return doneCode; //TODO 1 = quit 2= restart  change this

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::Shutdown()
{
#ifdef TRP_USE_BINDING
	scriptManager->RunScript("void OnShutdown()",(char*)"");
#endif

	// Shutdown always done, even in restart only

	tweenManager->Shutdown(); //Must be here to remove potential pending tweens and release them.
#ifdef TRP_USE_BINDING
	scriptManager->Shutdown();
#endif
	guiManager->Shutdown(); // Must be here to release the widgets.

	if (doneCode != DONECODE_RESTART_ONLY)
		{
		// Shutdown done only in real quit
#ifdef TRP_USE_PHYSICS
		physicsManager->Shutdown();//TODO check if it must be here
#endif
#ifdef TRP_USE_NETWORK
		networkManager->Shutdown();
#endif
		watchManager->Shutdown();
		resourceManager->Shutdown();
		textManager->Shutdown();
		soundManager->Shutdown();
		SDL_Quit();
		this->WriteSettings(); //TODO check if it must be here

		if (this->settings_logtofile != 0)
		UTI_CloseLogFile();

		}
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::ReadSettings()
{
	std::string loadedString;

	loadedString = LoadTextFile("settings.xml",FULLPATH|BOTH);

	if (loadedString.empty())
		return;
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_string(loadedString.c_str());
  pugi::xml_node root = doc.first_child();
  for (pugi::xml_node elem = root.first_child(); elem != NULL; elem = elem.next_sibling())
		{
    SDL_Log(elem.name());
    if (strcmp(elem.name(), "position") == 0)
			{
      this->settings_winpos_x = atoi(elem.attribute("x").as_string());
      this->settings_winpos_y = atoi(elem.attribute("y").as_string());
			}
		else
      if (strcmp(elem.name(), "size") == 0)
			{
      this->settings_winsize_w = atoi(elem.attribute("w").as_string());
      this->settings_winsize_h = atoi(elem.attribute("h").as_string());
			}
		else
      if (strcmp(elem.name(), "editor") == 0)
			{
      this->settings_editorURL = elem.attribute("url").as_string();
      this->settings_editorArgs = elem.attribute("args").as_string();
			}
		else
      if (strcmp(elem.name(), "datafolder") == 0)
			{
      this->settings_gamedataURL = elem.attribute("directory").as_string();
			}
		else
      if (strcmp(elem.name(), "autorestart") == 0)
			{
      this->settings_autorestart = atoi(elem.attribute("value").as_string());
			}
		else
      if (strcmp(elem.name(), "allowdebug") == 0)
			{
      this->settings_allowdebug = atoi(elem.attribute("value").as_string());
			}
		else
      if (strcmp(elem.name(), "verbose") == 0)
			{
      this->settings_verbose = atoi(elem.attribute("value").as_string());
			}
		else
      if (strcmp(elem.name(), "logtofile") == 0)
			{
      this->settings_logtofile = atoi(elem.attribute("value").as_string());
			}
		else
      if (strcmp(elem.name(), "server") == 0)
			{
      this->settings_serverIP = elem.attribute("ip").as_string();
			}

		} // for(tinyxml2::XMLElement* elem ...

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/

void Application::WriteSettings()
{
	
}

