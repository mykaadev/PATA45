#include <iostream>
#include "World.h"
#include "EngineTime.h"
#include "WeaponPowerUp.h"
#include "ShieldPowerUp.h"
#include "PowerUpsSpawner.h"



PowerUpsSpawner* PowerUpsSpawner::m_Instance = nullptr;


Uint32 SpawnShieldPowerup(Uint32 interval, void* data) {

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		ShieldPowerUp* shieldPowerUp = nullptr;

		shieldPowerUp = new ShieldPowerUp(new Properties("ShieldPowerUp", (rand() % (840 - 160 + 1) + 160), -100, 32, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(shieldPowerUp);
	}

	return interval;
}




Uint32 SpawnWeaponPowerup(Uint32 interval, void* data) {

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		WeaponPowerUp* weaponPowerUp = nullptr;

		weaponPowerUp = new WeaponPowerUp(new Properties("WeaponPowerUp", (rand() % (840 - 260 + 1) + 160), -100, 32, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(weaponPowerUp);
	}

	return interval;
}


PowerUpsSpawner::~PowerUpsSpawner()
{
	delete m_Instance;
}

void PowerUpsSpawner::Init()
{
	EngineTime::GetInstance()->StartTimer(5000, SpawnShieldPowerup, (void*)"");
	EngineTime::GetInstance()->StartTimer(5000, SpawnWeaponPowerup, (void*)"");
}
