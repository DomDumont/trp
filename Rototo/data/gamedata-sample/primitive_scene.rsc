class PrimitiveScene:Scene
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

        monLabel.SetText("Primitive");
        monLabel.SetPosition(windowX/2,windowY/2);


        buttonBack.SetText("Back");
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);    
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        @buttonBack.on_click_handler = CallbackHandler(primitiveScene.OnClickHandler);
        @buttonBack.user_data = @this;
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        myPrimitive.SetColor(255,250,250,50);       
        myPrimitive.SetSize(500,500);
        myPrimitive.SetPosition(500,350);    
        //myPrimitive.set_Rotation(5);
        myPrimitive.SetShape(1);        

        }
        
    void OnUpdate(uint64 _delta)
        {   
        
        }

    void OnRender(uint64 _delta)
        {
        WND_Clear();
        //monLabel.Render();
        //buttonBack.Render();
        myPrimitive.Render();
        }

    void OnShutdown()
        {
        UTI_Log("====> helloWorldScene Shutdown");    
        maFonte.UnLoad();
        @buttonBack.on_click_handler = null;
        @buttonBack.user_data = null;    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();
        }
        
    Font   maFonte;
    Label  monLabel;
    Button buttonBack; 
    Atlas  myAtlas;
    Primitive myPrimitive;
    }