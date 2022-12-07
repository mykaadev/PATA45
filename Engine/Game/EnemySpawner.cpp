#include "EnemySpawner.h"
#include <iostream>
#include "EngineTime.h"
#include "LonerEnemy.h"
#include "World.h"
#include "RusherEnemy.h"


EnemySpawner* EnemySpawner::m_Instance = nullptr;



Uint32 SpawnLoner(Uint32 interval, void* data)
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		LonerEnemy* loner1 = nullptr;

		loner1 = new LonerEnemy(new Properties("Loner1", (rand() % (840 - 160 + 1) + 160) , -100, 64, 64, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(loner1);
	}

	return interval;
}


Uint32 SpawnRusher(Uint32 interval, void* data)
{

	if (!World::GetInstance()->GetWorld()->IsLocked()) 
	{ 
		RusherEnemy* rusher1 = nullptr;

		rusher1 = new RusherEnemy(new Properties("Rusher1", (rand() % (840 - 160 + 1) + 160), 0, 64, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(rusher1);
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
	EngineTime::GetInstance()->StartTimer(3000, SpawnLoner, (void*)"");
	EngineTime::GetInstance()->StartTimer(3000, SpawnRusher, (void*)"");
}

