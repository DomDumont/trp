


## Your first T.R.P. project (aka HelloWorld version 1)

T.R.P. needs at least one file called main.rsc in the gamedata directory

Take your favorite text editor and create a file called main.rsc. Paste the code below. This is the minimal TRP program.



* OnInit
* OnUpdate
* OnRender
* OnShutdown

~~~~ { .Cpp }

	void OnInit()
	{
	UTI_Log("Hello World");
	}

	void OnUpdate(uint64 _delta)
	{
	UTI_Exit();
	}

	void OnRender(uint64 _delta)
	{

	}

	void OnShutdown()
	{
	}

~~~~

You must provide at least those 4 functions , they will be automatically called by T.R.P. during the execution of your game.

* OnInit is called only once at the beginning of the game (or each time you restart the game )
* OnUpdate is called every engine frame before OnRender
* OnRender is called every engine frame
* OnShutdown is called only once at the end of the game (when the window close or if you call UTI_Exit)


## Enhanced HelloWorld (Version 2)

Let's do the same HelloWorld program but this time by using the scene manager provided in the samples
and with a Label (to have something more graphical than the system logs)

In the gamedata directory you will find a minimal scene manager.
So let's open the file scene_manager.rsc to see what's inside ;-)

We have 2 classes : 

* a SceneManager class
* a Scene class

The SceneManager class includes a "ChangeScene" method
void ChangeScene(Scene @ _newScene) wich is more interesting than the others.

The global idea is to have multiple scenes, 1 for the game, 1 for menus, ... and to navigate from one to another by calling the ChangeScene method.


Let's do this 2 steps process :

1. Write a game scene wich inherits from  Scene
1. Write a new "main.rsc" file  to launch directly our new scene (e.g. by calling a changeScene with it)

### Create our game scene

Create a text file called "game_scene.rsc"
paste the following code into it: 

~~~~ { .Cpp }

class GameScene:Scene
	{


	void Init()
		{
		maFonte.Load("casual.ttf",50);

		monLabel.SetFont(maFonte);
		monLabel.SetText("Hello World");

		}
		
	void OnUpdate(uint64 _delta)
		{	
		
		}

	void OnRender(uint64 _delta)
		{
		monLabel.Render();
		}

	CFont   maFonte;
	CLabel 	monLabel;
	}
~~~~~~~~

[Font](#class-font) 
[Label](#class-label) 

Of course, you noticed that the scene manager provides is minimalist, nothing prevents you to create your own with more features 
(of pushScenes / Popscene example)


### Create the new main.rsc file


create a text file called main.rsc. Paste the code below. 



