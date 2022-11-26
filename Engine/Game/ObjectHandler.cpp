#include "ObjectHandler.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"
#include "LonerEnemy.h"
#include "RusherEnemy.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

ObjectHandler* ObjectHandler::m_Instance = nullptr;

Player* player = nullptr;

LonerEnemy* loner = nullptr;
RusherEnemy* rusher = nullptr;


void ObjectHandler::LoadObjects()
{

	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);

	loner = new LonerEnemy(new Properties("Loner", SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 - 100, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(loner);

	rusher = new RusherEnemy(new Properties("Rusher", SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2 - 100, 64, 32, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(rusher);

	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



