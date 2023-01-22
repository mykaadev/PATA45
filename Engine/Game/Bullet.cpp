#include "Bullet.h"
#include "World.h"
#include "Renderer.h"
#include "BaseEnemy.h"
#include "StoneAsteroid.h"
#include "WeaponPowerUp.h"
#include "ShieldPowerUp.h"
#include "Player.h"
#include "RusherEnemy.h"
#include "MetalAsteroid.h"

Bullet::Bullet(Properties* props) : Character(props) {

	m_Animation = new Animation();
	m_damageAmount = { 1 };

}

void Bullet::Init()
{
	if (m_Body == nullptr) { SetupBody(); }

	if (m_Body == nullptr) { return; }

	if (m_damageAmount == 1)
	{
		m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
		SetSize(16, 16);
	}
	else if (m_damageAmount == 2)
	{
		m_Animation->SetProperties("BulletTwo", 1, 0, 2, 50, true);
		SetSize(16,16);
	}
	else {
	
		m_Animation->SetProperties("BulletThree", 1, 0, 2, 50, true);
		SetSize(16, 16);
	}

//	m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, -3.0f));
	}
}



void Bullet::SetupBody()
{	
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		b2BodyDef _BodyDef;
		_BodyDef.type = b2_dynamicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;
		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);


		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2 - 4, m_Height / 2 - 4);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;
		_fixtureDef.density = 0.01f;
		_fixtureDef.friction = 0.01f;

		b2Fixture* _Fixture;
		
		_Fixture = m_Body->CreateFixture(&_fixtureDef);
	}
}



void Bullet::CheckIfIsOutOfBounds()
{
	if (m_Body == nullptr) { return;	}
	if (m_Body->GetPosition().y < 10.0f)
	{
		m_IsDead = true;
	}

	if (m_IsDead)
	{
		if (m_Body != nullptr)
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}

		m_Animation->SetProperties("Explosion", 1, 0, 11, 100, false);
		
		if (m_Animation->GetAnimationFinished())
		{
			Clean();
		}
	}
}

void Bullet::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animation->Update(deltaTime);

	SetOriginPoint();

	CheckIfIsOutOfBounds();
}


void Bullet::Draw()
{
	if (m_Body == nullptr) { return; }
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Bullet::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}


void Bullet::Clean()
{
	m_PendingKill = true;
	//World::GetInstance()->DestroyGameObject(this, m_Body);
	
}

void Bullet::CheckCollision(GameObject* otherGameObject)
{
	
	if (dynamic_cast<BaseEnemy*>(otherGameObject) && !m_PendingKill && !dynamic_cast<BaseEnemy*>(otherGameObject)->GetIsDead())
	{
		((BaseEnemy*)otherGameObject)->TakeDamage(m_damageAmount);
		m_IsDead = true;
	}

	if (dynamic_cast<StoneAsteroid*>(otherGameObject) && !m_PendingKill && !dynamic_cast<StoneAsteroid*>(otherGameObject)->GetIsDead())
	{
		((StoneAsteroid*)otherGameObject)->TakeDamage(m_damageAmount);
		m_IsDead = true;
	}	
	
	if (dynamic_cast<MetalAsteroid*>(otherGameObject) && !m_PendingKill && !dynamic_cast<MetalAsteroid*>(otherGameObject)->GetIsDead())
	{
		m_IsDead = true;
	}
}


void Bullet::ChooseType(int type)
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
	
		switch (type) {
		case 0:
			type = light;
			m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
			m_damageAmount = 1;
			break;
		case 1:
			type = medium;
			m_Animation->SetProperties("BulletTwo", 1, 0, 24, 50, true);
			m_damageAmount = 2;
			break;
		case 2:
			type = hard;
			m_Animation->SetProperties("BulletThree", 1, 0, 16, 50, true);
			break;

		default:
			m_Animation->SetProperties("BulletOne", 1, 0, 2, 100, true);
			m_damageAmount = 3;
			break;
		}
	}
}



Bullet::~Bullet()
{
	delete m_Animation;
}

