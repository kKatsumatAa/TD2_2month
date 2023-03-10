#include "Player.h"
#include "GetBackManager.h"
#include "ParticleManager.h"

Player::~Player()
{
	delete stateConnectTurn;
	delete stateMove;
	//delete conectLimit_;
}

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
	ConnectingEffect2Manager* connectE2M, Tutorial* tutorial, CameraManager* cameraM, Model* model, DebugText* debugText_,
	ConectLimit* conectLimit)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	this->moveDistance = moveDistance;
	this->blockM = blockM;
	this->playerSocket = playerSocket;
	this->connectE2M = connectE2M;
	this->tutorial = tutorial;
	this->cameraM = cameraM;

	this->conectLimit_ = conectLimit;
	//conectLimit_->ResetCounts();

	isPlayer = true;
	isDead = false;
	isGoal = false;

	{
		//死亡フラグ
		isDead = false;

		velocity = { 0,0,0 };
	}
	colorCount = 0;

	isLoadConectCount = true;

	//繋ぐ回数を設定
	conectCount_ = conectLimit_->GetConectcount();
	//繋ぐ回数制限を設定
	conectCountMax = conectLimit_->GetLimitCount();

	//スタートマスのフラグ
	isStartConect = true;
	//進んだカウントを記録する
	moveCount = 0;

	worldTransform_.scale = { scaleTmp,0,scaleTmp };
	//worldTransform_.trans = { 0,moveDistance,0 };
	posYTmp = moveDistance;
	posXTmp = 0;
	worldTransform_.SetWorld();

	radius_ = scaleTmp;

	color = { 1.0f,1.0f,1.0f,1.0f };

	{
		isMove = false;
		isMoveNow = false;
		isConnect = false;
		isCennectNow = false;
		isTurn = false;
		isTurnNow = false;
		moveEndPos = { 0,0,0 };
		bufferedPushSpace = false;
		bufferedKeyArrow = BUFFERED_INPUT_ARROW::NONE;
		bufferedTurnRelease = false;
	}

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			playerPos[i][j] = false;
		}
	}

	//プレイヤーの長さ制限
	playerLimitX = blockM->GetGameWidth();
	playerLimitZ = blockM->GetGameHeight();

	//衝突属性
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributeEnemy);

	ChangeStateMove(new StateNormalMoveP);
	ChangeStateTurnConnect(new StateNormalConTurP);
}

void Player::SetPosStage(bool playerPos[][13])
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			if (playerPos[i][j] == true)
			{
				this->playerPos[i][j] = true;
				worldTransform_.trans = { moveDistance * (float)j, moveDistance, this->moveDistance * (float)i };
				worldTransform_.SetWorld();
				posYTmp = worldTransform_.trans.y;
				posXTmp = worldTransform_.trans.x;
			}
			else
			{
				this->playerPos[i][j] = false;
			}
		}
	}
}

void Player::Reset()
{
	isPlayer = true;
	isDead = false;
	isGoal = false;
	isLoadConectCount = true;

	velocity = { 0,0,0 };
	color = { 1.0f,1.0f,1.0f,1.0f };

	//繋ぐ回数を設定
	conectCount_ = conectLimit_->GetConectcount();
	//繋ぐ回数制限を設定
	conectCountMax = conectLimit_->GetLimitCount();

	//スタートマスのフラグ
	isStartConect = true;
	//進んだカウントを記録する
	moveCount = 0;


	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	posYTmp = moveDistance;
	posXTmp = 0;
	worldTransform_.SetWorld();

	radius_ = scaleTmp;
	colorCount = 0;
	{
		isMove = false;
		isMoveNow = false;
		isConnect = false;
		isCennectNow = false;
		isTurn = false;
		isTurnNow = false;
		moveEndPos = { 0,0,0 };
		bufferedPushSpace = false;
		bufferedKeyArrow = BUFFERED_INPUT_ARROW::NONE;
		bufferedTurnRelease = false;
	}

	SetPosStage(this->playerPos);

	ChangeStateMove(new StateNormalMoveP);
	ChangeStateTurnConnect(new StateNormalConTurP);
}

