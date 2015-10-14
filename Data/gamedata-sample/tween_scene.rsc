bool MyCompleteHandler(ref @ _sender,ref @ _usedData)
	{
	
 	TweenScene @tempScene = cast<TweenScene>(_usedData);
 	tempScene.mySprite.SetScale(0.5f,0.5f);		
	UTI_Log("Done");
    return true;
	};		


class TweenScene:Scene
{

bool OnClickHandler(ref @ _sender,ref @ _userData)
    {
    theSceneManager.ChangeScene(menuScene);
    return true;
    }


void Init()
	{
		
	myAtlas.Load("graphics/sheet");

    buttonBack.SetText("Back");
    buttonBack.SetSize(250,100);
    buttonBack.SetPosition(800,650);    
    buttonBack.set_Rotation(5);
    buttonBack.SetEnabled(true);
    @buttonBack.SetClickHandler(CallbackHandler(helloworldScene.OnClickHandler));
    buttonBack.SetUserData(@this);
    buttonBack.SetScale(1.0,1.0);
    GUI_AddWidget(buttonBack);


	mySprite.Load(myAtlas,"sheep.png");
	mySprite.SetPosition(windowX/2,windowY/2); // Center of the screen

	

	
	
	
	Tween myTween;
	myTween.Init(2000,BOUNCE_EFFECT,EASE_OUT);
	myTween.SetOnCompleteHandler(@MyCompleteHandler);
	myTween.SetUserData(@this);
	monFloat.initialValue = 100;
	monFloat.targetValue = 650;
	myTween.AddProp(monFloat);
	TWN_AddTween(myTween);

	}
	

void OnUpdate(uint64 _delta)
	{
	mySprite.SetPosition(windowX/2,int (monFloat.value));
	}

void OnRender(uint64 _delta)
	{
    WND_Clear();
	mySprite.Render();
    buttonBack.Render();
	}
	
void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{
	
	}
	


	void OnShutdown()
	{
    buttonBack.SetClickHandler(null);
    buttonBack.SetUserData(null);
    GUI_RemoveWidget(buttonBack);  

	mySprite.UnLoad();	
	myAtlas.UnLoad();	

	TWN_CancelTweens();
	}

	Atlas 	myAtlas;
	Sprite mySprite;
    TweenedFloat monFloat;
    Button buttonBack; 

}


