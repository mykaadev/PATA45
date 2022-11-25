#include "World.h"
#include <iostream>
#include "box2d.h"
#include "..\..\Game\ObjectHandler.h"
#include "LevelParser.h"
#include "..\Components\Camera.h"

World* World::m_Instance = nullptr;

World::World()
{
}

World::~World()
{
	delete m_Level;
	delete m_Instance;
}

void World::SetupWorld()
{
	m_Level = LevelParser::GetInstance()->GetLevel("Level0");

	b2Vec2 gravity(0.0f, 0.0f);
	m_World = std::make_unique<b2World>(gravity);

	ObjectHandler::GetInstance()->LoadObjects();


	//REMOVE THIS
	// 
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

}

void World::HandlePhysics(float deltaTime)
{
	// Instruct the world to perform a single step of simulation.
	// It is generally best to keep the time step and iterations fixed.

	m_World->Step(deltaTime, 6, 2);
	//std::cout << "step" << std::endl;


// 	b2Vec2 position = playerBody->GetPosition();
// 	float angle = playerBody->GetAngle();
// 	printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
}

void World::Update(float deltaTime)
{
	HandlePhysics(deltaTime);

	for (int i = 0; i < GameObjectLoaded.size(); ++i)
	{
		GameObjectLoaded[i]->Update(deltaTime);
	}

	Camera::GetInstance()->Update(deltaTime);
	m_Level->Update();
}

void World::Render()
{
	for (auto Object : GameObjectLoaded) { Object->Draw(); }

	m_Level->Render();
}
