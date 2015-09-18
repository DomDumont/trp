#include "Global.h"
#include "Utils.h"

#include "Application.h"

class DemoApp : Application
{

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