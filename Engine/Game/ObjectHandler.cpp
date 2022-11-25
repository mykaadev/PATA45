#include "ObjectHandler.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"

#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

ObjectHandler* ObjectHandler::m_Instance = nullptr;

Player* player = nullptr;


void ObjectHandler::LoadObjects()
{

	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);

	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



