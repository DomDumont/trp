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
	void OnInit()
	{
		UTI_Log("OnInit");

		WND_GetLogicalSize(windowX, windowY);

		g_app->guiManager->LoadTheme("aeon");

		//monLabel.SetText(UTI_GetLanguage());
		//maFonte.Load("fonts/casual.ttf",25);
		//monLabel.SetFont(maFonte);
		monLabel.SetText("French");
		monLabel.SetPosition(windowX / 2, windowY / 2);

		   buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.SetRotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetScale(1.0,1.0);
        buttonBack.on_click_handler = OnClickHandler;
        

        g_app->guiManager->AddWidget(&buttonBack);
	}

	void OnUpdate(Uint64 elapsed)
	{

	}

	void OnRender(Uint64 elapsed)
	{
		WND_Clear();
		monLabel.Render();
		 buttonBack.Render();
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