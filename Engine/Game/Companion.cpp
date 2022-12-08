#include "Companion.h"
#include "World.h"
#include "TextureManager.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "RusherEnemy.h"

Companion::Companion(Properties* props) : Character(props) {

	m_Animation = new Animation();
	m_damageAmount = { 1 };

}

void Companion::Init()
{
	if (m_Body == nullptr) { SetupBody(); }

	if (m_Body == nullptr) { return; }
	m_Animation->SetProperties("Companion", 1, 0, 16, 100, true);
}



void Companion::SetupBody()
{	
	if (!World::GetInstance()->GetWorld()->IsLocked())
	{
		b2BodyDef _BodyDef;
		_BodyDef.type = b2_dynamicBody;
		_BodyDef.position.Set(m_Transform->X, m_Transform->Y);
		_BodyDef.gravityScale = 0.0f;
		_BodyDef.fixedRotation = true;
		_BodyDef.bullet = true;
		m_Body = World::GetInstance()->GetWorld()->CreateBody(&_BodyDef);

		b2PolygonShape _boxShape;
		_boxShape.SetAsBox(m_Width / 2, m_Height / 2);

		b2FixtureDef _fixtureDef;
		_fixtureDef.shape = &_boxShape;
		_fixtureDef.isSensor = true;
		_fixtureDef.userData.pointer = (uintptr_t)this;


		b2Fixture* _Fixture;
		_Fixture = m_Body->CreateFixture(&_fixtureDef);
	}
}




void Companion::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animation->Update(deltaTime);

	SetOriginPoint();

}


void Companion::Draw()
{
	if (m_Body == nullptr) { return; }
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Companion::SetOriginPoint()
{
	if (m_Body == nullptr) return;

	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}


void Companion::SetPosition(b2Vec2 inPlayerPosition, float interpSpeed)
{
	vDesiredPosition.x = m_Origin->X += (inPlayerPosition.x - m_Body->GetPosition().x) * interpSpeed;
	vDesiredPosition.y = m_Origin->Y += (inPlayerPosition.y - m_Body->GetPosition().y) * interpSpeed;

	m_Body->SetTransform(vDesiredPosition, m_Body->GetAngle());
}

void Companion::Clean()
{
	m_PendingKill = true;
	
}

void Companion::CheckCollision(GameObject* otherGameObject)
{

}


Companion::~Companion()
{
	delete m_Animation;
}