void Player::Update()
{
	if (isLoadConectCount == true)
	{
		conectCount_ = conectLimit_->GetConectcount();
		conectCountMax = conectLimit_->GetLimitCount();
		isLoadConectCount = false;
	}
	else
	{
		//よくわからないことをしていたのでここでコネクトリミットの方も合わせる
		conectLimit_->SetDrawCount(conectCount_);
	}

	if (worldTransform_.scale.x > scaleTmp) { worldTransform_.scale.x -= 0.05f; }
	if (worldTransform_.scale.x < scaleTmp) { worldTransform_.scale.x += 0.05f; }

	if (worldTransform_.scale.y > scaleTmp) { worldTransform_.scale.y -= 0.05f; }
	if (worldTransform_.scale.y < scaleTmp) { worldTransform_.scale.y += 0.05f; }

	if (worldTransform_.scale.z > scaleTmp) { worldTransform_.scale.z -= 0.05f; }
	if (worldTransform_.scale.z < scaleTmp) { worldTransform_.scale.z += 0.05f; }



	//先行入力
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		bufferedPushSpace = true;

		if (isTurnNow)
		{
			bufferedTurnRelease = true;
		}
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
	//移動失敗
	if (stateMove->GetShake())
	{
		color = { 1.0f,0.1f,0.2f,1.0f };
	}
	//残量ないとき
	else if (this->conectCount_ <= 0)
	{
		colorCount++;

		float addColor = sinf(colorCount * 0.05f) * 0.3f;
		color = { 0.5f + addColor,0.5f + addColor,0.5f + addColor,1.0f };

		if (colorCount % 60 < 20 && colorCount % 5 == 0)
		{
			color = { color.x - 0.4f,color.y - 0.4f,color.z + 0.2f,1.0f };
		}
		if (colorCount % 180 == 5)
		{
			Sound::GetInstance().PlayWave("tutu4.wav", 0.25f);
		}
	}
	else
	{
		Sound::GetInstance().StopWave("tutu4.wav");
		color = { 1.0f,1.0f,1.0f,1.0f };
	}

	draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &model_[0], color);

	//
	stateMove->Draw(camera, model_);
	stateConnectTurn->Draw(camera, model_);
}

void Player::DrawSprite()
{
}



bool Player::PlayerOutArea()
{
	//ブロックの直径
	int BlockDia = blockM->blockRadius_ * 2;
	//ずれているX分の長さ
	float outOfPositionXLeft = (blockM->outOfBlockNum) * BlockDia;
	float outOfPositionXRight = (blockM->gameAreaWidth - blockM->outOfBlockNum + 6) * BlockDia;
	if (GetWorldPos().x <= outOfPositionXRight && GetWorldPos().x >= -outOfPositionXLeft && GetWorldPos().z <= playerLimitZ && GetWorldPos().z > -playerLimitZ - BlockDia)
	{
		return false;
	}
	else
	{
		return true;;
	}
}

void Player::OnCollision(Collider& collider)
{
}

Player& Player::operator=(const Player& obj)
{
	this->Initialize(obj.moveDistance, obj.blockM, obj.playerSocket, obj.connectE2M, obj.tutorial, obj.cameraM, obj.model_, obj.debugText_, obj.conectLimit_);

	//*this->model_ = *obj.model_;
	//*this->textureHandle = *obj.textureHandle;
	*this->stateMove = *obj.stateMove;
	*this->stateConnectTurn = *obj.stateConnectTurn;//deleteされちゃうものはポインタの中身だけ
	this->posYTmp = obj.posYTmp;
	this->posXTmp = obj.posXTmp;
	this->conectCount_ = obj.conectCount_;
	this->conectCountMax = obj.conectCountMax;
	this->isMove = obj.isMove;
	this->isWantToMove = obj.isWantToMove;
	this->isMoveNow = obj.isMoveNow;
	this->isConnect = obj.isConnect;
	this->isWantToConnect = obj.isWantToConnect;
	this->isCennectNow = obj.isCennectNow;
	this->isTurn = obj.isTurn;
	this->isisWantToTurn = obj.isisWantToTurn;
	this->isTurnNow = obj.isTurnNow;
	this->isGoal = obj.isGoal;
	this->moveDistance = obj.moveDistance;
	this->moveEndPos = obj.moveEndPos;
	this->moveStartPos = obj.moveStartPos;
	//先行入力はおかしくなるので保存しない
	//this->bufferedPushSpace = obj.bufferedPushSpace;
	//this->bufferedKeyArrow = obj.bufferedKeyArrow;
	//this->draw = obj.draw;
	//*this->debugText_ = *obj.debugText_;
	this->blockM = obj.blockM;
	this->playerSocket = obj.playerSocket;
	//this->connectE2M = obj.connectE2M;
	this->tutorial = obj.tutorial;
	this->cameraM = obj.cameraM;
	this->worldTransform_ = obj.worldTransform_;
	this->worldTransform_.SetWorld();
	this->velocity = obj.velocity;
	//*this->conectLimit_ = *obj.conectLimit_;
	this->moveCount = obj.moveCount;
	this->color = obj.color;

	isStartConect = obj.isStartConect;
	isLoadConectCount = obj.isLoadConectCount;

	colorCount = obj.colorCount;

	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			playerPos[i][j] = obj.playerPos[i][j];
		}
	}

	return *this;
}

