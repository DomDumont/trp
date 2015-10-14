class IOScene:Scene
    {

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        UTI_Log("====> IO OnClickHandler");
        theSceneManager.ChangeScene(menuScene);
        return true;
        }

    void Init()
        {
        UTI_Log("====> helloWorldScene Init");    

        myAtlas.Load("graphics/sheet"); 

        maFonte.Load("fonts/casual.ttf",50);


        monLabel.SetText("Hello World");
        monLabel.SetPosition(windowX/2,windowY/2);


        buttonBack.SetText("Back");
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);    
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(ioScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        uint64 handle;
        handle  = IO_Open("slot0.txt","w");
        string ts = "Handle =  "+formatInt(handle,"");
        UTI_Log(ts);
        IO_WriteString(handle,"coucou");
        IO_WriteString(handle,"coucou2");
        IO_WriteInt(handle,24);
        IO_Close(handle);

        handle  = IO_Open("slot0.txt","r");
        ts = "Handle =  "+formatInt(handle,"");
        UTI_Log(ts);
        string test;
        int testInt;
        IO_ReadString(handle,test);
        UTI_Log(test);
        IO_ReadString(handle,test);
        UTI_Log(test);
        IO_ReadInt(handle,testInt);
        ts = "testInt =  "+formatInt(testInt,"");
        UTI_Log(ts);

        IO_Close(handle);

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