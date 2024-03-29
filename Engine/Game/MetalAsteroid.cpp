#include "MetalAsteroid.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>
#include "Player.h"

MetalAsteroid::MetalAsteroid(Properties* props) : BaseAsteroid(props)
{
	b_IsIndestructible = true;
	r = 0;
	m_GoingRight = true;

}

/* Set the default asteroid for the small one */
void MetalAsteroid::Init()
{
	__super::Init();

	SetupBody();

	m_Animation->SetProperties("MetalSmall", 1, 0, 16, 100, true);
	SpawnAsteroid();
}

void MetalAsteroid::Draw()
{
	__super::Draw();
}

void MetalAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		if (!m_IsDead && m_Body != nullptr)
		{
			if (m_GoingRight && m_Body->GetPosition().x < 960 - m_Width / 2)
			{
				m_Body->SetLinearVelocity(b2Vec2(2.0f, 1.0f));
			}

			if (m_GoingRight && m_Body->GetPosition().x >= 960 - m_Width / 2)
			{
				m_GoingRight = false;
			}

			if (!m_GoingRight && m_Body->GetPosition().x > 900 - m_Width / 2)
			{
				m_Body->SetLinearVelocity(b2Vec2(-2.0f, 1.0f));

			}

			if (!m_GoingRight && m_Body->GetPosition().x <= 0 + m_Width / 2)
			{
				m_GoingRight = true;
			}

			//Handle Out of screen destroy
			if (m_Body->GetPosition().y > 640.0f + m_Height / 2 && !m_IsDead)
			{
				m_IsDead = true;

				if (m_Body != nullptr)
				{
					m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				}
				
				m_Animation->SetCurrentSprite(0);
				m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
				SetSize(64, 64);
			}
		}
	}
	
}

void MetalAsteroid::Clean()
{
	m_PendingKill = true;
}

void MetalAsteroid::CheckCollision(GameObject* otherGameObject)
{
	if (dynamic_cast<Player*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Player*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Player*)otherGameObject)->TakeDamage(1);
		collided = true;		
	}

	if (dynamic_cast<Companion*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Companion*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Companion*)otherGameObject)->TakeDamage(1);
		collided = true;

		m_IsDead = true;
	}

	if (!dynamic_cast<Player*>(otherGameObject) && !dynamic_cast<Companion*>(otherGameObject))
	{
		collided = false;
	}
}

void MetalAsteroid::GiveDamage()
{
	
}


void MetalAsteroid::SpawnAsteroid()
{
	
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		int r = rand() % 3;

		switch (r) {
		case 0:
			size_ = Big;
			m_Animation->SetProperties("Metal", 1, 0, 25, 200, true);
			SetSize(96, 96);
			break;
		case 1:
			size_ = Medium;
			m_Animation->SetProperties("MetalMedium", 1, 0, 24, 200, true);
			SetSize(64, 64);
			break;
		case 2:
			size_ = Small;
			m_Animation->SetProperties("MetalSmall", 1, 0, 16, 200, true);
			SetSize(32, 32);
			break;

		}

	}
}


void MetalAsteroid::SetOriginPoint()
{
	if (m_Body != nullptr)
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
	
}
