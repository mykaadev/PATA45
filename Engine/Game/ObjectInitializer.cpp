#include "ObjectInitializer.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"
#include "LonerEnemy.h"
#include "RusherEnemy.h"
#include "Renderer.h"
#include "Background.h"
#include "EngineTime.h"
#include "EnemySpawner.h"
#include "BackgroundProps.h"
#include "AsteroidSpawner.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

ObjectInitializer* ObjectInitializer::m_Instance = nullptr;

Player* player = nullptr;

Background* galaxy = nullptr;
Background* galaxy1 = nullptr;



void ObjectInitializer::LoadObjects()
{
	if (World::GetInstance()->GetWorld()->IsLocked()) { return; }

	galaxy = new Background(new Properties("Galaxy", SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 960, 640, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(galaxy);

	galaxy1 = new Background(new Properties("Galaxy1", SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 - 640, 960, 640, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(galaxy1);


	for (int i = 1; i <= 4; ++i)
	{
		for (int k = 1; k <= 2; ++k)
		{
			for (int j = 1; j <= 3; ++j)
			{
				if (k == 1)
				{
					BackgroundProps* galaxyProps = nullptr;
					galaxyProps = new BackgroundProps(new Properties("BackgroundProps" + std::to_string(j), 0 + 32, (rand() % (32 + 1 - SCREEN_HEIGHT * 3) - SCREEN_HEIGHT * 3), 64, 64, SDL_FLIP_NONE));
					World::GetInstance()->LoadObjects(galaxyProps);
					galaxyProps->SetAnimationProps("BackgroundProps" + std::to_string(j), false);
				}

				if (k == 2)
				{
					BackgroundProps* galaxyProps = nullptr;
					galaxyProps = new BackgroundProps(new Properties("BackgroundProps" + std::to_string(j), SCREEN_WIDTH - 32, (rand() % (32 + 1 - SCREEN_HEIGHT * 3) - SCREEN_HEIGHT * 3), 64, 64, SDL_FLIP_NONE));
					World::GetInstance()->LoadObjects(galaxyProps);
					galaxyProps->SetAnimationProps("BackgroundProps" + std::to_string(j), true);
				}

				if (k == 2 && j == 3)
				{
					break;
				}
			}
		}
	}
	

	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);

	EnemySpawner::GetInstance()->Init();
	AsteroidSpawner::GetInstance()->Init();

	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



