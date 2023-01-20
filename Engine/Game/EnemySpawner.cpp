#include "EnemySpawner.h"
#include <iostream>
#include "EngineTime.h"
#include "LonerEnemy.h"
#include "Drone.h"
#include "World.h"
#include "RusherEnemy.h"


EnemySpawner* EnemySpawner::m_Instance = nullptr;



Uint32 SpawnLoner(Uint32 interval, void* data)
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		LonerEnemy* loner = nullptr;

		loner = new LonerEnemy(new Properties("Loner1", (rand() % (840 - 160 + 1) + 160) , -100, 64, 64, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(loner);
	}

	return interval;
}


Uint32 SpawnRusher(Uint32 interval, void* data)
{

	if (!World::GetInstance()->GetWorld()->IsLocked()) 
	{ 
		RusherEnemy* rusher = nullptr;

		rusher = new RusherEnemy(new Properties("Rusher1", (rand() % (840 - 160 + 1) + 160), 0, 64, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(rusher);
	}

	return interval;
}

Uint32 SpawnDrone(Uint32 interval, void* data) {

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		float x = -400;
		float randY = (rand() % (840 - 160 + 1) + 160);

		for (int i = 0; i < 8; ++i)
		{
			
			Drone* drone = new Drone(new Properties("Drone", x, randY, 32, 32));
			randY -= 50;
			x += 50;
			World::GetInstance()->LoadObjects(drone);			
		}
	
	}
	return interval;
}

EnemySpawner::EnemySpawner()
{

}


EnemySpawner::~EnemySpawner()
{
	delete m_Instance;
}

void EnemySpawner::Init()
{
	/*EngineTime::GetInstance()->StartTimer(3000, SpawnLoner, (void*)"");
	EngineTime::GetInstance()->StartTimer(3000, SpawnRusher, (void*)"");*/
	EngineTime::GetInstance()->StartTimer(5000, SpawnDrone, (void*)"");
}

