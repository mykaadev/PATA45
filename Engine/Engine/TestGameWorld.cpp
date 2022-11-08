#include "TestGameWorld.h"
#include "box2d.h"
#include <stdio.h>




/*


int main()
{


	/////////////////////////////////////  WORLD ///////////////////////////////////////

	//Gravity Force
	b2Vec2 gravity(0.0f, -10.0f);

	//Create World
	b2World world(gravity);

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 6;
	int32 positionIterations = 2;




	/////////////////////////////////////  GROUND ///////////////////////////////////////

	//Defining Ground Body
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);

	//Call the body factory which allocates memory for the ground body from a pool and creates the ground box shape
	//the body is also added to the world
	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	//define the ground box shape
	b2PolygonShape groundBox;

	// the extents are the half widths of the box;
	groundBox.SetAsBox(50.0f, -10.0f);

	//Add the ground fixture to the groundbody
	groundBody->CreateFixture(&groundBox, 0.0f);



	/////////////////////////////////////  DYNAMIC ///////////////////////////////////////

	b2BodyDef dynamicBodyDef;
	dynamicBodyDef.type = b2_dynamicBody;
	dynamicBodyDef.position.Set(0.0f, 5.0f);
	b2Body* dynamicBody = world.CreateBody(&dynamicBodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef dynamicFixtureDef;
	dynamicFixtureDef.shape = &dynamicBox;

	dynamicFixtureDef.density = 1.0f;

	dynamicFixtureDef.friction = 0.2f;

	dynamicBody->CreateFixture(&dynamicFixtureDef);




	/////////////////////////////////////  LOOP ///////////////////////////////////////

	// This is our little game loop.
	for (int32 i = 0; i < 60; ++i)
	{
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step(timeStep, velocityIterations, positionIterations);
		// Now print the position and angle of the body.
		b2Vec2 position = dynamicBody->GetPosition();
		float angle = dynamicBody->GetAngle();
		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);
	}
}

*/