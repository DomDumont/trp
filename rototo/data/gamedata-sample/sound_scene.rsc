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
        @buttonBack.on_click_handler = CallbackHandler(soundScene.OnClickHandler);
        @buttonBack.user_data = @this;
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);
        }
        
    void OnUpdate(uint64 _delta)
        {   
        
        }

    void OnRender(uint64 _delta)
        {
        WND_Clear();
        buttonBack.Render();
        }

    void OnShutdown()
        {
        @buttonBack.on_click_handler = null;
        @buttonBack.user_data = null;    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();            
        }
        
    Sound  mySFX;

    Button buttonBack; 
    Atlas  myAtlas;    
    }