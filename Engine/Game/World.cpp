#include "World.h"
#include "box2d.h"
#include <stdio.h>

World* World::m_Instance = nullptr;

World::World()
{

}

void World::SetupWorld()
{
	b2Vec2 gravity(0.0f, -10.0f);
	myWorld = b2World(gravity);

}

void World::HandlePhysics(float deltaTime)
{
	// Instruct the world to perform a single step of simulation.
	// It is generally best to keep the time step and iterations fixed.

	//        world.Step(deltaTime, 6, 2);

	
	// Now print the position and angle of the body.
	
	//        b2Vec2 position = playerBody->GetPosition();
	//        float angle = playerBody->GetAngle();
	//        printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);


}
