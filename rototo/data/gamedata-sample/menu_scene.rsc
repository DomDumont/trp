


class MenuScene:Scene
{

bool OnSelectionChangedHandler(ref @ _sender,ref @ _userData)
	{
	ListBox @tempLB = cast<ListBox>(_sender);
	MenuScene @tempScene = cast<MenuScene>(_userData);


		int sel = tempLB.GetSelectedIndex();
		if (sel == -1)
			tempScene.labelChosen.SetText("None selected");
		else
			tempScene.labelChosen.SetText(tempLB.GetItemText(sel));

	tempScene.buttonStart.SetEnabled(true);
	UTI_Log("rototo Selection Changed");
	return false; //Not fully handle, continue
	};

bool OnClickHandler(ref @ _sender,ref @ _userData)
	{
	UTI_Log("====> Debut du OnClickHandler");
	Button @tempButton = cast<Button>(_sender);
	MenuScene @tempScene = cast<MenuScene>(_userData);
	tempButton.SetScale(1.5,1.5);
	int sel = tempScene.listBox.GetSelectedIndex();
	string selText = tempScene.listBox.GetItemText(sel);

	theSceneManager.ChangeScene(allScenes[sel]);
	UTI_Log("====> Fin du OnClickHandler");
	return true; //Very important !!!
	}

void Init()
	{

	UTI_Log("====> menuScene Init");

	myAtlas.Load("graphics/sheet");	

	

	
	

    
	


    helpLabel.SetText("Press F12 to see scripts\nPress F1 for Help",true);
    helpLabel.SetPosition(windowX/2,700);    

    fpsLabel.SetText("FPS : ",true);
    fpsLabel.SetPosition(100,740);    

    string tempText;
    tempText = "T.R.P. Samples";
	//monLabel.SetFont(fonte35);
#if OSX
	tempText += " OSX Version ";
#endif
#if WIN32
	tempText += " Win Version ";
#endif
#if IOS
	tempText += " IOS Version ";
#endif

#if ANDROID
    tempText += " Android Version ";
#endif    
	tempText += UTI_GetVersion();
	monLabel.SetText(tempText);
	monLabel.SetShaded(false);
	monLabel.SetPosition(260,100);
	monLabel.SetRotation(-5);
	
	//labelChosen.SetFont(fonte8);

	labelChosen.SetText("None Selected");
	labelChosen.SetPosition(750,250);
	
	


	//buttonStart.SetFont(fonte35);
	buttonStart.SetText("Start");
	//buttonStart.SetSprite(myAtlas,"button9.png",true);
	buttonStart.SetSize(300,100);
	buttonStart.SetPosition(750,windowY/2);	
	//buttonStart.SetTextColor(255,150,150);		
	buttonStart.SetRotation(5);
	buttonStart.SetEnabled(false);
	buttonStart.SetScale(1.0,1.0);
	@buttonStart.on_click_handler = CallbackHandler(menuScene.OnClickHandler);

	@buttonStart.user_data = @this;
	GUI_AddWidget(buttonStart);

	// Setup Listbox

	listBox.SetSize(400,400);
	listBox.SetPosition(200,380);
	//listBox.SetFont(fonteMed);
	@listBox.onSelectionChangedHandler = CallbackHandler(menuScene.OnSelectionChangedHandler);
	@listBox.userData = @this;
	
    listBox.AddItem("Particles");
    listBox.AddItem("Hello World");
    listBox.AddItem("GUI");    
    listBox.AddItem("Physics");
    listBox.AddItem("Sprites");
    listBox.AddItem("Animation");
    listBox.AddItem("Sound");
    listBox.AddItem("Music");
    listBox.AddItem("Mini Game");
    listBox.AddItem("Tween");
    listBox.AddItem("IO");
    listBox.AddItem("Primitives");
    listBox.AddItem("Platform");
	listBox.AddItem("Dialog");    
        

	listBox.SetEnabled(true);
	GUI_AddWidget(listBox);
	

    accu = 0;

	}
	

void OnUpdate(uint64 _delta)
	{
	
	}	
void OnRender(uint64 _delta)
	{
	WND_Clear();
	monLabel.Render();
    helpLabel.Render();
	labelChosen.Render();
	buttonStart.Render();
	listBox.Render();

    accu += _delta;
    if (accu > 1000)
    {
    accu = 0;
    float fps = floor(1000.0f/_delta);
    string ts = "FPS : "+formatInt(fps,"");
    fpsLabel.SetText(ts,false);
    }

    fpsLabel.Render();
	}
	
void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{

	}
	


	void OnShutdown()
	{
	UTI_Log("====> menuScene Shutdown");	
	listBox.ResetContent();
	@listBox.onSelectionChangedHandler = null;
	@listBox.userData = null;

	GUI_RemoveWidget(listBox);
	@buttonStart.on_click_handler = null;
	@buttonStart.user_data = null;
	GUI_RemoveWidget(buttonStart);


	


	
	
	
	myAtlas.UnLoad();	
	
	//monLabel.UnLoad();


	TWN_CancelTweens();
	}

	Atlas 	myAtlas;
	
	
	

	Label  monLabel;
	Label  labelChosen;
    Label  helpLabel;
    Label  fpsLabel;
    float  accu;

	

	Button buttonStart;

	ListBox listBox;
}


