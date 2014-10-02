class Entity
{

	// VARIABLES
	float 		Radius = 20;
	Sprite		Sprite;
	bool 		IsExpired;
	Vector2D 	Position;
	Vector2D 	Velocity;
	float		Orientation = 0;

Entity()
{
	IsExpired = false;
}
void Init()
	{
	UTI_Log("Entity Init");
	Orientation = 0;
	}

void OnUpdate(uint64 _delta)
	{
		//UTI_Log("Entity Update");
		Sprite.SetPosition(Position.x,Position.y);
		Sprite.SetRotation(Orientation);
	}


void OnRender(uint64 _delta)
	{
		Sprite.Render();
	}	
};





class EntityManager
{
//VARIABLES

	array<Entity@> 	entities;
	array<Entity@> 	addedEntities;
	bool 			isUpdating;

 EntityManager()
    {
    isUpdating = false;
    }

void Add(Entity @ tempEnt)
	{		
	if (isUpdating == false)
        entities.insertLast(tempEnt);
    else
        addedEntities.insertLast(tempEnt);
	}

void Init()
	{	
	for (int i = 0;i< entities.length();i++)
		entities[i].Init();
	}
	

void OnUpdate(uint64 _delta)
	{
	isUpdating = true;
	for (int i = 0;i< entities.length();i++)
		entities[i].OnUpdate(_delta);		
	isUpdating = false;	

	for (int i = 0;i< addedEntities.length();i++)
		entities.insertLast(addedEntities[i]);		

	while(addedEntities.isEmpty() == false)
		addedEntities.removeLast();

	for (int i = 0;i< entities.length();)
		{
		if (entities[i].IsExpired == true)
			{
			entities.removeAt(i);
			}		
		else
			i++;
		}


	}	

void OnRender(uint64 _delta)
	{
	for (int i = 0;i< entities.length();i++)
		entities[i].OnRender(_delta);				
	}

};

