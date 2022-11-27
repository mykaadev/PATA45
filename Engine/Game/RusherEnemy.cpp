#include "RusherEnemy.h"
#include "BaseEnemy.h"
#include "World.h"


RusherEnemy::RusherEnemy(Properties* props) : BaseEnemy(props)
{
	m_GoingRight = true;

	m_MaxHealth = { 2 };

	m_CurrentHealth = m_MaxHealth;
}


void RusherEnemy::Init()
{ 
	__super::Init();
}

void RusherEnemy::Draw()
{
	__super::Draw();
}

void RusherEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);

	//Handle Animations
	m_Animation->SetProperties("Rusher", 1, 0, 24, 50, true);

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
	if (m_Body->GetPosition().y > 700.0f)
	{
		World::GetInstance()->DestroyGameObject(this, m_Body);
	}

}


void RusherEnemy::TakeDamage(int inDamage)
{

	m_CurrentHealth -= inDamage;

	std::cout << "RUSHER " << m_CurrentHealth << std::endl;

	if (m_CurrentHealth <= 0)
	{
		std::cout << "RUSHER DEAD" << std::endl;

		World::GetInstance()->DestroyGameObject(this, m_Body);
	}
}

void RusherEnemy::Clean()
{
	__super::Clean();
}

void RusherEnemy::CheckCollision(GameObject* otherGameObject)
{

}
