#include <iostream>
#include "World.h"
#include "EngineTime.h"
#include "MetalAsteroid.h"
#include "StoneAsteroid.h"
#include "AsteroidSpawner.h"

AsteroidSpawner* AsteroidSpawner::m_Instance = nullptr;


Uint32 SpawnMetalAsteroids(Uint32 interval, void* data) {

	if (!World:: GetInstance()->GetWorld()->IsLocked())
	{
		MetalAsteroid* metalAsteroid = nullptr;

		metalAsteroid = new MetalAsteroid(new Properties("Metal", (rand() % (840 - 160 + 1) + 160), -100, 96, 96, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(metalAsteroid);
	}

	return interval;
}



Uint32 SpawnStoneAsteroids(Uint32 interval, void* data) {

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		StoneAsteroid*  stoneAsteroid = nullptr;

		stoneAsteroid = new StoneAsteroid(new Properties("Stone", (rand() % (840 - 260 + 1) + 160), -100, 96, 96, SDL_FLIP_NONE));	
		World::GetInstance()->LoadObjects(stoneAsteroid);
	}

	return interval;	
}



AsteroidSpawner::~AsteroidSpawner()
{
	delete m_Instance;
}

void AsteroidSpawner::Init()
{
//	EngineTime::GetInstance()->StartTimer(2500, SpawnMetalAsteroids, (void*)"");
	EngineTime::GetInstance()->StartTimer(5000, SpawnStoneAsteroids, (void*)"");
	 
}
