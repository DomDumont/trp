
int                 windowX;
int                 windowY;


Label  				monLabel;







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
    monLabel.Render();
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