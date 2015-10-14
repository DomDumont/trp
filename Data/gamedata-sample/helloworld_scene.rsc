class HelloWorldScene:Scene
    {

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }


    void Init()
        {
						int rititi = 2;

        UTI_Log("====> helloWorldScene Init");    

						rititi += 10;
        myAtlas.Load("graphics/sheet"); 

        maFonte.Load("fonts/casual.ttf",50);


        monLabel.SetText("Hello World");
        monLabel.SetPosition(windowX/2,windowY/2);


  
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(helloworldScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        }
        
    void OnUpdate(uint64 _delta)
        {   
        
        }

    void OnRender(uint64 _delta)
        {
        WND_Clear();
        monLabel.Render();
        buttonBack.Render();
        }

    void OnShutdown()
        {
        UTI_Log("====> helloWorldScene Shutdown");    
        maFonte.UnLoad();
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();
        }
        
    Font   maFonte;
    Label  monLabel;
    Button buttonBack; 
    Atlas  myAtlas;
    }