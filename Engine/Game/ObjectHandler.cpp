#include "ObjectHandler.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"
#include "LonerEnemy.h"
#include "RusherEnemy.h"
#include "TextureManager.h"
#include "Background.h"
#include "EngineTime.h"
#include "EnemySpawner.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

ObjectHandler* ObjectHandler::m_Instance = nullptr;

Player* player = nullptr;

Background* galaxy = nullptr;
Background* galaxy1 = nullptr;


void ObjectHandler::LoadObjects()
{

	galaxy = new Background(new Properties("Galaxy", SCREEN_WIDTH / 2, SCREEN_HEIGHT/2, 960, 640, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(galaxy);

	galaxy1 = new Background(new Properties("Galaxy1", SCREEN_WIDTH / 2, SCREEN_HEIGHT/2 - 640, 960, 640, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(galaxy1);

	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);

	EnemySpawner::GetInstance()->Init();

	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



