#include "~/helpers/trp_api.rsc" //Enums, Helpers, etc...

#include "~/helpers/scene_manager.rsc" //Minimal scene manager sample
#include "game_scene.rsc"





int windowX,windowY;

GameScene gameScene;

SceneManager theSceneManager;

void OnInit()
{
WND_SetWindowTitle("Coccinelle");

WND_GetLogicalSize(windowX,windowY);
string ts = "Logical Size =  "+formatInt(windowX,"")+" , "+formatInt(windowY,"");
UTI_Log(ts);
		
theSceneManager.ChangeScene(gameScene);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void OnUpdate(uint64 _delta)
{
theSceneManager.OnUpdate(_delta);
}

void OnKeyUp(uint32 _scancode)
{
}
void OnRender(uint64 _delta)
{
theSceneManager.OnRender(_delta);
}

void OnTouch(uint32 _button,uint32 _x,uint32 _y)
{
/*	
string ts = "OnTouch "+formatInt(_x,"")+","+formatInt(_y,"");
SCR_Log(ts);
menuScene.OnTouch(_button,_x,_y);
*/
theSceneManager.OnTouch(_button,_x,_y);
}


void OnShutdown()
{
UTI_Log("BYE BYE FROM SCRIPT");
theSceneManager.OnShutdown();
}