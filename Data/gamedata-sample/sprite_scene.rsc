class SpriteScene:Scene
    {
    Sprite mySprite;
    Atlas  myAtlas;
    Button buttonBack;
    float currentScale;

    bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }
        
    void Init()
        {
        myAtlas.Load("graphics/sheet");         
        mySprite.Load(myAtlas,"sheep.png");
        mySprite.SetPosition(windowX/2,windowY/2);


        
        buttonBack.SetText("Back");        
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);            
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(spriteScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);

        currentScale = 1;
        }
        
    void OnUpdate(uint32 _delta)
        {   
        
        }

    void OnMultiGesture(int _numFingers, double _x, double _y, double _theta, double _dist )
        {
      
            mySprite.Rotation = mySprite.Rotation + _theta;
            mySprite.SetPosition( _x * windowX, _y * windowY);
            currentScale += (_dist * 10);
            mySprite.SetScale(currentScale,currentScale);
        }


    void OnRender(uint32 _delta)
        {
        WND_Clear();
        mySprite.Render(); 
        buttonBack.Render();
        }

    void OnShutdown()
        {
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);              
        myAtlas.UnLoad();
        }
        
    }