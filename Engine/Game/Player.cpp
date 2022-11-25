#include "Player.h"
#include "Bullet.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
#include "box2d.h"
#include "World.h"


/*
* Create a void for shooting and input in hold  done/ not input - done
* 
* Create an component for gun (with bullet class inside and render sprite ) fire rate 0.1  - with bug
* 
* Health system is local but almost finished 
* 
*/
Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();
	canShoot = true;
	currentHealth = maxHealth;
	fSpeed = 100;
}


void Player::Init()
{
	__super::Init();

	SetupBody();
	SetAnimationState(Idle, 0);
}

Player::~Player()
{
    delete m_Animation;
	World::GetInstance()->Destroy(m_Body);
}



void Player::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();
	Move();
	HandleInput();
	HealthHandler(currentHealth);
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
	_boxShape.SetAsBox(m_Width/2, m_Height/2);
	
	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.density = 0.1f;
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



	if (!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) { canShoot = true; }
	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && canShoot)
	{
		FireGun();
	}

}


void Player::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
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

void Player::Move()
{
	m_Body->SetLinearVelocity(b2Vec2(InputHandler::GetInstance()->GetAxisKeys().X * fSpeed, InputHandler::GetInstance()->GetAxisKeys().Y * -fSpeed));
}

void Player::FireGun()
{
	canShoot = false;

	Bullet* bullet = nullptr;

	bullet = new Bullet(new Properties("Bullet", m_Body->GetPosition().x, m_Body->GetPosition().y - 33, 16, 16, SDL_FLIP_NONE));

	World::GetInstance()->LoadObjects(bullet);
}


void Player::HealthHandler(int damage)
{
	isDead = false;
	maxHealth -= damage;

	if (currentHealth <= 0)
	{
		DeathAnimation();
		isDead = true;
	}
}


void Player::DeathAnimation()
{
	SetAnimationState(Dead, 0);	
}


void Player::Clean()
{
    TextureManager::GetInstance()->Clean();
}

