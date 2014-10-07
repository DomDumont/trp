class Bullet : Entity
{



	Bullet(Vector2D _position, Vector2D _velocity)
	{
	Position = _position;
	Velocity = _velocity;

	mysprite.Position = Position;
	mysprite.Rotation = Orientation;

	Orientation = Velocity.ToAngle();
	}


	void Init() override
		{
		//UTI_Log("Bullet Init");
		mysprite.Load(gameScene.atlas,"Bullet.png");
		mysprite.Position = Position;
		mysprite.Rotation = Orientation;
		//Orientation = 0;
		//Position.x = screenSizeX/2;
		//Position.y = screenSizeY/2;
		};


	void OnUpdate(uint64 _delta) override 
		{

		//UTI_Log("Bullet Update");

		Position += Velocity;
		Orientation = Velocity.ToAngle();
		mysprite.Position = Position;
		mysprite.Rotation = Orientation;

		if ((Position.x<0)||(Position.x>screenSizeX)||(Position.y<0)||(Position.y>screenSizeY))
			{
			//UTI_Log("Bullet Expired");				
			IsExpired = true;
			}
		}
};
