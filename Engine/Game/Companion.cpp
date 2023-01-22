#include "Companion.h"
#include "World.h"
#include "Renderer.h"
#include "BaseEnemy.h"
#include "Player.h"
#include "RusherEnemy.h"

Companion::Companion(Properties* props) : Character(props) {

	m_Animation = new Animation();
	m_damageAmount = { 1 };

	currentHealth = 2;

}

void Companion::Init()
{
	if (m_Body == nullptr && !m_IsDead) { SetupBody(); }

	if (m_Body == nullptr && !m_IsDead) { return; }
	m_Animation->SetProperties("Companion", 1, 0, 16, 100, true);
}



void Companion::SetupBody()
{	
	if (!World::GetInstance()->GetWorld()->IsLocked() && !m_IsDead)
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

	SetOriginPoint();
	
	if (myPlayer->GetIsDead() && !m_IsDead)
	{
		m_IsDead = true;
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		SetSize(64, 64);
	}

	if (m_IsDead)
	{

		dynamic_cast<Player*>(myPlayer)->DetachCompanion(this);

		

		if (GetAnimation()->GetCurrentSprite() >= 10)
		{
			Clean();
		}
	}

	m_Animation->Update(deltaTime);
}


void Companion::Draw()
{
	if (m_Body == nullptr && !m_IsDead) { return; }
	m_Animation->Draw(m_Body->GetPosition().x, m_Body->GetPosition().y, m_Width, m_Height);
}


void Companion::SetOriginPoint()
{
	if (m_Body == nullptr && !m_IsDead) return;

	m_Origin->X = m_Body->GetPosition().x;
	m_Origin->Y = m_Body->GetPosition().y;
}


void Companion::SetPosition(b2Vec2 inPlayerPosition, float interpSpeed)
{
	if (m_Body != nullptr && !m_IsDead)
	{
		vDesiredPosition.x = m_Origin->X += (inPlayerPosition.x - m_Body->GetPosition().x) * interpSpeed;
		vDesiredPosition.y = m_Origin->Y += (inPlayerPosition.y - m_Body->GetPosition().y) * interpSpeed;
		m_Body->SetTransform(vDesiredPosition, m_Body->GetAngle());
	}

}

void Companion::Clean()
{
	m_PendingKill = true;
	
}

void Companion::CheckCollision(GameObject* otherGameObject)
{

}


void Companion::SetPlayer(Player* inMyPlayer)
{
	myPlayer = inMyPlayer;
}

void Companion::TakeDamage(int inDamage)
{
	currentHealth -= inDamage;

	if (currentHealth <= 0 && !m_IsDead)
	{
		currentHealth = 0;

		m_Animation->SetCurrentSprite(0);
		m_Animation->SetProperties("ExplosionMob", 1, 0, 11, 150, false);
		SetSize(64, 64);
		m_IsDead = true;
	}
}

Companion::~Companion()
{
	delete m_Animation;
}

