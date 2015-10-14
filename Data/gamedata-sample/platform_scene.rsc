class PlatformScene:Scene
    {
        TiledMap map;

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }

    void Init()
        {
        UTI_Log("====> helloWorldScene Init");    

        myAtlas.Load("graphics/sheet"); 

        maFonte.Load("fonts/casual.ttf",50);

        
        monLabel.SetText("Platform Sample");
        monLabel.SetPosition(windowX/2,windowY/2);

        
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(platformScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        map.Load("platform.tmx");
        }
        
    void OnUpdate(uint32 _delta)
        {   
        
        }

    void OnRender(uint32 _delta)
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