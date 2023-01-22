#include "LonerEnemy.h"
#include "World.h"
#include "EnemyBullet.h"
#include "EngineTime.h"
#include <iostream>



Uint32 Fire(Uint32 interval, void* data)
{

	if (!World::GetInstance()->GetWorld()->IsLocked()) {

		EnemyBullet* bullet = nullptr;

		bullet = new EnemyBullet(new Properties("EnemyBullet", 
				dynamic_cast<GameObject*>((LonerEnemy*)data)->m_Body->GetPosition().x, 
				dynamic_cast<GameObject*>((LonerEnemy*)data)->m_Body->GetPosition().y + 50, 16, 16, SDL_FLIP_NONE));
	
		World::GetInstance()->LoadObjects(bullet);
	}

	return interval;

}


LonerEnemy::LonerEnemy(Properties* props) : BaseEnemy(props)
{
	m_GoingRight = true;

	m_MaxHealth = { 1 };

	m_CurrentHealth = m_MaxHealth;
}


void LonerEnemy::Init()
{
	__super::Init();

	SetupBody();

	myTimerID = EngineTime::GetInstance()->StartTimer((rand() % (5000 - 1000 + 1) + 1000), Fire, (LonerEnemy*)this);

	m_Animation->SetProperties("Loner", 1, 0, 16, 50, true);
}

void LonerEnemy::Draw()
{
	__super::Draw();
}

void LonerEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);


	SetOriginPoint();

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

		}
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


void LonerEnemy::TakeDamage(int inDamage)
{
	m_CurrentHealth -= inDamage;

	if (m_CurrentHealth <= 0)
	{
		m_IsDead = true;

		if (m_Body != nullptr)
		{
			m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
		}

		m_Animation->SetCurrentSprite(0);
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
	}
}

void LonerEnemy::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
	
}

void LonerEnemy::Clean()
{
	EngineTime::GetInstance()->RemoveTimer(myTimerID);
	m_PendingKill = true;
}

void LonerEnemy::CheckCollision(GameObject* otherGameObject)
{

}




