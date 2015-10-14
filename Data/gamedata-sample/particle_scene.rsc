

class ParticleScene:Scene
    {
    Font   maFonte;
    Label  monLabel;
    Button buttonBack; 
    Atlas  myAtlas;
		 Emitter myEmitter;

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }


    void Init()
        {
						int rititi = 2;

        UTI_Log("====> ParticleScene Init");    

						rititi += 10;
        myAtlas.Load("graphics/sheet"); 

        maFonte.Load("fonts/casual.ttf",50);


        monLabel.SetText("Particles");
        monLabel.SetPosition(windowX/2,windowY/2);


  
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);        
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(particleScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        myEmitter.Load(myAtlas,"particles/snow"); 
        myEmitter.SetPosition(100,100);
        }
        
    void OnUpdate(uint32 _delta)
        {   
        myEmitter.Update(_delta);
        }

    void OnRender(uint32 _delta)
        {
        WND_Clear();
        monLabel.Render();
        buttonBack.Render();
		myEmitter.Render();
        }

    void OnShutdown()
        {
        UTI_Log("====> ParticleScene Shutdown");    
        maFonte.UnLoad();
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);   
        myAtlas.UnLoad();
        }
        
    }
