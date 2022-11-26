#include "BaseEnemy.h"
#include "World.h"
#include "Animation.h"
#include "TextureManager.h"
#include "World.h"

BaseEnemy::BaseEnemy(Properties* props) : Character(props)
{
	m_Animation = new Animation();
}

void BaseEnemy::Init()
{
	__super::Init();

	SetupBody();
}

void BaseEnemy::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}

void Move()
{

}

void BaseEnemy::Update(float deltaTime)
{
	__super::Update(deltaTime);

	SetOriginPoint();
	Move();
	m_Animation->Update(deltaTime);

}

void BaseEnemy::Clean()
{
	delete m_Animation;
	World::GetInstance()->Destroy(m_Body);
}

void BaseEnemy::CheckColision()
{

}

void BaseEnemy::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_kinematicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;
	_BodyDef.fixedRotation = true;

	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(m_Width / 2, m_Height / 2);

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.density = 100.0f;
	_fixtureDef.friction = 1.0f;

	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}

void BaseEnemy::SetAnimationState(EnemyState inCurrentAnimationState, float inAxisValue)
{
	
}

void BaseEnemy::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}


