class HelloWorldScene:Scene
    {

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        //theSceneManager.RestartScene();            
        theSceneManager.ChangeScene(menuScene);
        return true;
        }


    void Init()
        {
						int rititi = 2;

        UTI_Log("====> helloWorldScene Init");    

						rititi += 10;
        myAtlas.Load("graphics/sheet"); 




        monLabel.SetText("Hello World");
        monLabel.SetPosition(windowX/2,windowY/2);


  
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(helloworldScene.OnClickHandler));
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

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

        //buttonBack.SetClickHandler(null);
        //buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();
        }
        

    Label  monLabel;
    Button buttonBack; 
    Atlas  myAtlas;
    }