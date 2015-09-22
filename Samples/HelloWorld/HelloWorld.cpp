#include "Global.h"
#include "Utils.h"

#include "Application.h"
#include "Label.h"
#include "Font.h"
#include "Button.h"
#include "GUIManager.h"

#ifdef __EMSCRIPTEN__
#include "emscripten/emscripten.h"
#endif

#include "SceneManager.h"
#include "MenuScene.h"

bool OnClickHandler(void * _sender,void * _userData)
    {
    UTI_Log("click");
    return true;
    }

class DemoApp : Application
{
private:
	Font		maFonte;
	Label  		monLabel;
	int             windowX;
	int             windowY;
	Button buttonBack; 

	SceneManager theSceneManager;
	MenuScene    menuScene;

	void OnInit()
	{
		UTI_Log("OnInit");

		WND_GetLogicalSize(windowX, windowY);
		  
		//g_app->guiManager->LoadTheme("aeon");
		g_app->guiManager->LoadTheme("metal");
		
		theSceneManager.ChangeScene(&menuScene);

	}

	void OnUpdate(Uint64 elapsed)
	{

	}

	void OnRender(Uint64 elapsed)
	{
	
	
	}
};


void mainLoopForEmscripten()
{
	g_app->Run();		  
}
int main(int argc, char *argv[])
  {
	  g_app = (Application *) new DemoApp();
	  g_app->SetTitle("HelloWorld");

	  int doneCode = DONECODE_NOT_DONE;
#ifndef __EMSCRIPTEN__	  
	  do
	  {
#endif	  	
		  g_app->Init();
	
#ifndef __EMSCRIPTEN__		  
	  	doneCode = g_app->Run();		  
		  g_app->Shutdown();
	  } while (doneCode != DONECODE_REAL_QUIT);
#else
	emscripten_set_main_loop(mainLoopForEmscripten,0,true);	  
#endif	  

	  delete g_app;
	  return 0;
  return 0;
  }