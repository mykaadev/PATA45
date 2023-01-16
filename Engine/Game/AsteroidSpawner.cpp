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
		MetalAsteroid* asteroid = nullptr;

		asteroid->SpawnAsteroid();
		asteroid = new MetalAsteroid(new Properties("Metal", (rand() % (840 - 160 + 1) + 160), -100, 64, 64, SDL_FLIP_NONE),);
		World::GetInstance()->LoadObjects(asteroid);

		//asteroid = new MetalAsteroid(new Properties("Asteroid"))
	}
	return interval;
}


/*
Uint32 SpawnStoneAsteroids(Uint32 interval, void* data) {

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		StoneAsteroid*  stoneAsteroid = nullptr;

		//stoneAsteroid = new MetalAsteroid(new Properties("Asteroid"))
	}
}
*/


AsteroidSpawner::~AsteroidSpawner()
{
	delete m_Instance;
}

void AsteroidSpawner::Init()
{
	EngineTime::GetInstance()->StartTimer(2500, SpawnMetalAsteroids, (void*)"");
	/*EngineTime::GetInstance()->StartTimer(3000, SpawnStoneAsteroids(), (void*)"");*/
}
