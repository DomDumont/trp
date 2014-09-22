#include "~/helpers/trp_api.rsc" //Enums, Helpers, etc...

#include "~/helpers/scene_manager.rsc" //Minimal scene manager sample

#include "menu_scene.rsc"
#include "helloworld_scene.rsc"
#include "gui_scene.rsc"
#include "physics_scene.rsc"
#include "sprite_scene.rsc"
#include "animation_scene.rsc"
#include "sound_scene.rsc"
#include "music_scene.rsc"
#include "game_scene.rsc"
#include "tween_scene.rsc"
#include "io_scene.rsc"
#include "primitive_scene.rsc"
#include "platform_scene.rsc"
#include "dialog_scene.rsc"
#include "particle_scene.rsc"

//#include "game_scene.as"
//#include "textbox_scene.rsc"




int windowX,windowY;
Font   fonte8;
Font   fonteMed;
Font   fonte35;

array<Scene@> allScenes; 

MenuScene menuScene;	
//TextboxScene       textboxScene;
HelloWorldScene    helloworldScene;
GUIScene           guiScene;
PhysicsScene       physicsScene;
SpriteScene        spriteScene;
AnimationScene     animationScene;
SoundScene         soundScene;
MusicScene         musicScene;
GameScene          gameScene;
TweenScene         tweenScene;
IOScene            ioScene;
PrimitiveScene     primitiveScene;
PlatformScene      platformScene;
DialogScene        dialogScene;
ParticleScene        particleScene;

SceneManager theSceneManager;


void OnInit()
{

    WND_SetWindowTitle("T.R.P. Samples");

    WND_GetLogicalSize(windowX,windowY);
    string ts = "Logical Size =  "+formatInt(windowX,"")+" , "+formatInt(windowY,"");
    UTI_Log(ts);

    //WND_SetLogicalSize(windowX,windowY);

    //GUI_LoadTheme("aeon");
    GUI_LoadTheme("metal");
    //GUI_LoadTheme("minimal");
    //GUI_LoadTheme("metroid");
    //GUI_LoadTheme("castlevania");
    
    fonte35.Load("fonts/casual.ttf",50);
    fonte8.Load("fonts/casual.ttf",25);
    fonteMed.Load("fonts/casual.ttf",40);

allScenes.insertLast(@particleScene);
allScenes.insertLast(@helloworldScene);
allScenes.insertLast(@guiScene);
allScenes.insertLast(@physicsScene);
allScenes.insertLast(@spriteScene);
allScenes.insertLast(@animationScene);
allScenes.insertLast(@soundScene);
allScenes.insertLast(@musicScene);
allScenes.insertLast(@gameScene);
allScenes.insertLast(@tweenScene);
allScenes.insertLast(@ioScene);
allScenes.insertLast(@primitiveScene);
allScenes.insertLast(@platformScene);
allScenes.insertLast(@dialogScene);


//WND_SetOrientation(C_ORIENTATION_PAYSAGE);
theSceneManager.ChangeScene(menuScene);
//theSceneManager.ChangeScene(textboxScene);
//theSceneManager.ChangeScene(physicsScene);
//menuScene.Init();
WND_SetCapFPS(30);
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
string ts = "OnKeyUp "+formatInt(_scancode,"");
UTI_Log(ts);
}
void OnRender(uint64 _delta)
{
//menuScene.Render(_delta);
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

void OnMultiGesture(int _numFingers, double _x, double _y, double _theta, double _dist )
{
theSceneManager.OnMultiGesture(_numFingers, _x, _y, _theta, _dist);    
}

void OnCollide()
{
theSceneManager.OnCollide();    
}

void OnShutdown()
{
    GUI_UnLoadTheme();

    fonte8.UnLoad();
    fonteMed.UnLoad();
    fonte35.UnLoad();
    
    UTI_Log("BYE BYE FROM SCRIPT");
    theSceneManager.OnShutdown();
}