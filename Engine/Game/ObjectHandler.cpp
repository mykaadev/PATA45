#include "ObjectHandler.h"
#include "Player.h"
#include "Camera.h"
#include "Engine.h"
#include "World.h"


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

Player* player = nullptr;


ObjectHandler* ObjectHandler::m_Instance = nullptr;


void ObjectHandler::LoadObjects()
{
	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	World::GetInstance()->LoadObjects(player);
	player->Init();
	
	Camera::GetInstance()->SetCameraFollowTarget(false);
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());
}



