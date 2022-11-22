#include "ObjectHandler.h"
#include "Player.h"
#include "Bullet.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

Player* player = nullptr;
Bullet* bullet = nullptr;


ObjectHandler* ObjectHandler::m_Instance = nullptr;


void ObjectHandler::LoadObjects()
{
	/*------------Player--------------*/
	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);
	player->Init();

	/*-----------Bullet-----------------*/

	


	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



