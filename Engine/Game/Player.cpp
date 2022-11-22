#include "Player.h"
#include "Bullet.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
#include "box2d.h"
#include "World.h"


/*
* Create a void for shooting and input in hold  done/ not input
* 
* Create an component for gun (with bullet class inside and render sprite ) fire rate 0.1
* 
*/
Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();
	canShoot = true;
}


void Player::Init()
{
	
	SetupBody();
	SetAnimationState(Idle, 0);

	fSpeed = 5.0f;
}

Player::~Player()
{
    delete m_Animation;
}



void Player::Update(float deltaTime)
{
	HandleInput();
	SetOriginPoint();
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


void Player::HandleInput()
{
	if (InputHandler::GetInstance()->GetAxisKeys().X == 0 && InputHandler::GetInstance()->GetAxisKeys().Y == 0)
	{
		SetAnimationState(Idle, InputHandler::GetInstance()->GetAxisKeys().X);
	}
	
	if (InputHandler::GetInstance()->GetAxisKeys().X < 0 || InputHandler::GetInstance()->GetAxisKeys().X > 0)
	{
 		SetAnimationState(MovingX, InputHandler::GetInstance()->GetAxisKeys().X);
	}

	if (InputHandler::GetInstance()->GetAxisKeys().Y < 0 || InputHandler::GetInstance()->GetAxisKeys().Y > 0)
	{
		SetAnimationState(Idle, InputHandler::GetInstance()->GetAxisKeys().X);
	}	

	m_Body->SetLinearVelocity(b2Vec2(InputHandler::GetInstance()->GetAxisKeys().X * fSpeed, InputHandler::GetInstance()->GetAxisKeys().Y * -fSpeed));

	// Input shooting

	if (!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) { canShoot = true; }
	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && canShoot)
	{
		canShoot = false;
 		bullet = new Bullet(new Properties("Bullet", m_Body->GetPosition().x, m_Body->GetPosition().y - 1, 16, 16, SDL_FLIP_NONE));
		myBullets.push_back((GameObject*)bullet);
		World::GetInstance()->LoadObjects(myBullets[myBullets.size() - 1]);
		myBullets[myBullets.size() - 1]->Init();
	}

}

// position
void Player::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x + m_Width / 2;
	m_Origin->Y = m_Body->GetPosition().y -  m_Height / 2;
}

// animation states according the input
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
// 	/*
// 	bullet = new Bullet(new Properties("Bullet", m_Body->GetPosition().x, m_Body->GetPosition().y - 1, 16, 16, SDL_FLIP_NONE));
// 	myBullets.push_back((GameObject*)bullet);
// 	myBullets[myBullets.size()-1]->Init();
// 	World::GetInstance()->LoadObjects(myBullets[myBullets.size()-1]);
// 
// 		Object->Update(deltaTime);
// 		*/
}

void Player::Clean()
{
    TextureManager::GetInstance()->Clean();
}

