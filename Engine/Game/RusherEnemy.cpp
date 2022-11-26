#include "RusherEnemy.h"
#include "BaseEnemy.h"


RusherEnemy::RusherEnemy(Properties* props) : BaseEnemy(props)
{
	m_GoingRight = true;
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

	std::cout << m_Body->GetPosition().x << std::endl;

	m_Animation->SetProperties("Rusher", 1, 0, 24, 50, true);

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
}

void RusherEnemy::Clean()
{
	__super::Clean();
}

void RusherEnemy::CheckColision()
{
	__super::CheckColision();


}
