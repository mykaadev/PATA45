#include "Player.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
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
}


void Player::Init()
{
	
	SetupBody();
	SetAnimationState(Idle, 0);
}

Player::~Player()
{
    delete m_Animation;
}



void Player::Update(float deltaTime)
{
	HandleInput();
	SetOriginPoint();

	//m_Body->SetLinearVelocity(b2Vec2(InputHandler::GetInstance()->GetAxisKeysAsVector()));

	std::cout << "PLAYER POSITION: " << m_Body->GetPosition().x << " " << m_Body->GetPosition().y << std::endl;

	m_Animation->Update(deltaTime);
}

void Player::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Player::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_dynamicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;

	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(1.0f, 1.0f);
	
	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.density = 1.0f;
	_fixtureDef.friction = 0.3f;
	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}



//TODO: FIX INPUTS SO WE CAN BUILD A VECTOR2 WITH THE X AND Y INPUTS
// SO WE CAN NORMALIZE IT AFTER



void Player::HandleInput()
{
	
	
	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) > -1 && InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) < 1 ||
		InputHandler::GetInstance()->GetAxisKeys(VERTICAL) > -1 && InputHandler::GetInstance()->GetAxisKeys(VERTICAL) < 1)
	{
		SetAnimationState(Idle, 0);
		// add force and speeeeeeed trough the physics with float not ints and normalize the vector
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
	
	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) == 1)
	{
 		SetAnimationState(MovingX, 1);
		m_Body->SetLinearVelocity(b2Vec2(5.0f, 0.0f));
	}

	if (InputHandler::GetInstance()->GetAxisKeys(HORIZONTAL) == -1)
	{
		SetAnimationState(MovingX, -1);
		m_Body->SetLinearVelocity(b2Vec2(-5.0f, 0.0f));
	}	

	
}

void Player::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x + m_Width / 2;
	m_Origin->Y = m_Body->GetPosition().y- + m_Height / 2;
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

