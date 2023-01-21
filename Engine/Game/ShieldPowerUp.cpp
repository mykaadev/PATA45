#include "World.h"
#include "EngineTime.h"
#include "Player.h"
#include <iostream>
#include "ShieldPowerUp.h"



ShieldPowerUp::ShieldPowerUp(Properties* props): BasePowerUp(props)
{
	m_CurrentHealth = 1;
	addLife = 0;
}

void ShieldPowerUp::Init()
{
	__super::Init();

	m_Animation->SetProperties("ShieldPowerUp", 1, 0, 8, 60, true);

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Body->SetLinearVelocity(b2Vec2(0.f, 1.5f));
	}
}

void ShieldPowerUp::Draw()
{
	__super::Draw();
}


void ShieldPowerUp::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();

	if (m_Body->GetPosition().y > 700.0f && !m_IsDead && m_Body != nullptr)
	{
		m_IsDead = true;
		m_Height = m_Width;


		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		m_Animation->SetCurrentSprite(0);

	}

	if (m_IsDead)
	{
		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}

	m_Animation->Update(deltaTime);

}

void ShieldPowerUp::Clean()
{
	m_PendingKill = true;
}

void ShieldPowerUp::CheckCollision(GameObject* otherGameObject)
{
	m_PendingKill = true;
}

void ShieldPowerUp::TakeDamage(int damage)
{
	m_CurrentHealth -= damage;

	if (m_CurrentHealth <= 0 && !m_IsDead)
	{
		m_IsDead = true;
		if (m_Body != nullptr && !World::GetInstance()->GetWorld()->IsLocked())
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		m_Animation->SetCurrentSprite(0);
	}
}

void ShieldPowerUp::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		m_Origin->X = m_Body->GetPosition().x;
		m_Origin->Y = m_Body->GetPosition().y;
	}
}

void ShieldPowerUp::ApplyPowerUp(int amount)
{
	Player* player = nullptr;
	amount = addLife +=1;
	player->AddSheildPowerUp(amount);
}
