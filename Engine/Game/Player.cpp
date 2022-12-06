#include "Player.h"
#include "Bullet.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputHandler.h"
#include "box2d.h"
#include "World.h"
#include "EngineTime.h"



Uint32 HoldingFire(Uint32 interval, void* data)
{

	// 	Bullet* bullet = nullptr;
	// 	bullet = new Bullet(new Properties("Bullet", ((Bullet*)data)->m_Body->GetPosition().x,
	// 		((Bullet*)data)->m_Body->GetPosition().y - 50, 16, 16, SDL_FLIP_NONE));

	Player* player = nullptr;
	((Player*)data)->FireGun();
	//	World::GetInstance()->LoadObjects(bullet);


	return interval;

}

Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();
	canShoot = true;
	maxHealth = 3;
	currentHealth = maxHealth;
	fSpeed = 1.5f;
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
}


void Player::Update(float deltaTime)
{
	__super::Update(deltaTime);

	BindAxisAndActions();
	SetOriginPoint();
	Move();

	m_Animation->Update(deltaTime);
}


void Player::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_dynamicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;
	_BodyDef.fixedRotation = true;

	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(m_Width/2, m_Height/2);
	
	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.userData.pointer = (uintptr_t) this;
	_fixtureDef.density = 0.1f;
	_fixtureDef.friction = 0.3f;

	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}


void Player::BindAxisAndActions()
{


	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_A) || InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT))
	{
		m_MoveAxis.X = -1;
	}

	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_D) || InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT))
	{
		m_MoveAxis.X = 1;
	}


	if (!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_D) && !InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT) && 
		!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_A) && !InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT))
	{
		m_MoveAxis.X = 0.0f;
	}

	if (!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_S) && !InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN) && 
		!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && !InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))
	{
		m_MoveAxis.Y = 0.0f;
	}


	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))
	{
		m_MoveAxis.Y = 1.0f;
	}

	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_S) || InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN))
	{
		m_MoveAxis.Y = -1.0f;
	}


	if (!InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)) 
	{ 
		canShoot = true; 
		EngineTime::GetInstance()->RemoveTimer(myTimerID);
	}
	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE) && canShoot)
	{
		Player::FireGun();
		myTimerID = EngineTime::GetInstance()->StartTimer(200, HoldingFire, (Player*)this);
		canShoot = false;
	}

}



void Player::Move()
{
	if (m_MoveAxis.X == 0 && m_MoveAxis.Y == 0)
	{
		SetAnimationState(Idle, m_MoveAxis.X);
	}

	if (m_MoveAxis.X < 0 || m_MoveAxis.X > 0)
	{
		SetAnimationState(MovingX, m_MoveAxis.X);
	}

	m_Body->SetLinearVelocity(b2Vec2(m_MoveAxis.X * fSpeed, m_MoveAxis.Y * -fSpeed));


}


void Player::FireGun()
{
	canShoot = false;

	Bullet* bullet = nullptr;

	bullet = new Bullet(new Properties("Bullet", m_Body->GetPosition().x, m_Body->GetPosition().y - 50, 16, 16, SDL_FLIP_NONE));

	World::GetInstance()->LoadObjects(bullet);

}


void Player::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
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


void Player::TakeDamage(int inDamage)
{
	currentHealth -= inDamage;

	if (currentHealth <= 0 && !isDead)
	{
		isDead = true;
		SetAnimationState(Dead, 0);
		World::GetInstance()->DestroyGameObject(this, m_Body);
	}
}


void Player::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Player::Clean()
{
    TextureManager::GetInstance()->Clean();
}

void Player::CheckCollision(GameObject* otherGameObject)
{
	
}

