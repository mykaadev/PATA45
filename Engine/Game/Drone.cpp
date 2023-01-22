
#include "Drone.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>
#include "Player.h"

Drone::Drone(Properties* props) : BaseEnemy(props)
{
	m_CurrentHealth = 1;

	m_SineMovement.ticksCounter = 0;
	m_SineMovement.sineSpeed = 0.05f;
	m_SineMovement.sineAmplitude = (rand() % 10);
}

void Drone::Init()
{
	__super::Init();

	m_Animation->SetProperties("Drone", 1, 0, 16,100, true);
}

void Drone::Draw()
{
	__super::Draw();
}

void Drone::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();


	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		if (!m_IsDead && m_Body != nullptr)
		{
			m_SineMovement.ticksCounter += m_SineMovement.sineSpeed * deltaTime;
			xMovement = m_SineMovement.sineAmplitude * sin(m_SineMovement.ticksCounter);

			m_Body->SetLinearVelocity(b2Vec2(xMovement, 1.0f));

			// handle out of screen destroy
			if (m_Body->GetPosition().x < 0.0f - m_Width / 2 || m_Body->GetPosition().x > 960.0f + m_Width / 2
				|| m_Body->GetPosition().y > 700.0f && !m_IsDead && m_Body != nullptr)
			{
				m_IsDead = true;
			
				m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
				
				m_Animation->SetCurrentSprite(0);
				m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 50, false);
				SetSize(64, 64);
			}
		}

		if (m_IsDead)
		{
			if (GetAnimation()->GetCurrentSprite() >= 10)
			{
				Clean();
			}
		}
	}
}

void Drone::Clean()
{
	m_PendingKill = true;
}

	


void Drone::CheckCollision(GameObject* otherGameObject)
{
	if (dynamic_cast<Player*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Player*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Player*)otherGameObject)->TakeDamage(1);
		collided = true;

		m_IsDead = true;
	}

	if (dynamic_cast<Companion*>(otherGameObject) && !m_PendingKill && !dynamic_cast<Companion*>(otherGameObject)->GetIsDead() && !collided)
	{
		((Companion*)otherGameObject)->TakeDamage(1);
		collided = true;

		m_IsDead = true;
	}
}

void Drone::TakeDamage(int damage)
{
	
	m_CurrentHealth -= damage;
	
	if (m_CurrentHealth <= 0)
	{

		Explosion();
	}
}

void Drone::SetOriginPoint()
{
	if (m_Body == nullptr) return;
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
}


void Drone::Explosion()
{
	m_IsDead = true;
	
	m_Animation->SetCurrentSprite(0);
	m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
	SetSize(64, 64);
}