void Player::OnCollision2(Collider& collider)
{
}

//----------------------------------------------------------------------------------------------
void PlayerState::SetPlayer(Player* player)
{
	this->player = player;
}

void PlayerState::FailedEffect()
{
	player->GetWorldTransForm()->scale = { player->GetRadius() * 0 ,player->GetRadius() * 0 ,player->GetRadius() * 0 };

	player->bufferedTurnRelease = false;

	player->cameraM->usingCamera->CameraShake(15, 1.3f);

	for (int i = 0; i < 5; i++)
	{
		ParticleManager::GetInstance()->GenerateRandomParticle(7, 60, 0.2f, { player->GetWorldPos().x,player->GetWorldPos().y + player->GetRadius() * 1.3f,player->GetWorldPos().z }
		, 5.0f, 0.1f, { 1.0f,1.0f,1.0f,0.2f }, { 0,0,0,0.1f });

		ParticleManager::GetInstance()->GenerateRandomParticle(5, 90, 0.45f, { player->GetWorldPos().x,player->GetWorldPos().y + player->GetRadius() * 1.3f,player->GetWorldPos().z },
			0.45f, 0.1f, { 1.0f,1.0f,0,1.0f }, { 0,0,0,1.0f });
	}

	//音
	Sound::GetInstance().PlayWave("limitFailed.wav", 0.4f);
}

