#include "EnemySpawner.h"
#include <iostream>
#include "EngineTime.h"
#include "LonerEnemy.h"
#include "World.h"
#include "RusherEnemy.h"


EnemySpawner* EnemySpawner::m_Instance = nullptr;
LonerEnemy* loner1 = nullptr;
LonerEnemy* loner2 = nullptr;
LonerEnemy* loner3 = nullptr;

RusherEnemy* rusher1 = nullptr;
RusherEnemy* rusher2 = nullptr;
RusherEnemy* rusher3 = nullptr;


Uint32 SpawnLoner(Uint32 interval, void* data)
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{

	loner1 = new LonerEnemy(new Properties("Loner1", (rand() % (840 - 160 + 1) + 160) , -100, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(loner1);
	
	loner2 = new LonerEnemy(new Properties("Loner2", (rand() % (840 - 160 + 1) + 160), -200, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(loner2);

	loner3 = new LonerEnemy(new Properties("Loner3", (rand() % (840 - 160 + 1) + 160) , -300, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(loner3);
	}

	return interval;
}



Uint32 SpawnRusher(Uint32 interval, void* data)
{

	if (!World::GetInstance()->GetWorld()->IsLocked()) 
	{ 
	rusher1 = new RusherEnemy(new Properties("Rusher1", (rand() % (840 - 160 + 1) + 160), 0, 64, 32, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(rusher1);

	rusher2 = new RusherEnemy(new Properties("Rusher2", (rand() % (840 - 160 + 1) + 160), -50, 64, 32, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(rusher2);

	rusher3 = new RusherEnemy(new Properties("Rusher3", (rand() % (840 - 160 + 1) + 160), -100, 64, 32, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(rusher3);
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

	EngineTime::GetInstance()->StartTimer(20000, SpawnLoner, (void*)"");
	EngineTime::GetInstance()->StartTimer(20000, SpawnRusher, (void*)"");
}

