class Scene
{
void Init()
	{

	}

void OnRender(uint32 _delta)
	{

	}

void OnUpdate(uint32 _delta)
	{

	}
void OnKeyUp(uint32 _scancode)
	{
		
	}
	
void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{

	}	

void OnMultiGesture(int _numFingers, double _x, double _y, double _theta, double _dist )
	{
		
	}

void OnCollide()	
	{

	}
	
void OnShutdown()
	{

	}
}

class SceneManager
{
	void RestartScene()
	{
		if (currentScene !is null) 
			{
			currentScene.OnShutdown();
			currentScene.Init();
			}
	}	

	void ChangeScene(Scene @ _newScene)
	{
		if (currentScene !is null) 
			{
			currentScene.OnShutdown();
			}
	    _newScene.Init();
	    @currentScene = @_newScene;
	}

	void OnRender(uint32 _delta)
	{	
		if (currentScene !is null) 
			{
			currentScene.OnRender(_delta);
			}

	}

	void OnUpdate(uint32 _delta)
	{	
		if (currentScene !is null) 
			{
			currentScene.OnUpdate(_delta);
			}

	}

	void OnShutdown()
	{	
		if (currentScene !is null) 
			{
			currentScene.OnShutdown();
			}

	}

	void OnKeyUp(uint32 _scancode)
	{
	if (currentScene !is null) 
		currentScene.OnKeyUp(_scancode);	
	}

	void OnCollide()
	{	
		if (currentScene !is null) 
			{
			currentScene.OnCollide();
			}

	}

	void OnMultiGesture(int _numFingers, double _x, double _y, double _theta, double _dist )
	{
		if (currentScene !is null) 
			currentScene.OnMultiGesture(_numFingers, _x, _y, _theta, _dist);

	}

	void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{
		if (currentScene !is null) 
			currentScene.OnTouch(_button,_x,_y);
	}
	Scene @currentScene;
}