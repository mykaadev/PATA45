#include "LonerEnemy.h"

LonerEnemy::LonerEnemy(Properties* props) : BaseEnemy(props)
{

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

	m_Animation->SetProperties("Loner", 1, 0, 16,50, true);

	m_Body->SetLinearVelocity(b2Vec2(0.f, 1.0f));

}

void LonerEnemy::Clean()
{
	__super::Clean();
}

void LonerEnemy::CheckColision()
{
	__super::CheckColision();
}
