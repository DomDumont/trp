class AnimationScene:Scene
    {
    Animation myAnim;
    Atlas  myAtlas;
    Button buttonBack;
    Atlas  myAtlasUI;

   bool OnClickHandler(ref @ _sender,ref @ _userData)
        {
        theSceneManager.ChangeScene(menuScene);
        return true;
        }

    void Init()
        {
        myAtlas.Load("graphics/broute"); 
        myAtlasUI.Load("graphics/sheet");                 
        myAnim.Load(myAtlas,"broute","%s_%05d.tga",30);
        myAnim.SetFPS(12);
        myAnim.Play(MODE_NORMAL,INFINITE_LOOP);


        buttonBack.SetText("Back");
        buttonBack.SetSize(250,100);
        buttonBack.SetPosition(800,650);    
        buttonBack.set_Rotation(5);
        buttonBack.SetEnabled(true);
        buttonBack.SetClickHandler(CallbackHandler(animationScene.OnClickHandler));
        buttonBack.SetUserData(@this);
        buttonBack.SetScale(1.0,1.0);
        GUI_AddWidget(buttonBack);        
        }
        
    void OnUpdate(uint32 _delta)
        {   
        myAnim.Update(_delta);
        }

    void OnRender(uint32 _delta)
        {
        WND_Clear();
        myAnim.Render();
        buttonBack.Render();
        }

    void OnShutdown()
        {
        buttonBack.SetClickHandler(null);
        buttonBack.SetUserData(null);    
        GUI_RemoveWidget(buttonBack);      

        myAtlas.UnLoad();
        myAtlasUI.UnLoad();
        myAnim.UnLoad();
        }
        
    }