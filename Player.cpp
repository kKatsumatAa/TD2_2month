#include "Player.h"


void Player::ChangeState(PlayerState* state)
{
	delete this->state;
	this->state = state;
	state->SetPlayer(this);
}

void Player::Initialize(Model* model, DebugText* debugText_, Camera* camera)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	this->camera = camera;

	isPlayer = true;
	isDead = false;

	{
		//Ž€–Sƒtƒ‰ƒO
		isDead = false;

		velocity = { 0,0,0 };
	}

	//this->tutorial = tutorial;

	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();

	radius_ = scaleTmp;

	//Õ“Ë‘®«
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);

	ChangeState(new StateNormalP);
}

void Player::Update()
{

	//
	state->Update();

	worldTransform_.SetWorld();
}

void Player::Draw(Camera* camera)
{

	//
	state->Draw(camera, model_);
}

void Player::DrawSprite()
{
}

void Player::OnCollision(Collider& collider)
{
}

void Player::OnCollision2(Collider& collider)
{
}


//--------------------------------------------------------------------------------
void PlayerState::SetPlayer(Player* player)
{
}

//--------------------------------------------------------------------------
void StateNormalP::Update()
{
}

void StateNormalP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateConnectP::Update()
{
}

void StateConnectP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateTurnP::Update()
{
}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


