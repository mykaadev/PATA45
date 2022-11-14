#include "ObjectHandler.h"
#include "Player.h"
#include "Camera.h"
#include "Engine.h"


#define SCREEN_WIDTH 960
#define SCREEN_HEIGHT 640

Player* player = nullptr;


ObjectHandler* ObjectHandler::m_Instance = nullptr;


void ObjectHandler::LoadObjects()
{
	player = new Player(new Properties("Player", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 64, 64, SDL_FLIP_NONE));
	Engine::GetInstance()->LoadObjects(player);
	player->Init();
	
	Camera::GetInstance()->SetCameraTarget(player->GetOrigin());

}



void ObjectHandler::UpdateObjects(float deltaTime)
{
	Camera::GetInstance()->Update(deltaTime);
}

void ObjectHandler::RenderObjects()
{
}


