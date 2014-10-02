#include "~/helpers/trp_api.rsc" //Enums, Helpers, etc...

#include "~/helpers/scene_manager.rsc" //Minimal scene manager sample
#include "game_scene.rsc"





int screenSizeX,screenSizeY;

GameScene gameScene;

SceneManager theSceneManager;

void OnInit()
{
WND_SetWindowTitle("Shooter");

WND_GetLogicalSize(screenSizeX,screenSizeY);
string ts = "Logical Size =  "+formatInt(screenSizeX,"")+" , "+formatInt(screenSizeY,"");
UTI_Log(ts);
		
SND_SetMusicVolume(128);
SND_SetSFXVolume(20);		

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
theSceneManager.OnKeyUp(_scancode);
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