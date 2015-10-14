
int                 windowX;
int                 windowY;


Label  				monLabel;

Button              myButton;



bool OnClickHandler(ref @ _sender,ref @ _userData)
    {
    UTI_Log("====> OnClickHandler");
    return true;
    }

void OnInit()
{

    WND_SetWindowTitle("AmILate");

    WND_GetLogicalSize(windowX,windowY);
    string ts = "Logical Size =  "+formatInt(windowX,"")+" , "+formatInt(windowY,"");
    UTI_Log(ts);

    GUI_LoadTheme("aeon");
 
    //monLabel.SetText(UTI_GetLanguage());
    monLabel.SetText("Français");
    monLabel.SetPosition(windowX/2,windowY/2);
   
    myButton.SetText("Coucou le gros bouton");
    myButton.SetSize(300,200);
    myButton.SetPosition(windowX/2,windowY/2);    
    myButton.SetClickHandler(OnClickHandler);
    myButton.SetEnabled(true);
    //myButton.SetScale(1,1);

    GUI_AddWidget(myButton);

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void OnUpdate(uint64 _delta)
{

}

void OnKeyUp(uint32 _scancode)
{
string ts = "OnKeyUp "+formatInt(_scancode,"");
UTI_Log(ts);
}

void OnRender(uint64 _delta)
{
    WND_Clear();
    myButton.Render();
}

void OnTouch(uint32 _button,uint32 _x,uint32 _y)
{
}


void OnCollide()
{
}

void OnShutdown()
{
   GUI_UnLoadTheme();
}