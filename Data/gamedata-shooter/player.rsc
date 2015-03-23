const int cooldownFrames = 500;

class Player : Entity
{

	int cooldownRemaining = 0;

	Player()
	{
	
	}

	void Init() override
		{
		UTI_Log("Player Init");
		mysprite.Load(gameScene.atlas,"Player.png");
		Orientation = 0;
		Position.x = screenSizeX/2;
		Position.y = screenSizeY/2;
		};


	void OnUpdate(uint64 _delta) override 
		{

		Orientation = gameScene.targetAngle;
		mysprite.SetPosition(Position.x,Position.y);
		mysprite.Rotation = Orientation;

		if (cooldownRemaining <= 0)
			{

		    	cooldownRemaining = cooldownFrames;	
		    	
		    	Vector2D vel;
		    	vel.FromPolar(gameScene.targetAngle, 5.0f);	    	
		    	//UTI_Log("Add a bullet");

		    	Vector2D offset = vel;
		    	offset.TurnLeft();
		    	offset.Normalize();
		    	offset *=10; 
		    	Bullet newBullet(Position+offset,vel);
		    	newBullet.Init();	    	
		    	gameScene.em.Add(newBullet);
	
				offset = vel;
				offset.TurnRight();
		    	offset.Normalize();
		    	offset *=10; 
		    	Bullet newBullet2(Position+offset,vel);
		    	newBullet2.Init();	    	
		    	gameScene.em.Add(newBullet2);	    	

		    	gameScene.shot.Play();	

		    	}
		else
		    	cooldownRemaining -= _delta;
		}
};
