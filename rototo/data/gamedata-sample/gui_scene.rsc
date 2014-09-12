class GUIScene:Scene
    {

    Label  monLabel;
    Button buttonBack; 
    Button checkboxTest;
    Atlas  myAtlas;

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }

    void Init()
        {
        UTI_Log("====> GUIScene Init");    

        myAtlas.Load("graphics/sheet"); 

        

        
        monLabel.SetText("GUI");
        monLabel.SetPosition(windowX/2,windowY/2);

        
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.SetRotation(5);
        buttonBack.SetEnabled(true);
        @buttonBack.on_click_handler = CallbackHandler(guiScene.OnClickHandler);
        @buttonBack.user_data = @this;
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        checkboxTest.SetType(TYPE_CHECKBOX);
        checkboxTest.SetText("Test Checkbox");        
        checkboxTest.SetSize(350,100);
        checkboxTest.SetPosition(200,200);        
        checkboxTest.SetEnabled(true);
        checkboxTest.SetScale(1.0,1.0);

        GUI_AddWidget(checkboxTest);

        }
        
    void OnUpdate(uint64 _delta)
        {   
        
        }

    void OnRender(uint64 _delta)
        {
        WND_Clear();
        monLabel.Render();
        buttonBack.Render();
        checkboxTest.Render();
        }

    void OnShutdown()
        {
        UTI_Log("====> GUIScene Shutdown");    
        
        @buttonBack.on_click_handler = null;
        @buttonBack.user_data = null;    
        GUI_RemoveWidget(buttonBack);   
        GUI_RemoveWidget(checkboxTest);   
        myAtlas.UnLoad();
        }
        
    
    }