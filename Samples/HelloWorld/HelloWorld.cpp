#include "Global.h"
#include "Utils.h"

#include "Application.h"
#include "Label.h"
#include "Font.h"

class DemoApp : Application
{
private:
	Font		maFonte;
	Label  		monLabel;
	int             windowX;
	int             windowY;

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
	}

	void OnUpdate(Uint64 elapsed)
	{

	}

	void OnRender(Uint64 elapsed)
	{
		WND_Clear();
		monLabel.Render();
		
	}
};

int main(int argc, char *argv[])
  {
	  g_app = (Application *) new DemoApp();
	  g_app->SetTitle("HelloWorld");

	  int doneCode = DONECODE_NOT_DONE;
	  do
	  {
		  g_app->Init();
		  doneCode = g_app->Run();
		  g_app->Shutdown();
	  } while (doneCode != DONECODE_REAL_QUIT);

	  delete g_app;
	  return 0;
  return 0;
  }