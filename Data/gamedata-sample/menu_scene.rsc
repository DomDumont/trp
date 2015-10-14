
int thisIsBad = 1;

class MenuScene:Scene
{
bool OnSelectionChangedHandlerCB(ref @ _sender,ref @ _userData)
	{
	UTI_Log("comboBox Selection Changed");
	ComboBox @tempCB = cast<ComboBox>(_sender);

	int sel = tempCB.GetSelectedIndex();
	thisIsBad = sel;
	if (sel == 0)
		{
		TXT_Load("strings/en");
		}
	else
	if (sel == 1)
		{
		TXT_Load("strings/fr");
		}
	theSceneManager.RestartScene();
	return true; //We change scene so it's fully handled
	}

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
	UTI_Log("listbox Selection Changed");
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

	

	
	

    
	


    helpLabel.SetText("Ctrl-R to restart scripts\nCtrl-F to explore scripts\nCtrl-H for Help",true);
    helpLabel.SetPosition(windowX * 3/4,600);    

    fpsLabel.SetText("FPS : ",true);
    fpsLabel.SetPosition(100,740);    

    string tempText;

    tempText = TXT_GetString("STR_TITLE");

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
	monLabel.SetPosition(260,100);
	monLabel.Rotation = -5;
	
	//labelChosen.SetFont(fonte8);

	labelChosen.SetText("None Selected");
	labelChosen.SetPosition(750,250);
	
	


	//buttonStart.SetFont(fonte35);
	buttonStart.SetText(TXT_GetString("STR_START"));
	//buttonStart.SetSprite(myAtlas,"button9.png",true);
	buttonStart.SetSize(300,100);
	buttonStart.SetPosition(750,windowY/2);	
	//buttonStart.SetTextColor(255,150,150);		
	buttonStart.set_Rotation(5);
	buttonStart.SetEnabled(false);
	buttonStart.SetScale(1.0,1.0);
	//buttonStart.SetClickHandler(CallbackHandler(menuScene.OnClickHandler));

	//buttonStart.SetUserData(@this);
	GUI_AddWidget(buttonStart);

	// Setup Listbox

	listBox.SetSize(400,400);
	listBox.SetPosition(200,380);
	
	//listBox.SetSelectionChangedHandler(CallbackHandler(menuScene.OnSelectionChangedHandler));
	//listBox.SetUserData(@this);
	
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
	

	// Setup Combobox

	comboBox.SetSize(400,200);
	comboBox.SetPosition(800,150);

	
    comboBox.AddItem("English");
    comboBox.AddItem("French");
    comboBox.AddItem("Italian");
    comboBox.AddItem("Portuguese");
    comboBox.AddItem("Lithuania");
	comboBox.SetSelectedIndex(thisIsBad);

 	//comboBox.SetSelectionChangedHandler(CallbackHandler(menuScene.OnSelectionChangedHandlerCB));
	//comboBox.SetUserData(@this);       

	comboBox.SetEnabled(true);
	GUI_AddWidget(comboBox);

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
	comboBox.Render();

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
	//listBox.SetSelectionChangedHandler(null);
	//listBox.SetUserData(null);
	GUI_RemoveWidget(listBox);

	comboBox.ResetContent();
	//comboBox.SetSelectionChangedHandler(null);
	//comboBox.SetUserData(null);
	GUI_RemoveWidget(comboBox);

	//buttonStart.SetClickHandler(null);
	//buttonStart.SetUserData(null);
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
	ComboBox comboBox;
}