//--------------------------------------------------------------------------
void StateNormalMoveP::Update()
{
	if (player->isTurnNow == false)
	{
		shake.Update();

		countE++;

		effectCount--;
		effectCount2--;

		Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
		trans = { trans.x ,trans.y,trans.z };

		player->GetWorldTransForm()->trans = { player->posXTmp + shake.GetShake() ,player->posYTmp + sinf(countE * 0.07f) * 0.3f ,trans.z };
	}
	else
	{
		shake.Initialize();
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
			if (player->isConnect == true)
			{
				if (player->conectCount_ > 0)
				{
					player->moveEndPos = { player->GetWorldPos().x - player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
				}
				else if (KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_A) || effectCount2 <= 0)
				{
					effectCount2 = effectCountTmp;
					FailedEffect();

					//演出
					player->conectLimit_->BeginNumEffect(20, 1.8f, { 1.0f,0.1f,0.2f,1.0f });
				}
			}
			else
			{
				player->moveEndPos = { player->GetWorldPos().x - player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
			}

		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_D)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::RIGHT)
		{
			if (player->isConnect == true)
			{
				if (player->conectCount_ > 0)
				{
					player->moveEndPos = { player->GetWorldPos().x + player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
				}
				else if (KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_D) || effectCount2 <= 0)
				{
					effectCount2 = effectCountTmp;
					FailedEffect();

					//演出
					player->conectLimit_->BeginNumEffect(20, 1.8f, { 1.0f,0.1f,0.2f,1.0f });
				}
			}
			else
			{
				player->moveEndPos = { player->GetWorldPos().x + player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
			}
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_UPARROW) || KeyboardInput::GetInstance().KeyPush(DIK_W)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::UP)
		{
			if (player->isConnect == true)
			{
				if (player->conectCount_ > 0)
				{
					player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + player->moveDistance };
				}
				else if (KeyboardInput::GetInstance().KeyTrigger(DIK_UPARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_W) || effectCount2 <= 0)
				{
					effectCount2 = effectCountTmp;
					FailedEffect();

					//演出
					player->conectLimit_->BeginNumEffect(20, 1.8f, { 1.0f,0.1f,0.2f,1.0f });
				}
			}
			else
			{
				player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + player->moveDistance };
			}

		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_DOWNARROW) || KeyboardInput::GetInstance().KeyPush(DIK_S)
			|| player->bufferedKeyArrow == BUFFERED_INPUT_ARROW::DOWN)
		{
			if (player->isConnect == true)
			{
				if (player->conectCount_ > 0)
				{
					player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + -player->moveDistance };
				}
				else if (KeyboardInput::GetInstance().KeyTrigger(DIK_DOWNARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_S) || effectCount2 <= 0)
				{
					effectCount2 = effectCountTmp;
					FailedEffect();

					//演出
					player->conectLimit_->BeginNumEffect(20, 1.8f, { 1.0f,0.1f,0.2f,1.0f });
				}
			}
			else
			{
				player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + -player->moveDistance };
			}
		}

		//進んだ先にブロック
		if (player->blockM->GetPosIsBlock(player->moveEndPos))
		{
			//一手戻る機能に記録
			GetBackManager::GetInstance()->SaveDatas();

			//先行
			player->bufferedKeyArrow = NONE;

			player->isMove = true;

			//フラグ、スピードなどをセット
			player->SetVelocity((player->moveEndPos - player->GetWorldPos()).GetNormalized());
			player->moveStartPos = (player->GetWorldPos());

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 1.2f,scale.y * 0.6f,scale.z * 1.2f };

			ParticleManager::GetInstance()->GenerateRandomParticle(15, 25, 0.2f, { player->GetWorldPos().x,player->GetWorldPos().y - player->GetRadius() * 1.5f,player->GetWorldPos().z }
			, 2.0f, 0.1f, { 1.0f,1.0f,1.0f,0.2f }, { 0,0,0,0.1f });

			//数字
			if (player->conectCount_ > 0 && player->isConnect)
			{
				//演出
				player->conectLimit_->BeginNumEffect(15, 1.8f, { 1.0f,1.0f,0.2f,1.0f });
			}

			//チュートリアル
			if (player->tutorial->GetState() == TUTORIAL::MOVE)
			{
				player->tutorial->AddStateNum();
			}

			//音
			Sound::GetInstance().PlayWave("move (2).wav", 0.5f);


			player->ChangeStateMove(new StateMoveP);
		}
		//無かった時
		else if (!shake.GetIsShaking() && shake.GetShake() == 0 && effectCount <= 0 ||
			KeyboardInput::GetInstance().KeyTrigger(DIK_DOWNARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_S) ||
			KeyboardInput::GetInstance().KeyTrigger(DIK_UPARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_W) ||
			KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_A) ||
			KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_D))
		{
			//先行
			player->bufferedKeyArrow = NONE;

			//演出
			shake.SetShake(12, player->moveDistance / 4.0f);

			effectCount = effectCountTmp;

			//音
			Sound::GetInstance().PlayWave("moveFailed (2).wav", 0.5f);
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
		if (player->isConnect == true)
		{
			player->conectCount_ -= 1;
			player->moveCount += 1;

			//チュートリアル
			if (player->tutorial->GetState() == TUTORIAL::CONNECT_LIMIT && player->tutorial->GetStateNum() == 0 && player->conectCount_ <= 0)
			{
				player->tutorial->AddStateNum();
				player->tutorial->spriteCount = 0;
			}
		}
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
	if (player->isConnect)
	{
		player->ChangeStateTurnConnect(new StateConnectP);
	}

	//繋ぐ
	else if (((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedPushSpace) && !player->isMove))
	{
		//ボタンがあったら
		if (player->blockM->GetPosIsGear(player->GetWorldPos()) /*&& !player->isMove*/)
		{
			if (player->conectCount_ > 0)
			{
				//一手戻る機能に記録
				GetBackManager::GetInstance()->SaveDatas();

				//シェイクやめる
				Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
				trans = { trans.x ,trans.y,trans.z };
				player->GetWorldTransForm()->trans = { player->posXTmp ,player->posYTmp,trans.z };

				//先行解除
				player->bufferedPushSpace = false;
				player->isConnect = true;

				//軸を登録
				player->blockM->RegistAxisGear(player->GetWorldPos());

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

				//カメラ切り替え
				{
					player->cameraM->usingCamera = player->cameraM->gameTurnCamera.get();

					player->cameraM->BegineLerpUsingCamera(player->cameraM->gameMainCamera->GetEye(),
						{ 0 + player->GetWorldPos().x,player->cameraM->gameMainCamera->GetEye().y + 35.0f,0 + player->GetWorldPos().z },
						player->cameraM->gameMainCamera->GetTarget(),
						{ 0 + player->GetWorldPos().x,0,0 + player->GetWorldPos().z },
						player->cameraM->gameMainCamera->GetUp(),
						{ 0,0,1 },
						60);
				}

				//音
				Sound::GetInstance().PlayWave("connectBegine.wav", 0.7f);


				player->ChangeStateTurnConnect(new StateConnectP);
			}
			//残量がなかったら
			else
			{
				player->bufferedPushSpace = false;
				FailedEffect();

				//演出
				player->conectLimit_->BeginNumEffect(20, 1.8f, { 1.0f,0.1f,0.2f,1.0f });
			}
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

	if (player->isTurnNow)
	{
		player->ChangeStateTurnConnect(new StateTurnP);
	}

	else if ((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedPushSpace) && !player->isMove)
	{
		//押したところがボタンだったら
		if (player->blockM->CheckAxisGear(player->GetWorldPos()))
		{
			//進んだマス分カウントリセット
			player->moveCount = 0;

			//スタートマスのフラグをオンに
			player->isStartConect = true;

			//一手戻る機能に記録
			GetBackManager::GetInstance()->SaveDatas();

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

			//音
			Sound::GetInstance().PlayWave("connectEnd.wav", 0.7f);

			player->ChangeStateTurnConnect(new StateTurnP);
		}
		else
		{
			//スタートマスのフラグをオンに
			player->isStartConect = true;
			//進んだマス分カウント戻す
			player->conectCount_ += player->moveCount;
			player->moveCount = 0;

			//一手戻る機能に記録
			GetBackManager::GetInstance()->SaveDatas();

			//繋がれているブロックを全部解除するステージ関数
			player->blockM->ReleseConectedBlock();
			//コンセントを抜く
			player->playerSocket->FinishSocket(player->GetWorldPos());

			player->isConnect = false;

			player->bufferedPushSpace = false;

			//演出
			player->conectLimit_->BeginNumEffect(30, 2.8f, { 0.3f,0.9f,0.9f,1.0f });

			//演出
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 0.1f,scale.y * 2.0f,scale.z * 0.1f };

			//カメラ切り替え
			{
				player->cameraM->usingCamera = player->cameraM->gameTurnCamera.get();

				player->cameraM->BegineLerpUsingCamera(
					player->cameraM->gameTurnCamera->GetEye(),
					player->cameraM->gameMainCamera->GetEye(),
					player->cameraM->gameTurnCamera->GetTarget(),
					player->cameraM->gameMainCamera->GetTarget(),
					player->cameraM->gameTurnCamera->GetUp(),
					player->cameraM->gameMainCamera->GetUp(),
					60);
			}

			//音
			Sound::GetInstance().PlayWave("connectCancel.wav", 0.35f);

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
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedTurnRelease) && !player->blockM->GetIsRollingLeftorRight() && player->blockM->GetCheckElec())
	{
		if (player->blockM->GetisLockedBlock(player->GetWorldPos()) == false)
		{

			//一手戻る機能に記録
			GetBackManager::GetInstance()->SaveDatas();

			//先行
			player->bufferedKeyArrow = NONE;
			player->bufferedTurnRelease = false;

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

			//カメラ切り替え
			{
				player->cameraM->usingCamera = player->cameraM->gameTurnCamera.get();

				player->cameraM->BegineLerpUsingCamera(
					player->cameraM->gameTurnCamera->GetEye(),
					player->cameraM->gameMainCamera->GetEye(),
					player->cameraM->gameTurnCamera->GetTarget(),
					player->cameraM->gameMainCamera->GetTarget(),
					player->cameraM->gameTurnCamera->GetUp(),
					player->cameraM->gameMainCamera->GetUp(),
					60);
			}

			player->blockM->SetCheckElec();

			//予測線消す
			player->blockM->predictBlockM->ClearPredictBlock();

			player->ChangeStateTurnConnect(new StateNormalConTurP);
		}
		//解除できないときの演出
		else if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedTurnRelease)
		{
			FailedEffect();
			player->bufferedTurnRelease = false;
		}
		else
		{
			player->bufferedTurnRelease = false;
		}

	}

}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


