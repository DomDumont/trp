#include "MenuScene.h"
#include "GuiManager.h"
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
	
	//@buttonStart.on_click_handler = CallbackHandler(menuScene.OnClickHandler);
	//@buttonStart.user_data = @this;

	GUI_AddWidget(&buttonStart);

	// Setup Listbox

	listBox.SetSize(400,400);
	listBox.SetPosition(200,380);
	
	//@listBox.onSelectionChangedHandler = CallbackHandler(menuScene.OnSelectionChangedHandler);
	//@listBox.userData = @this;
	
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

	
}

void MenuScene::OnRender(Uint64 _delta)
{
	WND_Clear();
	monLabel.Render();
    helpLabel.Render();
	labelChosen.Render();
	buttonStart.Render();
	listBox.Render();
	comboBox.Render();
	
}

void MenuScene::OnUpdate(Uint64 _delta)
{

}
