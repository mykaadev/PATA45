#include "Player.h"
#include "Bullet.h"
#include "../Graphics/Renderer.h"
#include "../Core/InputHandler.h"
#include "box2d.h"
#include "World.h"
#include "EngineTime.h"
#include "Companion.h"
#include "WeaponPowerUp.h"
#include "ShieldPowerUp.h"
#include "PowerUpCompanion.h"



Uint32 HoldingFire(Uint32 interval, void* data)
{
	((Player*)data)->FireGun();
	return interval;
}



Uint32 SpawnCompanionTimer(Uint32 interval, void* data)
{
	((Player*)data)->SpawnCompanion();
	EngineTime::GetInstance()->RemoveTimer(((Player*)data)->myTimerIDCompanion);
	return interval;
}

Player::Player(Properties* props) : Character(props) {
	
	m_Animation = new Animation();
	canShoot = true;
	maxHealth = 3;
	currentHealth = maxHealth;
	fSpeed = 2.0f;
	fireRate = 300.0f;
	m_MaxPowerLevel = 2;
	m_DamageAmount = 1;
	m_PowerLevel = 0;

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

	if (FirstCompanion != nullptr && m_Body != nullptr)
	{
		firstCompanionPosition = b2Vec2(m_Body->GetPosition().x + 50, m_Body->GetPosition().y - 10);
		dynamic_cast<Companion*>(FirstCompanion)->SetPosition(firstCompanionPosition, 0.15f);
	}

	if (SecondCompanion != nullptr && m_Body != nullptr)
	{
		secondCompanionPosition = b2Vec2(m_Body->GetPosition().x - 50, m_Body->GetPosition().y - 10);
		dynamic_cast<Companion*>(SecondCompanion)->SetPosition(secondCompanionPosition, 0.15f);
	}

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
	_fixtureDef.density = 0.01f;
	_fixtureDef.friction = 0.01f;

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
		canShoot = false;
		FireGun();
		myTimerID = EngineTime::GetInstance()->StartTimer(fireRate, HoldingFire, (Player*)this);

	}


	if (InputHandler::GetInstance()->GetKeyDown(SDL_SCANCODE_O))  AddCompanion();

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

	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(b2Vec2(m_MoveAxis.X * fSpeed, m_MoveAxis.Y * -fSpeed));
	}
}


void Player::SpawnCompanion()
{
	

	if (SecondCompanion == nullptr && FirstCompanion != nullptr && !secondCompanionSpawned)
	{
		SecondCompanion = new Companion(new Properties("Companion", m_Body->GetPosition().x - 50, m_Body->GetPosition().y + 50, 32, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(SecondCompanion);
		secondCompanionSpawned = true;
	}

	if (FirstCompanion == nullptr && !firstCompanionSpawned)
	{
		FirstCompanion = new Companion(new Properties("Companion", m_Body->GetPosition().x + 50, m_Body->GetPosition().y + 50, 32, 32, SDL_FLIP_NONE));
		World::GetInstance()->LoadObjects(FirstCompanion);
		firstCompanionSpawned = true;

	}	

}


void Player::FireGun()
{
	if (!World::GetInstance()->GetWorld()->IsLocked() && m_Body != nullptr)
	{
		canShoot = false;
		Bullet* bullet = nullptr;
		bullet = new Bullet(new Properties("Bullet", m_Body->GetPosition().x, m_Body->GetPosition().y - 32, 16, 16, SDL_FLIP_NONE));
		bullet->SetDamage(m_DamageAmount);
		World::GetInstance()->LoadObjects(bullet);

		if (FirstCompanion != nullptr && firstCompanionSpawned)
		{

			if (FirstCompanion->GetBody() == nullptr) { dynamic_cast<Companion*>(FirstCompanion)->Init(); }
			if (FirstCompanion->GetBody() == nullptr) { return; }
			Bullet* bullet = nullptr;
			b2Vec2 ShootPosition = dynamic_cast<Companion*>(FirstCompanion)->GetBody()->GetPosition();
			bullet = new Bullet(new Properties("Bullet", ShootPosition.x, ShootPosition.y - 32, 16, 16, SDL_FLIP_NONE));
			World::GetInstance()->LoadObjects(bullet);
		}

		if (SecondCompanion != nullptr && secondCompanionSpawned)
		{
			if (SecondCompanion->GetBody() == nullptr) { dynamic_cast<Companion*>(SecondCompanion)->Init(); }
			if (SecondCompanion->GetBody() == nullptr) { return; }
			Bullet* bullet = nullptr;
			b2Vec2 ShootPosition = dynamic_cast<Companion*>(SecondCompanion)->GetBody()->GetPosition();
			bullet = new Bullet(new Properties("Bullet", ShootPosition.x, ShootPosition.y - 32, 16, 16, SDL_FLIP_NONE));
			World::GetInstance()->LoadObjects(bullet);
		}

	}
}


void Player::SetOriginPoint()
{
	if (m_Body != nullptr)
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
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
		//World::GetInstance()->DestroyGameObject(this, m_Body);
		m_PendingKill = true;
	}
}


void Player::AddCompanion()
{
	if (m_PowerLevel <= m_MaxPowerLevel)
	{
		++m_PowerLevel;
// 		if (m_PowerLevel == 1) { myTimerIDCompanion = EngineTime::GetInstance()->StartTimer(100.0f, SpawnCompanionTimer, (Player*)this); fireRate = 200.0f; }
// 		if (m_PowerLevel == 2) { myTimerIDCompanion = EngineTime::GetInstance()->StartTimer(100.0f, SpawnCompanionTimer, (Player*)this); fireRate = 100.0f; }

		if (m_PowerLevel == 1) { SpawnCompanion(); fireRate = 200.0f; }
 		if (m_PowerLevel == 2) { SpawnCompanion(); fireRate = 100.0f; }

	}
	else
	{
		m_PowerLevel = m_MaxPowerLevel;
	}
}


void Player::AddSheildPowerUp(int morelife)
{
	currentHealth += morelife;

	if (currentHealth > maxHealth)
	{
		currentHealth = maxHealth;
	}
}


void Player::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Player::Clean()
{
    Renderer::GetInstance()->Clean();
}

void Player::CheckCollision(GameObject* otherGameObject)
{
	if (dynamic_cast<WeaponPowerUp*>(otherGameObject) && !m_PendingKill && !dynamic_cast<WeaponPowerUp*>(otherGameObject)->GetIsDead())
	{
		((WeaponPowerUp*)otherGameObject)->OnPickUp((GameObject*)this);
	}

	if (dynamic_cast<ShieldPowerUp*>(otherGameObject) && !m_PendingKill && !dynamic_cast<ShieldPowerUp*>(otherGameObject)->GetIsDead())
	{
		((ShieldPowerUp*)otherGameObject)->TakeDamage(m_DamageAmount);
	}
}

void Player::ChooseBulletType()
{

	bulletLevel++;

	if (bulletLevel == 0)
	{
		m_DamageAmount = 1;
	}
	else if (bulletLevel == 1)
	{
		m_DamageAmount = 2;
	}
	else {
		m_DamageAmount = 3;
	}


}