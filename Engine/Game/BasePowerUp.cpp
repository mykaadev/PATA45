#include "BasePowerUp.h"
#include "World.h"
#include "Animation.h"
#include "Renderer.h"
#include "World.h"


BasePowerUp::BasePowerUp(Properties* props) : Character(props)
{
	m_Animation = new Animation;
}

void BasePowerUp::Init()
{
	__super::Init();
	SetupBody();
}

void BasePowerUp::Draw()
{
	if (m_Body != nullptr)
	{
		m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
	}
}

void BasePowerUp::Update(float deltaTime)
{
	__super::Update(deltaTime);
}

void BasePowerUp::Clean()
{
	delete m_Animation;
}


void BasePowerUp::SetupBody()
{
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		b2BodyDef _BodyDef;
		_BodyDef.type = b2_kinematicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;

		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2, m_Height / 2);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.density = 1.0f;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;

		b2Fixture* _Fixture;
		_Fixture = m_Body->CreateFixture(&_fixtureDef);
	}
}

void BasePowerUp::SetAnimationState(PowerupState inCurrentAnimationState, float inAxisValue){}

void BasePowerUp::SetOriginPoint(){}

void BasePowerUp::ChooseType(){}

void BasePowerUp::CheckColision() {}

void BasePowerUp::TakeDamage(int inDamage) {}