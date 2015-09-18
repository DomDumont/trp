bool OnAnimationFinishedHandler(ref @ _sender,ref @ _usedData)
	{
	UTI_Log("Anim Finished");
    return true;
	};


class GameScene:Scene
{
	
bool OnClickHandler(ref @ _sender,ref @ _userData)
    {
    theSceneManager.ChangeScene(menuScene);
    return true;
    }

void Init()
	{
	
	myAtlasUI.Load("graphics/sheet");

    buttonBack.SetText("Back");
    buttonBack.SetSize(250,100);
    buttonBack.SetPosition(800,650);    
    buttonBack.set_Rotation(5);
    buttonBack.SetEnabled(true);
    @buttonBack.on_click_handler = CallbackHandler(gameScene.OnClickHandler);
    @buttonBack.user_data = @this;
    buttonBack.SetScale(1.0,1.0);
    GUI_AddWidget(buttonBack);	

	myAtlasBG.LoadFromImage("graphics/back1.jpg");
	monBG.Load(myAtlasBG,"graphics/back1.jpg");
	monBG.SetPosition(windowX/2,windowY/2); // Center of the screen

	myAtlas.Load("graphics/broute");
	myMusic.Load("sounds/invitation.mod");

	animBroute.Load(myAtlas,"broute","%s_%05d.tga",30);
	animBroute.SetFPS(12);
	animBroute.SetPosition(windowX/2,windowY/2); // Center of the screen
	@animBroute.onComplete = @OnAnimationFinishedHandler;
	@animBroute.userData = @animBroute;	

	
	animTortue.Load(myAtlas,"tortue","%s_%05d.tga",15);
	animTortue.SetFPS(12);	
	animTortue.SetPosition(windowX/2,windowY/2); // Center of the screen
	@animTortue.onComplete = @OnAnimationFinishedHandler;
	@animTortue.userData = @animTortue;	

	animBlink.Load(myAtlas,"blink","%s_%05d.tga",10);
	animBlink.SetFPS(12);	
	animBlink.SetPosition(windowX/2,windowY/2); // Center of the screen

	animReverse.Load(myAtlas,"retournormal","%s_%05d.tga",15);
	animReverse.SetFPS(12);	
	animReverse.SetPosition(windowX/2,windowY/2); // Center of the screen

	animBoule.Load(myAtlas,"boule","%s_%05d.tga",15);
	animBoule.SetFPS(12);	
	animBoule.SetPosition(windowX/2,windowY/2); // Center of the screen
	

	sfxRessort.Load("sounds/ressort.ogg");
	sfxPop.Load("sounds/pop.ogg");
	myMusic.Play();
	
	//animBroute.Scale(1.5f,1.5f);
	
	@currentAnim = @animBroute;
	currentAnim.Play(MODE_NORMAL,INFINITE_LOOP);
	stateAnim = 0;
	

	}
	

void OnUpdate(uint64 _delta)
	{
	currentAnim.Update(_delta);
	}	

void OnRender(uint64 _delta)
	{
	WND_ClearWithColor(80,80,80,255);
	monBG.Render();

	currentAnim.Render();
	buttonBack.Render();
	}
	
void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{
	
	if (currentAnim.Touched(_x,_y))
		{
		
		if (stateAnim == 0)
			{
			stateAnim = 1;
			@currentAnim = @animTortue;
			sfxRessort.Play();
			currentAnim.Play(MODE_NORMAL,1);	
			}
		else
		if (stateAnim == 1)
			{
			stateAnim = 2;
			@currentAnim = @animBoule;
			sfxRessort.Play();
			currentAnim.Play(MODE_NORMAL,1);	
			}
		else
		if (stateAnim == 2)
			{
			stateAnim = 3;
			@currentAnim = @animReverse;
			sfxPop.Play();
			currentAnim.Play(MODE_NORMAL,1);	
			}
		else
		if (stateAnim == 3)
			{
			stateAnim = 0;
			@currentAnim = @animBroute;
			currentAnim.Play(MODE_NORMAL,INFINITE_LOOP);	
			}
		
		}
	/*	else
		theSceneManager.ChangeScene(menuScene);*/

	}

void OnShutdown()
	{
	UTI_Log("SHUTDOWN GAME SCENE\n");

	@animBroute.userData = null;
	@animTortue.userData = null;

	animBroute.UnLoad();
	animReverse.UnLoad();
	animTortue.UnLoad();
	animBlink.UnLoad();
	animBoule.UnLoad();

	myMusic.UnLoad();
	
		
	myAtlas.UnLoad();

    @buttonBack.on_click_handler = null;
    @buttonBack.user_data = null;    
    GUI_RemoveWidget(buttonBack);   

	myAtlasUI.UnLoad();
	
	}
	
	Atlas 	myAtlasBG;
	Sprite monBG;
	Atlas 	myAtlas;
	Atlas 	myAtlasUI;
	Music  myMusic;

	Animation animBroute;
	Animation animTortue;
	Animation animBlink;
	Animation animReverse;	
	Animation animBoule;	

	

	Animation @currentAnim;
	int stateAnim;

	Sound	sfxRessort;
	Sound	sfxPop;

	Button buttonBack; 
};