#include "Player.h"


void Player::ChangeStateTurnConnect(PlayerState* state)
{
	delete this->stateConnectTurn;
	this->stateConnectTurn = state;
	state->SetPlayer(this);
}

void Player::ChangeStateMove(PlayerState* state)
{
	delete this->stateMove;
	this->stateMove = state;
	state->SetPlayer(this);
}

void Player::Initialize(float moveDistance, Model* model, DebugText* debugText_)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	this->moveDistance = moveDistance;

	isPlayer = true;
	isDead = false;

	{
		//死亡フラグ
		isDead = false;

		velocity = { 0,0,0 };
	}

	//this->tutorial = tutorial;

	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();

	radius_ = scaleTmp;

	{
		isMove = false;
		isMoveNow = false;
		isConnect = false;
		isCennectNow = false;
		isTurn = false;
		isTurnNow = false;
		moveEndPos = { 0,0,0 };
	}

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);

	ChangeStateMove(new StateNormalMoveP);
	ChangeStateTurnConnect(new StateNormalConTurP);
}

void Player::Update()
{

	stateMove->Update();
	stateConnectTurn->Update();

	worldTransform_.SetWorld();
}

void Player::Draw(Camera* camera)
{

	//
	stateMove->Draw(camera, model_);
	stateConnectTurn->Draw(camera, model_);
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

//----------------------------------------------------------------------------------------------
void PlayerState::SetPlayer(Player* player)
{
	this->player = player;
}

//--------------------------------------------------------------------------
void StateNormalMoveP::Update()
{
	//移動の場合(回転中は移動しない)
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW) ||
		KeyboardInput::GetInstance().KeyTrigger(DIK_UPARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_DOWNARROW))
		&& player->isTurnNow == false)
	{
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x - player->moveDistance ,0,0 };
		}
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x + player->moveDistance ,0,0 };
		}
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_UPARROW))
		{
			player->moveEndPos = { 0,player->moveDistance,0 };
		}
		if (KeyboardInput::GetInstance().KeyTrigger(DIK_DOWNARROW))
		{
			player->moveEndPos = { 0,-player->moveDistance ,0 };
		}

		//if (/*ステージの関数で先にブロックあるか判定(endPosを引数)*/)
		{
			//フラグ、スピードなどをセット
			player->SetVelocity((player->moveEndPos - player->GetWorldPos()).GetNormalized());
			player->moveStartPos = (player->GetWorldPos());
			player->ChangeStateMove(new StateMoveP);
		}
	}
}

void StateNormalMoveP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateMoveP::Update()
{
	count++;

	float t = (float)count / (float)countMax;

	//動かす
	player->SetWorldPos(LerpVec3(player->moveStartPos, player->moveEndPos, EaseOut(t)));

	//移動し終わったらステート戻す
	if (count >= countMax)
	{
		player->ChangeStateMove(new StateNormalMoveP);
	}
}

void StateMoveP::Draw(Camera* camera, Model* model)
{
}


//--------------------------------------------------------------------------
void StateNormalConTurP::Update()
{
	//繋ぐ
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		//if (/*ステージの関数で判定(player->GetWorldPos()に、ボタンがあるか)*/)
		{
			player->ChangeStateTurnConnect(new StateConnectP);
		}
	}
}

void StateNormalConTurP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateConnectP::Update()
{
	/*ステージの関数でつなぐ更新関数(player->GetWorldPos())*/

	if (KeyboardInput::GetInstance().KeyReleaseTrigger(DIK_SPACE))
	{
		//if(/*ステージ関数 /離したところが違うボタンだったら(player->GetWorldPos())*/)
		{
			player->isTurnNow = true;
			player->ChangeStateTurnConnect(new StateTurnP);
		}
		//else
		{

			player->ChangeStateTurnConnect(new StateNormalConTurP);
		}
	}
}

void StateConnectP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateTurnP::Update()
{
	/*ステージ関数/ キーボードによって回転(player->getWorldPos())*/
	//キーボードを関数内で取得

	//回転終わる
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		player->isTurnNow = false;
		player->ChangeStateTurnConnect(new StateNormalConTurP);
	}
}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


