#include "World.h"
#include <iostream>
#include "box2d.h"
#include "..\..\Game\ObjectHandler.h"
#include "LevelParser.h"
#include "..\Components\Camera.h"
#include <iostream>
#include "..\..\Game\Bullet.h"
#include "..\..\Game\Player.h"
#include "..\..\Game\RusherEnemy.h"

World* World::m_Instance = nullptr;

World::World()
{
}

World::~World()
{
	delete m_Level;
	delete m_Instance;
}


#pragma region SetupWorld


void World::SetupWorld()
{
	m_Level = LevelParser::GetInstance()->GetLevel("Level0");

	b2Vec2 gravity(0.0f, 0.0f);
	m_World = std::make_unique<b2World>(gravity);

	ObjectHandler::GetInstance()->LoadObjects();

	m_World->SetContactListener(this);

	///////////////////// REMOVE FROM HERE /////////////////////////
	//TEMP WORLD BOUNDARIES
	//top
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(480.0f, 0.0f);
	b2Body* yoh = m_World->CreateBody(&groundBodyDef);
	b2PolygonShape groundbox;
	groundbox.SetAsBox(960.0f, 1.0f);
	yoh->CreateFixture(&groundbox, 0.0f);

	//bot
	b2BodyDef groundBodyDef2;
	groundBodyDef2.position.Set(480.0f, 640.0f);
	b2Body* yoh2 = m_World->CreateBody(&groundBodyDef2);
	b2PolygonShape groundbox2;
	groundbox2.SetAsBox(960.0f, 1.0f);
	yoh2->CreateFixture(&groundbox2, 0.0f);

	///LEFT
	b2BodyDef groundBodyDef3;
	groundBodyDef3.position.Set(960.0f, 320.0f);
	b2Body* yoh3 = m_World->CreateBody(&groundBodyDef3);
	b2PolygonShape groundbox3;
	groundbox3.SetAsBox(1.0f, 640.0f);
	yoh3->CreateFixture(&groundbox3, 0.0f);

	//RIGHT
	b2BodyDef groundBodyDef4;
	groundBodyDef4.position.Set(0.0f, 320.0f);
	b2Body* yoh4 = m_World->CreateBody(&groundBodyDef4);
	b2PolygonShape groundbox4;
	groundbox4.SetAsBox(1.0f, 640.0f);
	yoh4->CreateFixture(&groundbox4, 0.0f);

	/////////////////// TO HERE ///////////////////////////

}


#pragma endregion


#pragma region Physics

void World::HandlePhysics(float deltaTime)
{  
	m_World->Step(deltaTime, 6, 2);
}

#pragma endregion


#pragma region Update


void World::Update(float deltaTime)
{
<<<<<<< Updated upstream
	CleanPendingKills();

=======
>>>>>>> Stashed changes
	HandlePhysics(deltaTime);

	for (int i = 0; i < GameObjectLoaded.size(); ++i)
	{
		GameObjectLoaded[i]->Update(deltaTime);
	}

	Camera::GetInstance()->Update(deltaTime);
<<<<<<< Updated upstream

	m_Level->Update();
=======
	m_Level->Update();

	CleanPendingKills();
>>>>>>> Stashed changes
}


#pragma endregion


#pragma region Render

void World::Render()
{

	for (int i = 0; i < GameObjectLoaded.size(); ++i)
	{
		GameObjectLoaded[i]->Draw();
	}

	m_Level->Render();
}

#pragma endregion


#pragma region ObjectDestruction

void World::DestroyGameObject(GameObject* inObject, b2Body* body /*= nullptr*/)
{

	if (body != nullptr) { NewBodyPendingKill(body); }

	auto it = std::find(GameObjectLoaded.begin(), GameObjectLoaded.end(), inObject);

	if (it != GameObjectLoaded.end())
	{
		int index = it - GameObjectLoaded.begin();

		delete GameObjectLoaded[index];

		GameObjectLoaded.erase(GameObjectLoaded.begin() + index);
	}
	else
	{
		std::cout << "ERROR: FAILED TO FIND OBJECT TO DESTROY" << std::endl;
	}
}

void World::CleanPendingKills()
{
	while (BodiesPendingKill.size() > 0)
	{
		if (BodiesPendingKill[0] != nullptr) { m_World->DestroyBody(BodiesPendingKill[0]); }
		BodiesPendingKill.erase(BodiesPendingKill.begin());
	}

}
#pragma endregion


#pragma region Collisions


void World::BeginContact(b2Contact* contact)
{
	GameObject* bodyA = ((GameObject*)contact->GetFixtureA()->GetUserData().pointer);
	GameObject* bodyB = ((GameObject*)contact->GetFixtureB()->GetUserData().pointer);

	if (bodyA != nullptr && bodyB != nullptr)
	{
		((Character*)bodyB)->CheckCollision((GameObject*)bodyA);
	}
}


#pragma endregion
