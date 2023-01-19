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

void Player::Initialize(float moveDistance, BlockManager* blockM, PlayerSocket* playerSocket,
	ConnectingEffect2Manager* connectE2M, Tutorial* tutorial, Model* model, DebugText* debugText_)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	this->moveDistance = moveDistance;
	this->blockM = blockM;
	this->playerSocket = playerSocket;
	this->connectE2M = connectE2M;
	this->tutorial = tutorial;

	isPlayer = true;
	isDead = false;
	isGoal = false;

	{
		//死亡フラグ
		isDead = false;

		velocity = { 0,0,0 };
	}

	//this->tutorial = tutorial;

	worldTransform_.scale = { scaleTmp,0,scaleTmp };
	worldTransform_.trans = { 0,moveDistance,0 };
	posYTmp = moveDistance;
	posXTmp = 0;
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

void Player::Reset()
{
	isPlayer = true;
	isDead = false;
	isGoal = false;

	velocity = { 0,0,0 };

	//this->tutorial = tutorial;

	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.trans = { 0,moveDistance,0 };
	posYTmp = moveDistance;
	posXTmp = 0;
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

	ChangeStateMove(new StateNormalMoveP);
	ChangeStateTurnConnect(new StateNormalConTurP);
}

void Player::Update()
{
	if (worldTransform_.scale.x > scaleTmp) { worldTransform_.scale.x -= 0.05f; }
	if (worldTransform_.scale.x < scaleTmp) { worldTransform_.scale.x += 0.05f; }

	if (worldTransform_.scale.y > scaleTmp) { worldTransform_.scale.y -= 0.05f; }
	if (worldTransform_.scale.y < scaleTmp) { worldTransform_.scale.y += 0.05f; }

	if (worldTransform_.scale.z > scaleTmp) { worldTransform_.scale.z -= 0.05f; }
	if (worldTransform_.scale.z < scaleTmp) { worldTransform_.scale.z += 0.05f; }

	{

	}

	//先行入力
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		bufferedPushSpace = true;
	}
	//〃移動
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW)) { bufferedKeyArrow = BUFFERED_INPUT_ARROW::LEFT; }
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW)) { bufferedKeyArrow = BUFFERED_INPUT_ARROW::RIGHT; }
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_UPARROW)) { bufferedKeyArrow = BUFFERED_INPUT_ARROW::UP; }
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_DOWNARROW)) { bufferedKeyArrow = BUFFERED_INPUT_ARROW::DOWN; }

	stateMove->Update();
	stateConnectTurn->Update();



	worldTransform_.SetWorld();


}

void Player::Draw(Camera* camera)
{
	draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &model_[0]);

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
	if (player->isTurnNow == false)
	{
		shake.Update();

		countE++;

		effectCount--;

		Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
		trans = { trans.x ,trans.y,trans.z };

		player->GetWorldTransForm()->trans = { player->posXTmp + shake.GetShake() ,player->posYTmp + sinf(countE * 0.07f) * 0.3f ,trans.z };
	}

	//移動の場合(回転中は移動しない)
	if (
		(
			(
				KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) ||
				KeyboardInput::GetInstance().KeyPush(DIK_UPARROW) || KeyboardInput::GetInstance().KeyPush(DIK_DOWNARROW)
				)
			||
			(
				KeyboardInput::GetInstance().KeyPush(DIK_A) || KeyboardInput::GetInstance().KeyPush(DIK_D) ||
				KeyboardInput::GetInstance().KeyPush(DIK_W) || KeyboardInput::GetInstance().KeyPush(DIK_S)
				)
			||
			(
				player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::LEFT || player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::RIGHT
				|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::UP || player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::DOWN
				)
			)
		&& player->isTurnNow == false && !shake.GetIsShaking())
	{
		if (KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_A)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::LEFT)
		{
			player->moveEndPos = { player->GetWorldPos().x - player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_D)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::RIGHT)
		{
			player->moveEndPos = { player->GetWorldPos().x + player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_UPARROW) || KeyboardInput::GetInstance().KeyPush(DIK_W)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::UP)
		{
			player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + player->moveDistance };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_DOWNARROW) || KeyboardInput::GetInstance().KeyPush(DIK_S)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::DOWN)
		{
			player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + -player->moveDistance };
		}

		//進んだ先にブロック
		if (player->blockM->GetPosIsBlock(player->moveEndPos))
		{
			//先行
			player->bufferedKeyArrow = NONE;

			player->isMove = true;

			//フラグ、スピードなどをセット
			player->SetVelocity((player->moveEndPos - player->GetWorldPos()).GetNormalized());
			player->moveStartPos = (player->GetWorldPos());

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 1.1f,scale.y * 0.8f,scale.z * 1.1f };

			//チュートリアル
			if (player->tutorial->GetState() == TUTORIAL::MOVE)
			{
				player->tutorial->AddStateNum();
			}

			player->ChangeStateMove(new StateMoveP);
		}
		//無かった時
		else if (!shake.GetIsShaking() && shake.GetShake() == 0 && effectCount <= 0)
		{
			//先行
			player->bufferedKeyArrow = NONE;

			//演出
			shake.SetShake(12, player->moveDistance / 4.0f);

			effectCount = effectCountTmp;
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

	//ゴールしたら
	if (!player->isGoal && player->blockM->GetIsGoal(player->GetWorldPos(), true))
	{
		player->isGoal = true;
	}
	//移動し終わったらステート戻す
	if (count >= countMax)
	{
		player->isMove = false;
		player->posXTmp = player->GetWorldPos().x;
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
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedPushSpace) && !player->isMove)
	{
		//ボタンがあったら
		if (player->blockM->GetPosIsButton(player->GetWorldPos()) /*&& !player->isMove*/)
		{
			//シェイクやめる
			Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
			trans = { trans.x ,trans.y,trans.z };
			player->GetWorldTransForm()->trans = { player->posXTmp ,player->posYTmp,trans.z };

			//先行解除
			player->bufferedPushSpace = false;
			player->isConnect = true;

			//軸を登録
			player->blockM->RegistAxisButton(player->GetWorldPos());

			//コンセントをさす
			player->playerSocket->UseSocket(player->GetWorldPos());

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 2.0f,scale.y * 0.2f,scale.z * 2.0f };

			//エフェクト
			Vec3 startScale = { player->GetRadius() * 2.0f,player->GetRadius() * 2.0f,player->GetRadius() * 2.0f };
			Vec3 endScale = { player->GetRadius() * 0.1f,player->GetRadius() * 100.0f,player->GetRadius() * 0.1f };
			player->connectE2M->GenerateConnectingEffect2(player->GetWorldPos(), startScale, endScale
				, { 1.0f,1.0f,0,0.9f }, { 1.0f,1.0f,1.0f,0.3f }, 40, { 0,pi * 10.0f,0 });

			player->ChangeStateTurnConnect(new StateConnectP);
		}
		else
		{
			player->bufferedPushSpace = false;
		}
	}
}

