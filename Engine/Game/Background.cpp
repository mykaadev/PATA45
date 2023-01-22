#include "Background.h"
#include "World.h"

Background::Background(Properties* props) : Character(props) 
{
	m_Animation = new Animation();
}

void Background::Init()
{
	SetupBody();
	m_DefaultPos = m_Body->GetPosition();
	
	if (m_Body != nullptr)
	{
		m_Body->SetLinearVelocity(b2Vec2(0.0f, 0.2f));
	}

	m_Animation->SetProperties("Galaxy", 1, 0, 1, 100, false);

}

void Background::Draw()
{
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}

void Background::Update(float deltaTime)
{
	SetOriginPoint();

	m_Animation->Update(deltaTime);

	if (m_Body->GetPosition().y >= m_DefaultPos.y + m_Height)
	{
		m_Body->SetTransform(b2Vec2(m_DefaultPos.x, m_DefaultPos.y + 1), m_Body->GetAngle());
	}
}

void Background::Clean()
{

}

void Background::CheckCollision(GameObject* otherGameObject)
{

}

void Background::SetOriginPoint()
{
	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}



void Background::SetupBody()
{
	b2BodyDef _BodyDef;
	_BodyDef.type = b2_dynamicBody;
	_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
	_BodyDef.gravityScale = 0.0f;
	_BodyDef.fixedRotation = true;
	_BodyDef.bullet = true;

	m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

	b2PolygonShape _boxShape;
	_boxShape.SetAsBox(0.1f, 0.1f );

	b2FixtureDef _fixtureDef;
	_fixtureDef.shape = &_boxShape;
	_fixtureDef.isSensor = true;



	b2Fixture* _Fixture;
	_Fixture = m_Body->CreateFixture(&_fixtureDef);
}