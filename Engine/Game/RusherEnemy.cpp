#include "RusherEnemy.h"
#include "BaseEnemy.h"
#include "World.h"


RusherEnemy::RusherEnemy(Properties* props) : BaseEnemy(props)
{
	m_GoingRight = true;

	m_MaxHealth = { 5 };

	m_CurrentHealth = m_MaxHealth;
}


void RusherEnemy::Init()
{ 
	__super::Init();

	//Handle Animations
	m_Animation->SetProperties("Rusher", 1, 0, 24, 50, true);
}

void RusherEnemy::Draw()
{
	__super::Draw();
}

void RusherEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animation->Update(deltaTime);

	
	//Handle Movement
	if (m_GoingRight && m_Body->GetPosition().x < 900)
	{
		m_Body->SetLinearVelocity(b2Vec2(1.5f, 0.1f));
	}

	if (m_GoingRight && m_Body->GetPosition().x >= 900)
	{
		m_GoingRight = false;
	}
	
	if (!m_GoingRight && m_Body->GetPosition().x > 900)
	{
		m_Body->SetLinearVelocity(b2Vec2(-1.5f, 0.1f));
	}

	if (!m_GoingRight && m_Body->GetPosition().x <= 60)
	{
		m_GoingRight = true;
	}

	//Handle Out of screen destroy
	if (m_Body->GetPosition().y > 700.0f && !m_IsDead)
	{
		m_IsDead = true;
	}

	if (m_IsDead)
	{
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 50, false);

		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}
}


void RusherEnemy::TakeDamage(int inDamage)
{
	m_CurrentHealth -= inDamage;

	if (m_CurrentHealth <= 0)
	{
		m_IsDead = true;
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 50, false);
	}
}

void RusherEnemy::Clean()
{
	World::GetInstance()->DestroyGameObject(this, m_Body);
}

void RusherEnemy::CheckCollision(GameObject* otherGameObject)
{

}