void StateNormalConTurP::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------
void StateConnectP::Update()
{
	player->blockM->UpdateConnect(player->GetWorldPos());

	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedPushSpace) && !player->isMove)
	{
		//押したところがボタンだったら
		if (player->blockM->CheckAxisButton(player->GetWorldPos()))
		{
			//シェイクやめる
			Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
			trans = { trans.x ,trans.y,trans.z };
			player->GetWorldTransForm()->trans = { player->posXTmp ,player->posYTmp,trans.z };

			//先行解除
			player->bufferedPushSpace = false;

			player->isTurnNow = true;

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 2.0f,scale.y * 0.2f,scale.z * 2.0f };

			//エフェクト
			Vec3 startScale = { player->GetRadius() * 2.0f,player->GetRadius() * 2.0f,player->GetRadius() * 2.0f };
			Vec3 endScale = { player->GetRadius() * 0.1f,player->GetRadius() * 100.0f,player->GetRadius() * 0.1f };
			player->connectE2M->GenerateConnectingEffect2(player->GetWorldPos(), startScale, endScale
				, { 0.2f,0.1f,1.0f,0.9f }, { 1.0f,1.0f,1.0f,0.3f }, 40, { 0,pi * 10.0f,0 });

			//チュートリアル
			if (player->tutorial->GetState() == TUTORIAL::CONNECT)
			{
				player->tutorial->AddStateNum();
			}

			player->ChangeStateTurnConnect(new StateTurnP);
		}
		else
		{
			//繋がれているブロックを全部解除するステージ関数
			player->blockM->ReleseConectedBlock();
			//コンセントを抜く
			player->playerSocket->FinishSocket(player->GetWorldPos());

			player->isConnect = false;

			player->bufferedPushSpace = false;

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 0.1f,scale.y * 2.0f,scale.z * 0.1f };

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
	//回転する関数
	player->blockM->UpdateRotate(player->GetWorldPos());

	player->posXTmp = player->GetWorldPos().x;

	//演出
	Vec3 scale = { player->GetRadius(),player->GetRadius(), player->GetRadius() };
	player->GetWorldTransForm()->scale = { scale.x ,scale.y * 1.3f,scale.z };

	//回転終わる
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) && !player->blockM->GetIsRollingLeftorRight())
	{
		//先行
		player->bufferedKeyArrow = NONE;

		player->isTurnNow = false;
		//繋がれているブロックを全部解除するステージ関数()
		player->blockM->ReleseConectedBlock();
		//コンセントを抜く
		player->playerSocket->FinishSocket(player->GetWorldPos());

		player->isConnect = false;

		player->bufferedPushSpace = false;

		//演出
		Vec3 scale = player->GetWorldTransForm()->scale;
		player->GetWorldTransForm()->scale = { scale.x * 0.1f,scale.y * 2.0f,scale.z * 0.1f };

		player->ChangeStateTurnConnect(new StateNormalConTurP);
	}

}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


