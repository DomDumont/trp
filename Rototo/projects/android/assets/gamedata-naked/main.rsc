
int                 windowX;
int                 windowY;

Font                fonte8;
Label  				monLabel;







void OnInit()
{

    WND_SetWindowTitle("AmILate");

    WND_GetLogicalSize(windowX,windowY);
    string ts = "Logical Size =  "+formatInt(windowX,"")+" , "+formatInt(windowY,"");
    UTI_Log(ts);

    
    fonte8.Load("fonts/casual.ttf",25);


    monLabel.SetFont(fonte8);
    monLabel.SetText("Hello World");
    monLabel.SetPosition(windowX/2,windowY/2);
    monLabel.SetColor(255,255,255,255);


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
    WND_ClearWithColor(255,80,80,255);
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
    fonte8.UnLoad();
}