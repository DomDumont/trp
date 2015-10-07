class PhysicsScene:Scene
	{
/*
	bool OnClickHandler(ref @ _sender,ref @ _userData)
		{
		theSceneManager.ChangeScene(menuScene);
        return true;
		}

	bool OnClickHandlerDD(ref @ _sender,ref @ _userData)
		{
		showDebugDraw = !showDebugDraw;
		PHY_ShowDebugDraw(showDebugDraw);
		return true;
		}

	bool OnClickHandlerRestart(ref @ _sender,ref @ _userData)
		{
		OnShutdown();
		Init();
		return true;
		}
*/
	void Init()
		{

/*
		UTI_Log("====> physicsScene Init");
		//showDebugDraw = true;

		PHY_ShowDebugDraw(showDebugDraw);
        PHY_SetGravity(0,2.0f);

		myAtlas.Load("graphics/sheet");	
		
		monSprite.Load(myAtlas,"sheep.png");
		monSprite.SetScale(0.25,0.25);
		

		
		myAtlasBG.LoadFromImage("graphics/apple-bg.png");
		monBG.Load(myAtlasBG,"graphics/apple-bg.png");
		monBG.SetPosition(windowX/2,windowY/2); // Center of the screen



		
		
		labelTitle.SetText("Physics Sample");
		labelTitle.SetPosition(360,100);

		
		buttonBack.SetText("Back");
		
		buttonBack.SetSize(250,100);
		buttonBack.SetPosition(800,650);			
		buttonBack.set_Rotation(5);
		buttonBack.SetEnabled(true);
		@buttonBack.on_click_handler = CallbackHandler(physicsScene.OnClickHandler);
		@buttonBack.user_data = @this;
		buttonBack.SetScale(1.0,1.0);
		GUI_AddWidget(buttonBack);



		
	

		
		buttonDebugDraw.SetText("Toggle Debug Draw");
		
		buttonDebugDraw.SetSize(380,50);
		buttonDebugDraw.SetPosition(300,600);			
		buttonDebugDraw.set_Rotation(0);
		buttonDebugDraw.SetEnabled(true);
		@buttonDebugDraw.on_click_handler = CallbackHandler(physicsScene.OnClickHandlerDD);
		@buttonDebugDraw.user_data = @this;
		buttonDebugDraw.SetScale(1.0,1.0);
		GUI_AddWidget(buttonDebugDraw);


		buttonRestart.SetText("Restart");		
		buttonRestart.SetSize(380,50);
		buttonRestart.SetPosition(300,700);	
		buttonRestart.set_Rotation(0);
		buttonRestart.SetEnabled(true);
		@buttonRestart.on_click_handler = CallbackHandler(physicsScene.OnClickHandlerRestart);
		@buttonRestart.user_data = @this;
		buttonRestart.SetScale(1.0,1.0);
		GUI_AddWidget(buttonRestart);

		box.SetType(DYNAMIC_BODY);
		box.SetShapeCircle(25);
		box.SetCenterPosition(windowX/4,100);
		box.SetRestitution(0.3);
		box.Create();
		//box.SetLinearVelocity(0,+10);
		//box.SetAngularVelocity(1.0f);

		buttonBox.SetType(STATIC_BODY);
		buttonBox.SetShapeBox(250,100);
		buttonBox.SetCenterPosition(800,650);
		buttonBox.SetAngle(5.0f);
		buttonBox.Create();

		ground.SetType(STATIC_BODY);
		ground.SetShapeBox(500,20);
		ground.SetCenterPosition(400,450);
		ground.SetAngle(5.0f);
		ground.Create();
*/
		}
		
	void OnUpdate(uint64 _delta)
		{	
/*		
		int x,y;
		box.GetPosition(x,y);
		monSprite.SetPosition(x,y);
		monSprite.set_Rotation(box.GetAngle());
*/		
		}

	void OnRender(uint64 _delta)
		{
/*			
		WND_Clear();
		monBG.Render();
		buttonBack.Render();
        buttonRestart.Render();	
		buttonDebugDraw.Render();
		labelTitle.Render();	
		monSprite.Render();	
*/		
		}
		
/*		
	void OnTouch(uint32 _button,uint32 _x,uint32 _y)
		{
        if (monSprite.Touched(_x,_y))
            {
            UTI_Log("boum");
            box.ApplyImpulse(0,-6);
            }
		}

	void OnCollide()
		{
		UTI_Log("la classe ça collide");
		Body @ca = PHY_GetContactA();
		Body @cb = PHY_GetContactB();
		if (@ca == @box)
			UTI_Log("box is A");
		if (@cb == @box)
			UTI_Log("box is B");

		}
		
	void OnShutdown()
		{
		UTI_Log("====> physicsScene Shutdown");			
		@buttonBack.on_click_handler = null;
		@buttonBack.user_data = null;	
		GUI_RemoveWidget(buttonBack);		

		@buttonDebugDraw.on_click_handler = null;
		@buttonDebugDraw.user_data = null;	
		GUI_RemoveWidget(buttonDebugDraw);		

        @buttonRestart.on_click_handler = null;
        @buttonRestart.user_data = null;   
        GUI_RemoveWidget(buttonRestart);      

        buttonBox.Destroy();
        box.Destroy();
        ground.Destroy();

		myAtlas.UnLoad();
		myAtlasBG.UnLoad();

		}


	Button buttonBack;	
	Button buttonDebugDraw;	
    Button buttonRestart;    

	Sprite monSprite;
	Atlas 	myAtlas;
	Atlas 	myAtlasBG;
	Sprite monBG;
	Label 	labelTitle;
	Body   box;
	Body   buttonBox;
	Body   ground;

	bool showDebugDraw = false;
*/	
	}

