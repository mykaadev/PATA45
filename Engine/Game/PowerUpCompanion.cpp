#include "PowerUpCompanion.h"
#include "World.h"
#include "EngineTime.h"
#include "Bullet.h"
#include "Player.h"
#include <iostream>

PowerUpCompanion::PowerUpCompanion(Properties* props) : Character(props)
{
	
}

void PowerUpCompanion::Init()
{
	__super::Init();

	SetupBody();

	m_Animation->SetProperties("Companion", 1, 0, 16, 100, true);

	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));
	}
}


void PowerUpCompanion::SetupBody()
{
	if (m_Body != nullptr) return;
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		b2BodyDef _BodyDef;
		_BodyDef.type = b2_kinematicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;


		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2, m_Height / 2);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.density = 1.0f;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;


		b2Fixture* _Fixture;
		_Fixture = m_Body->CreateFixture(&_fixtureDef);
	}
}

void PowerUpCompanion::Draw()
{
	__super::Draw();
	if (m_Body == nullptr) { return; }
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}

void PowerUpCompanion::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();

	if (m_Body == nullptr) { return; }

	if (m_Body->GetPosition().y > 700.0f)
	{
		m_Height = m_Width;

		if (m_Body != nullptr && !World::GetInstance()->GetWorld()->IsLocked() && m_Body != nullptr)
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}
		

		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		m_Animation->SetCurrentSprite(0);

		Clean();
	}

	m_Animation->Update(deltaTime);
}

void PowerUpCompanion::Clean()
{
	m_PendingKill = true;
}


void PowerUpCompanion::CheckCollision(GameObject* otherGameObject)
{
	if (dynamic_cast<Player*>(otherGameObject) && !collided)
	{
		collided = true;
		dynamic_cast<Player*>(otherGameObject)->AddCompanion();
		Clean();

	}

}

void PowerUpCompanion::OnPickUp(GameObject* otherGameObject)
{
	
}


void PowerUpCompanion::SetOriginPoint()
{
	if (m_Body != nullptr) 
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
}

