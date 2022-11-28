#include "LonerEnemy.h"
#include "World.h"

LonerEnemy::LonerEnemy(Properties* props) : BaseEnemy(props)
{
	m_MaxHealth = { 5 };

	m_CurrentHealth = m_MaxHealth;
}

void LonerEnemy::Init()
{
	__super::Init();

	//Handle Animations
	m_Animation->SetProperties("Loner", 1, 0, 16, 50, true);
}

void LonerEnemy::Draw()
{
	__super::Draw();
}

void LonerEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animation->Update(deltaTime);

	//Handle Movement
	m_Body->SetLinearVelocity(b2Vec2(0.f, 0.5f));

	//Handle Out of Screen Destroy
	if (m_Body->GetPosition().y > 700.0f && !m_IsDead)
	{
		m_IsDead = true;
	}

	if (m_IsDead)
	{
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 100, false);

		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}

}

void LonerEnemy::TakeDamage(int inDamage)
{
	m_CurrentHealth -= inDamage;

	if (m_CurrentHealth <= 0 && !m_IsDead)
	{
		m_IsDead = true;
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 100, false);
	}
}

void LonerEnemy::Clean()
{

	World::GetInstance()->DestroyGameObject(this, m_Body);
}

void LonerEnemy::CheckCollision(GameObject* otherGameObject)
{

}
