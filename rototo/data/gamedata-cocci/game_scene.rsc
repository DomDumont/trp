bool OnAnimationFinishedHandler(ref @ _sender,ref @ _userData)
	{

	UTI_Log("Anim Finished");
	return false; //Not fully handle, continue
	};

class GameScene:Scene
{



void Init()
	{


	myAtlasBG.LoadFromImage("coccifond.jpg");
	monBG.Load(myAtlasBG,"coccifond.jpg");
	monBG.SetPosition(windowX/2,windowY/2); // Center of the screen

	myAtlas.Load("cocci");
	myMusic.Load("musicbox.ogg");


	animBroute.Load(myAtlas,"cocci","%s_%05d.png",10);
	animBroute.SetFPS(30);
	cocciPosition.x = windowX/2;
	cocciPosition.y = windowY/2;
	currentAngle = 0;
	animBroute.SetPosition(cocciPosition.x,cocciPosition.y); // Center of the screen
	@animBroute.onComplete = @OnAnimationFinishedHandler;
	//@animBroute.userData = @animBroute;	

	

	sfxWings.Load("wings.ogg");
	
	myMusic.Play();
	
	//animBroute.Scale(1.5f,1.5f);
	
	@currentAnim = @animBroute;
	//currentAnim.Play(E_MODE_PINGPONG,1);
	stateAnim = 0;
	
	cocciSpeed = 0.15f;
	cocciRotationSpeed = 0.15f; // 2 deg par frame
	UTI_SRand(12);
	
	}
	

void OnUpdate(uint64 _delta)
	{
	currentAnim.Update(_delta);
	if (stateAnim == 2)
		{
		//Rotation
		if (currentAngle > targetAngle)
			currentAngle -= cocciRotationSpeed * float(_delta);
		else
			currentAngle += cocciRotationSpeed * float(_delta);
		currentAnim.SetRotation(currentAngle);
		if (abs(currentAngle - targetAngle)<10)
			{
			//UTI_Log("Rotation Finished");	
			stateAnim = 3;
			//currentAnim.Stop();
			}

		}
	if (stateAnim == 3)
		{

		string ts = "cocciPosition : "+formatFloat(cocciPosition.x,"") + " "  +formatFloat(cocciPosition.y,"");
		//UTI_Log(ts);
		cocciPosition  += differenceToCocci * float(_delta) * cocciSpeed;	
		ts = "cocciPosition : "+formatFloat(cocciPosition.x,"") + " "  +formatFloat(cocciPosition.y,"");
		//UTI_Log(ts);		
		currentAnim.SetPosition(cocciPosition.x,cocciPosition.y);
		float dist = (cocciTarget - cocciPosition).Length();
		if (dist < 5.0f)
			{
			//UTI_Log("Deplacement Finished");	
			stateAnim = 0;
			currentAnim.Stop();	
			currentAnim.SetScale(1.0f,1.0f);
			}
		}		
	}	

void OnRender(uint64 _delta)
	{
	monBG.Render();

	currentAnim.Render();
	}
	
void OnTouch(uint32 _button,uint32 _x,uint32 _y)
	{
	Vector2D tempTarget;
	if (currentAnim.Touched(_x,_y))
		{
		tempTarget.x = UTI_Rand(0,1024);
		tempTarget.y = UTI_Rand(0,768);
		currentAnim.SetScale(0.50f,0.50f);
		}
	else
		{
		tempTarget.x = _x;
		tempTarget.y = _y;
		
		}


		{
		sfxWings.Play();	
		//First Rotate
		Vector2D direction = tempTarget - cocciPosition ;
		targetAngle = direction.ToAngle();
		//targetAngle = - (atan2(cocciPosition.x-tempTarget.x,cocciPosition.y-tempTarget.y)*180/3.1415);
		currentAnim.Play(MODE_PINGPONG,-1);
		//currentAnim.SetRotation(-angle);	
		stateAnim = 2;

		//Compute the target for later
		cocciTarget.x = tempTarget.x;
		cocciTarget.y = tempTarget.y;
		differenceToCocci = cocciTarget - cocciPosition;

		string ts = "differenceToCocci : "+formatFloat(differenceToCocci.x,"") + " "  +formatFloat(differenceToCocci.y,"");
		//UTI_Log(ts);

		differenceToCocci.Normalize();

		ts = "differenceToCocci : "+formatFloat(differenceToCocci.x,"") + " "  +formatFloat(differenceToCocci.y,"");
		//UTI_Log(ts);

		}


	}

void OnShutdown()
	{
	//UTI_Log("SHUTDOWN GAME SCENE\n");

	@animBroute.userData = null;

	animBroute.UnLoad();


	myMusic.UnLoad();
	
		
	myAtlas.UnLoad();
	
	}
	

	// VARIABLES
	Atlas 	myAtlasBG;
	Sprite monBG;

	Atlas 	myAtlas;

	Animation animBroute;
	
	
	Vector2D cocciPosition;
	Vector2D cocciTarget;
	Vector2D differenceToCocci;
	float cocciSpeed;
	float cocciRotationSpeed;

	int currentAngle;
	int targetAngle;



	Animation @currentAnim;
	int stateAnim;

	Music  myMusic;
	Sound	sfxWings;
	
};