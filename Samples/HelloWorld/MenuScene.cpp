#include "MenuScene.h"


bool OnSelectionChangedHandler(void * _sender,void * _userData)
{
	UTI_Log("list changed");

	ListBox *tempLB = (ListBox*)(_sender);
	MenuScene *tempScene = (MenuScene*)(_userData);


	int sel = tempLB->GetSelectedIndex();
	if (sel == -1)
		tempScene->labelChosen.SetText("None selected");
	else
		tempScene->labelChosen.SetText(tempLB->GetItemText(sel));

	tempScene->buttonStart.SetEnabled(true);

	return false; //important
}

bool OnClickHandler(void * _sender,void * _userData)
	{
		UTI_Log("click on start");

	Button *tempButton = (Button*)(_sender);
	MenuScene *tempScene = (MenuScene*)(_userData);
	tempButton->SetScale(1.5,1.5);
	int sel = tempScene->listBox.GetSelectedIndex();
	

	//theSceneManager.ChangeScene();
	UTI_Log("====> Fin du OnClickHandler");
	return true; //Very important !!!		

	}

void MenuScene::Init()
{
	WND_GetLogicalSize(windowX, windowY);
  	helpLabel.SetText("Ctrl-R to restart scripts\nCtrl-F to explore scripts\nCtrl-H for Help",true);
    helpLabel.SetPosition(windowX * 3/4,600);  

	monLabel.SetText("tempText");
	monLabel.SetPosition(260,100);
	
	

	labelChosen.SetText("None Selected");
	labelChosen.SetPosition(750,250);
	
	


	
	buttonStart.SetText("START");
	
	buttonStart.SetSize(300,100);
	buttonStart.SetPosition(750,windowY/2);	
	
	
	buttonStart.SetEnabled(false);
	buttonStart.SetScale(1.0,1.0);
	
	buttonStart.SetClickHandler(OnClickHandler);
	buttonStart.SetUserData(this);
	buttonStart.SetSender(&buttonStart);


	GUI_AddWidget(&buttonStart);

	// Setup Listbox

	listBox.SetSize(400,400);
	listBox.SetPosition(200,380);
	
	listBox.SetSelectionClickHandler(OnSelectionChangedHandler);
	listBox.SetUserData(this);
	listBox.SetSender(&listBox);
	
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
	GUI_AddWidget(&listBox);
	

	// Setup Combobox

	comboBox.SetSize(400,200);
	comboBox.SetPosition(800,150);

	
    comboBox.AddItem("English");
    comboBox.AddItem("French");
    comboBox.AddItem("Italian");
    comboBox.AddItem("Portuguese");
    comboBox.AddItem("Lithuania");
	comboBox.SetSelectedIndex(1);

 	//@comboBox.onSelectionChangedHandler = CallbackHandler(menuScene.OnSelectionChangedHandlerCB);
	//@comboBox.userData = @this;       

	comboBox.SetEnabled(true);
	GUI_AddWidget(&comboBox);

	myMusic.Load("sounds/sample.ogg");  
    myMusic.Play(-1);   
}

void MenuScene::OnRender(unsigned int _delta)
{
	WND_Clear();
	monLabel.Render();
    helpLabel.Render();
	labelChosen.Render();
	buttonStart.Render();
	listBox.Render();
	comboBox.Render();
	
}

void MenuScene::OnUpdate(unsigned int _delta)
{

}
