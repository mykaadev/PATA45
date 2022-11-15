#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
#include "../Core/CodingHelper.h"
#include "box2d.h"
#include "World.h"


/*
* Create a void for shooting and input in hold 
* 
* Create an component for gun (with bullet class inside and render sprite ) fire rate 0.1
* 
*/
Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();
	SetAnimationState(Idle, 0);

	CodingHelper::GetInstance()->IncrementAmountToClearCounter(1);
}


void Player::Init()
{
	SetupBody();
}

Player::~Player()
{
    delete m_Animation;

	CodingHelper::GetInstance()->DecrementAmountToClearCounter(1);
}


void Player::Draw()
{
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}


void Player::Update(float deltaTime)
{
	HandleInput();
	SetOriginPoint();
    m_Animation->Update(deltaTime);
}


void Player::SetupBody()
{
	/*
	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);

	b2Body* body = World::GetInstance()->GetWorld().CreateBody(&bodyDef);

	// Define another box shape for our dynamic body
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;
	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture(&fixtureDef);
	*/
}


void Player::HandleInput()
{
	
	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) > -1 && InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) < 1)
	{
		SetAnimationState(Idle, 0);
		// add force and speeeeeeed trough the physics with float not ints and normalize the vector


	}
	
	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) == 1)
	{
 		SetAnimationState(MovingX, 1);
	}

	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) == -1)
	{
		SetAnimationState(MovingX, -1);
	}	
}

void Player::SetOriginPoint()
{
	m_Origin->X = m_Transform->X + m_Width / 2;
	m_Origin->Y = m_Transform->Y + m_Height / 2;
}

void Player::SetAnimationState(AnimationStates inCurrentAnimationState, float inAxisValue)
{

	if (inCurrentAnimationState == Idle && inAxisValue == 0)
	{
		m_Animation->SetProperties("ShipIdle", 1, 0, 1, 50, true);
	}

	if (inCurrentAnimationState == MovingX && inAxisValue > 0)
	{
		m_Animation->SetProperties("ShipRight", 1, 0, 3, 100, false);
	}


	if (inCurrentAnimationState == MovingX && inAxisValue < 0)
	{
		m_Animation->SetProperties("ShipLeft", 1, 0, 3, 100, false);
	}
}

void Player::Shooting()
{
	// in this function check holding 
}

void Player::Clean()
{
    TextureManager::GetInstance()->Clean();
}

