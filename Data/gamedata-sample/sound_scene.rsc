class SoundScene:Scene
    {

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }

    void Init()
        {
        mySFX.Load("sounds/sheep.ogg");  
        mySFX.Play();     

        myAtlas.Load("graphics/sheet");
        buttonBack.SetText("Back");
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);    
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(soundScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);
        }
        
    void OnUpdate(uint32 _delta)
        {   
        
        }

    void OnRender(uint32 _delta)
        {
        WND_Clear();
        buttonBack.Render();
        }

    void OnShutdown()
        {
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();            
        }
        
    Sound  mySFX;

    Button buttonBack; 
    Atlas  myAtlas;    
    }