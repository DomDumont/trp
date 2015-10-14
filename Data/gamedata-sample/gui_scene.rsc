class GUIScene:Scene
    {

    Label  monLabel;

    Button buttonBack; 
    Button checkboxTest;
    Button radioboxTest;

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
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(guiScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        checkboxTest.SetType(TYPE_CHECKBOX);
        checkboxTest.SetText("Test Checkbox");        
        checkboxTest.SetSize(350,100);
        checkboxTest.SetPosition(200,200);        
        checkboxTest.SetEnabled(true);
        checkboxTest.SetScale(1.0,1.0);

        GUI_AddWidget(checkboxTest);

        radioboxTest.SetType(TYPE_RADIOBOX);
        radioboxTest.SetText("Test Radiobox");        
        radioboxTest.SetSize(350,100);
        radioboxTest.SetPosition(200,300);        
        radioboxTest.SetEnabled(true);
        radioboxTest.SetScale(1.0,1.0);

        GUI_AddWidget(radioboxTest);


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
        radioboxTest.Render();
        }

    void OnShutdown()
        {
        UTI_Log("====> GUIScene Shutdown");    
        
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);
        GUI_RemoveWidget(buttonBack);   
        GUI_RemoveWidget(checkboxTest);   
        GUI_RemoveWidget(radioboxTest);           
        myAtlas.UnLoad();
        }
        
    
    }