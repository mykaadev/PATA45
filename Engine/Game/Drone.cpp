
#include "Drone.h"
#include "World.h"
#include "EngineTime.h"
#include "Player.h"
#include <iostream>
#include <cmath>

/*
Drone::Drone(Properties* props) : BaseEnemy(props)
{
	m_CurrentHealth = m_MaxHealth;
	m_GoingRight = true;
	m_SinCounter = 0;
}

void Drone::Init()
{
	__super::Init();
	SetupBody();

	m_Animation->SetProperties("Drone", 1, 0, 16, 100, true);
	SpawnDrone();
}

void Drone::Draw()
{
	__super::Draw();
}

void Drone::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();
	m_Animation->Update(deltaTime);

	if (!m_IsDead && m_Body != nullptr)
	{
		// Horizontal sinusoidal move
		m_SinCounter += deltaTime;
		float xPos = m_Body->GetPosition().x + sin(m_SinCounter) * 10.0f;
		m_Body->SetTransform(b2Vec2(xPos, m_Body->GetPosition().y), 0.0f);

		//Handle Out of screen destroy
		if (m_Body->GetPosition().y > 640.0f + m_Height / 2 && !m_IsDead)
		{
			m_IsDead = true;
			if (!World::GetInstance()->GetWorld()->IsLocked())
			{
				m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			}
			m_Animation->SetCurrentSprite(0);
			m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
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

void Drone::Clean()
{
}

void Drone::CheckCollision(GameObject* otherGameObject)
{
}

void Drone::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;
	if (m_CurrentHealth <= 0)
	{
		m_IsDead = true;
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		m_Animation->SetCurrentSprite(0);
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
	}
}

void Drone::SetOriginPoint()
{
}

void Drone::SpawnDrone() {
	if (!World::GetInstance()->GetWorld()->IsLocked()) {
		int r = rand() % 2;
		switch (r) {
		case 0:
			m_Body->SetTransform(b2Vec2(rand() % (960 - (int)m_Width), -m_Height), 0);
			break;
		case 1:
			m_Body->SetTransform(b2Vec2(rand() % (960 - (int)m_Width), 640 + m_Height), 0);
			break;
		}
	}
}

*/