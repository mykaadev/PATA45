#include "LonerEnemy.h"
#include "World.h"

LonerEnemy::LonerEnemy(Properties* props) : BaseEnemy(props)
{
	m_MaxHealth = { 2 };

	m_CurrentHealth = m_MaxHealth;
}

void LonerEnemy::Init()
{
	__super::Init();
}

void LonerEnemy::Draw()
{
	__super::Draw();
}

void LonerEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);

	//Handle Animation
	m_Animation->SetProperties("Loner", 1, 0, 16,50, true);

	//Handle Movement
	m_Body->SetLinearVelocity(b2Vec2(0.f, 0.5f));

	//Handle Out of Screen Destroy
	if (m_Body->GetPosition().y > 700.0f)
	{
		World::GetInstance()->DestroyGameObject(this, m_Body);
	}
}

void LonerEnemy::TakeDamage(int inDamage)
{
	m_CurrentHealth -= inDamage;

	std::cout << "LONER " << m_CurrentHealth << std::endl;

	if (m_CurrentHealth <= 0)
	{
		std::cout << "LONER DEAD" << std::endl;

		World::GetInstance()->DestroyGameObject(this, m_Body);
	}
}

void LonerEnemy::Clean()
{
	__super::Clean();
}

void LonerEnemy::CheckCollision(GameObject* otherGameObject)
{

}
