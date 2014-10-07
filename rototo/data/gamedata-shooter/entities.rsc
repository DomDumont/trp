class Entity
{

	// VARIABLES
	float 		Radius = 20;
	Sprite		mysprite;
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
	Position.x = 400;
	Position.y = 350;
	Orientation = 0;
	}

void OnUpdate(uint64 _delta)
	{
        UTI_Log("Entity Update");
	}


void OnRender(uint64 _delta)
	{
	mysprite.Render();
	}	
};





class EntityManager
{
//VARIABLES

	array<Entity@> 	entities;
	array<Entity@> 	addedEntities;
	bool 		isUpdating;

 EntityManager()
    {
    isUpdating = false;
    UTI_Log("Entity Manager Constructor\n");
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
	isUpdating = false;
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

