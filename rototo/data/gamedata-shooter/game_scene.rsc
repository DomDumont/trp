#include "entities.rsc"
#include "bullet.rsc"
#include "player.rsc"



class GameScene:Scene
{
Player p;
EntityManager em;
Atlas atlas;
Music  myMusic;
Sound	shot;

int targetAngle = 0;

void Init()
	{
	targetAngle = 0;
	atlas.Load("sheet");
	myMusic.Load("sound/music.mp3");
	shot.Load("sound/shoot-01.wav");

	em.Add(p);
	em.Init();

	myMusic.Play();
	}
	

void OnUpdate(uint64 _delta)
	{
	em.OnUpdate(_delta);
	}	

void OnRender(uint64 _delta)
	{
    WND_ClearWithColor(0,0,0,255);        
	em.OnRender(_delta);
	}

void OnKeyUp(uint32 _scancode)
	{
	Vector2D vel(0.2f,0.3f);
	UTI_Log("Add a bullet keyup");
	Bullet newBullet(p.Position,vel);
	newBullet.Init();	    	
	em.Add(newBullet);	
	gameScene.shot.Play();	
	}

void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{
	Vector2D tempTarget;
	tempTarget.x = _x;tempTarget.y = _y;
	Vector2D direction = tempTarget-p.Position;
	targetAngle = direction.ToAngle();
	//targetAngle = - (atan2(p.Position.x-tempTarget.x,p.Position.y-tempTarget.y)*180/3.1415);		
	}

void OnShutdown()
	{
	}
	


	
};