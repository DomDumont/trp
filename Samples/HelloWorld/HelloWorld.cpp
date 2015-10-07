#include "Rototo.h"



#include "SceneManager.h"
#include "MenuScene.h"
#include "ParticleScene.h"

bool OnClickHandler2(void * _sender,void * _userData)
    {
    UTI_Log("click");
    return true;
    }

class DemoApp : public Application
{
private:
	Font		maFonte;
	Label  		monLabel;
	int         windowX;
	int         windowY;
	Button		buttonBack;

	SceneManager theSceneManager;
	MenuScene    menuScene;
	ParticleScene    particleScene;

	void OnInit()
	{
		
/*		
		UTI_Log("OnInit");

		WND_GetLogicalSize(windowX, windowY);
		  
		//g_app->guiManager->LoadTheme("aeon");
		GUI_LoadTheme("metal");
		
		theSceneManager.ChangeScene(&menuScene);
*/
	}

	void OnUpdate(unsigned int elapsed)
	{
		
	}

	void OnRender(unsigned int elapsed)
	{
		
	
//	theSceneManager.OnRender(elapsed);
	}
};


int main(int argc, char *argv[])
  {
	  RegisterApplication(new DemoApp());

	  //GetApp()->SetTitle("HelloWorld"); //TODO APP_SetTitle is better no ?
	  return 0;
  }