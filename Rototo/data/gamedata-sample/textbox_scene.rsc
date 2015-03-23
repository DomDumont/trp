class CTextboxScene:CScene
	{

	bool OnClickHandler(ref @ _sender,ref @ _userData)
		{
		UTI_Log("cpicpi");
		CButton @tempButton = cast<CButton>(_sender);
		tempButton.SetScale(2.0,2.0);
		theSceneManager.ChangeScene(menuScene);
		return true;
		}

	void Init()
		{

		

		myAtlas.Load("sheet");	

		

		
		myAtlasBG.LoadFromImage("apple-bg.png");
		monBG.Load(myAtlasBG,"apple-bg.png");
		monBG.SetCenterPosition(windowX/2,windowY/2); // Center of the screen


		buttonBack.SetFont(fonte35);
		buttonBack.SetText("Back");
		buttonBack.SetSprite(myAtlas,"button.png");
		buttonBack.SetSize(250,100);
		buttonBack.SetCenterPosition(800,650);	
		buttonBack.SetTextColor(255,150,150);		
		buttonBack.set_Rotation(5);
		buttonBack.SetEnabled(true);
		@buttonBack.onClickHandler = CallbackHandler(textboxScene.OnClickHandler);
		@buttonBack.userData = @this;
		buttonBack.SetScale(1.0,1.0);

		GUI_AddWidget(buttonBack);
		//@buttonBack.onClickHandler = @OnClickHandler;
		//@buttonBack.userData = @this;

		maTextBox.SetSize(300,50);
		maTextBox.SetCenterPosition(windowX/2,200);
		maTextBox.SetFont(fonte8);
		maTextBox.SetText("Enter Your Name");
		maTextBox.SetTextColor(255,255,255,255);
		GUI_AddWidget(maTextBox);
		}
		
	void OnUpdate(uint64 _delta)
		{	
		}

	void OnRender(uint64 _delta)
		{
		monBG.Render();
		maTextBox.Render();
		buttonBack.Render();			
		}
		
	void OnTouch(uint32 _button,uint32 _x,uint32 _y)
		{
		}
		
	void OnShutdown()
		{
		@buttonBack.onClickHandler = null;
		@buttonBack.userData = null;	
		GUI_RemoveWidget(buttonBack);		
		}


	CButton buttonBack;		

	CAtlas 	myAtlas;
	CAtlas 	myAtlasBG;
	CSprite monBG;
	CTextBox maTextBox;

	}

