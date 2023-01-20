
#include "Drone.h"
#include "World.h"
#include "EngineTime.h"
#include <iostream>

Drone::Drone(Properties* props) : BaseEnemy(props)
{
	m_SinCounter = 0.0f;
	m_Speed = 1.5f;
	m_Amplitude = 50.f;
	y = 0;
	m_CurrentHealth = 1;

}

void Drone::Init()
{
	__super::Init();

	SetupBody();

	m_Animation->SetProperties("Drone", 1, 0, 16,100, true);

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

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		if (!m_IsDead && m_Body != nullptr)
		{
			// calculate horizontal sinusoidal movement
			
			float amplitude = 5.0f; 
			float frequency = 0.1f; 
			float offset = EngineTime::GetInstance()->GetDeltaTime() * frequency;
			float horizontalVelocity = amplitude * sin(offset);

			// handle out of screen destroy
			if (m_Body->GetPosition().x < 0.0f - m_Width / 2 || m_Body->GetPosition().x > 800.0f + m_Width / 2)
			{
				m_IsDead = true;
				if (!World::GetInstance()->GetWorld()->IsLocked())
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

void Drone::Clean()
{
	m_PendingKill = true;
}

void Drone::SpawnDrone()
{

	//create and load 8 drones into the world
	for (int i = 0; i < 8; ++i)
	{
		x = m_Body->GetPosition().x;
		y = m_Body->GetPosition().y;
		Drone* drone = new Drone(new Properties("Drone", m_Body->GetPosition().x, m_Body->GetPosition().y + y, 32, 32));
		drone->m_Animation->SetProperties("Drone", 1, 0, 16, 100, true);
		drone->m_CurrentHealth = 1;
		SetSize(32, 32);
		World::GetInstance()->LoadObjects(drone);
		y += 32;
	}
}


void Drone::CheckCollision(GameObject* otherGameObject)
{

}

void Drone::TakeDamage(int damage)
{
	if (damage > 0)
	{
		m_CurrentHealth -= damage;
	}
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
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
	}
	m_Animation->SetCurrentSprite(0);
	m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
	SetSize(64, 64);
}
