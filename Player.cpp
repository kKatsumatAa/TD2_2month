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
	ConnectingEffect2Manager* connectE2M, Tutorial* tutorial, CameraManager* cameraM, Model* model, DebugText* debugText_)
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

	isPlayer = true;
	isDead = false;
	isGoal = false;

	{
		//���S�t���O
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
		bufferedPushSpace = false;
		bufferedKeyArrow = BUFFERED_INPUT_ARROW::NONE;
	}

	//�Փˑ���
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
		bufferedPushSpace = false;
		bufferedKeyArrow = BUFFERED_INPUT_ARROW::NONE;
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

	//��s����
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		bufferedPushSpace = true;
	}
	//�V�ړ�
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

Player& Player::operator=(const Player& obj)
{
	this->model_ = obj.model_;
	//this->textureHandle = obj.textureHandle;
	this->stateMove  = obj.stateMove;
	this->stateConnectTurn  = obj.stateConnectTurn;
	//this->scaleTmp = obj.scaleTmp;
	this->posYTmp  = obj.posYTmp;
	this->posXTmp = obj.posXTmp;
	this->isMove  = obj.isMove;
	this->isWantToMove  = obj.isWantToMove;
	this->isMoveNow  = obj.isMoveNow;
	this->isConnect  = obj.isConnect;
	this->isWantToConnect  = obj.isWantToConnect;
	this->isCennectNow  = obj.isCennectNow;
	this->isTurn  = obj.isTurn;
	this->isisWantToTurn  = obj.isisWantToTurn;
	this->isTurnNow  = obj.isTurnNow;
	this->isGoal  = obj.isGoal;
	this->moveDistance = obj.moveDistance;
	this->moveEndPos = obj.moveEndPos;
	this->moveStartPos = obj.moveStartPos;
	this->bufferedPushSpace  = obj.bufferedPushSpace;
	this->bufferedKeyArrow  = obj.bufferedKeyArrow;
	//this->draw = obj.draw;
	this->debugText_  = obj.debugText_;
	this->blockM = obj.blockM;
	this->playerSocket = obj.playerSocket;
	this->connectE2M = obj.connectE2M;
	this->tutorial = obj.tutorial;
	this->cameraM = obj.cameraM;

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

	//�ړ��̏ꍇ(��]���͈ړ����Ȃ�)
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

		//�i�񂾐�Ƀu���b�N
		if (player->blockM->GetPosIsBlock(player->moveEndPos))
		{
			//��s
			player->bufferedKeyArrow = NONE;

			player->isMove = true;

			//�t���O�A�X�s�[�h�Ȃǂ��Z�b�g
			player->SetVelocity((player->moveEndPos - player->GetWorldPos()).GetNormalized());
			player->moveStartPos = (player->GetWorldPos());

			//���o
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 1.2f,scale.y * 0.6f,scale.z * 1.2f };

			//�`���[�g���A��
			if (player->tutorial->GetState() == TUTORIAL::MOVE)
			{
				player->tutorial->AddStateNum();
			}

			player->ChangeStateMove(new StateMoveP);
		}
		//����������
		else if (!shake.GetIsShaking() && shake.GetShake() == 0 && effectCount <= 0)
		{
			//��s
			player->bufferedKeyArrow = NONE;

			//���o
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

	//������
	player->SetWorldPos(LerpVec3(player->moveStartPos, player->moveEndPos, EaseOut(t)));

	//�S�[��������
	if (!player->isGoal && player->blockM->GetIsGoal(player->GetWorldPos(), true))
	{
		player->isGoal = true;
	}
	//�ړ����I�������X�e�[�g�߂�
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
	//�q��
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) || player->bufferedPushSpace) && !player->isMove)
	{
		//�{�^������������
		if (player->blockM->GetPosIsGear(player->GetWorldPos()) /*&& !player->isMove*/)
		{
			//�V�F�C�N��߂�
			Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
			trans = { trans.x ,trans.y,trans.z };
			player->GetWorldTransForm()->trans = { player->posXTmp ,player->posYTmp,trans.z };

			//��s����
			player->bufferedPushSpace = false;
			player->isConnect = true;

			//����o�^
			player->blockM->RegistAxisGear(player->GetWorldPos());

			//�R���Z���g������
			player->playerSocket->UseSocket(player->GetWorldPos());

			//���o
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 2.0f,scale.y * 0.2f,scale.z * 2.0f };

			//�G�t�F�N�g
			Vec3 startScale = { player->GetRadius() * 2.0f,player->GetRadius() * 2.0f,player->GetRadius() * 2.0f };
			Vec3 endScale = { player->GetRadius() * 0.1f,player->GetRadius() * 100.0f,player->GetRadius() * 0.1f };
			player->connectE2M->GenerateConnectingEffect2(player->GetWorldPos(), startScale, endScale
				, { 1.0f,1.0f,0,0.9f }, { 1.0f,1.0f,1.0f,0.3f }, 40, { 0,pi * 10.0f,0 });

			//�J�����؂�ւ�
			{
				player->cameraM->BegineLerpUsingCamera(player->cameraM->usingCamera->GetEye(),
					{ 0 + player->GetWorldPos().x,player->cameraM->gameMainCamera->GetEye().y + 35.0f,0 + player->GetWorldPos().z },
					player->cameraM->usingCamera->GetTarget(),
					{ 0 + player->GetWorldPos().x,0,0 + player->GetWorldPos().z },
					player->cameraM->usingCamera->GetUp(),
					{ 0,0,1 },
					60);

				/*player->cameraM->gameTurnCamera->SetEye({ 0 + player->GetWorldPos().x
					,player->cameraM->gameMainCamera->GetEye().y + 35.0f,
					0 +player->GetWorldPos().z });

				player->cameraM->gameTurnCamera->SetTarget({ 0 + player->GetWorldPos().x,
					0,
					0 +player->GetWorldPos().z });
				player->cameraM->gameTurnCamera->SetUp({ 0,0,1 });
				player->cameraM->gameTurnCamera->Update();*/

				player->cameraM->usingCamera = player->cameraM->gameTurnCamera.get();
			}

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
		//�������Ƃ��낪�{�^����������
		if (player->blockM->CheckAxisGear(player->GetWorldPos()))
		{
			//�V�F�C�N��߂�
			Vec3 trans = { player->GetWorldPos().x,player->GetWorldPos().y,player->GetWorldPos().z };
			trans = { trans.x ,trans.y,trans.z };
			player->GetWorldTransForm()->trans = { player->posXTmp ,player->posYTmp,trans.z };

			//��s����
			player->bufferedPushSpace = false;

			player->isTurnNow = true;

			//���o
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 2.0f,scale.y * 0.2f,scale.z * 2.0f };

			//�G�t�F�N�g
			Vec3 startScale = { player->GetRadius() * 2.0f,player->GetRadius() * 2.0f,player->GetRadius() * 2.0f };
			Vec3 endScale = { player->GetRadius() * 0.1f,player->GetRadius() * 100.0f,player->GetRadius() * 0.1f };
			player->connectE2M->GenerateConnectingEffect2(player->GetWorldPos(), startScale, endScale
				, { 0.2f,0.1f,1.0f,0.9f }, { 1.0f,1.0f,1.0f,0.3f }, 40, { 0,pi * 10.0f,0 });

			//�`���[�g���A��
			if (player->tutorial->GetState() == TUTORIAL::CONNECT)
			{
				player->tutorial->AddStateNum();
			}

			player->ChangeStateTurnConnect(new StateTurnP);
		}
		else
		{
			//�q����Ă���u���b�N��S����������X�e�[�W�֐�
			player->blockM->ReleseConectedBlock();
			//�R���Z���g�𔲂�
			player->playerSocket->FinishSocket(player->GetWorldPos());

			player->isConnect = false;

			player->bufferedPushSpace = false;

			//���o
			Vec3 scale = player->GetWorldTransForm()->scale;
			player->GetWorldTransForm()->scale = { scale.x * 0.1f,scale.y * 2.0f,scale.z * 0.1f };

			//�J�����؂�ւ�
			{
				player->cameraM->BegineLerpUsingCamera(
					player->cameraM->gameTurnCamera->GetEye(),
					player->cameraM->gameMainCamera->GetEye(),
					player->cameraM->gameTurnCamera->GetTarget(),
					player->cameraM->gameMainCamera->GetTarget(),
					player->cameraM->gameTurnCamera->GetUp(),
					player->cameraM->gameMainCamera->GetUp(),
					60);
			}

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
	//��]����֐�
	player->blockM->UpdateRotate(player->GetWorldPos());

	player->posXTmp = player->GetWorldPos().x;

	//���o
	Vec3 scale = { player->GetRadius(),player->GetRadius(), player->GetRadius() };
	player->GetWorldTransForm()->scale = { scale.x ,scale.y * 1.3f,scale.z };

	//��]�I���
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) && !player->blockM->GetIsRollingLeftorRight())
	{
		//��s
		player->bufferedKeyArrow = NONE;

		player->isTurnNow = false;
		//�q����Ă���u���b�N��S����������X�e�[�W�֐�()
		player->blockM->ReleseConectedBlock();
		//�R���Z���g�𔲂�
		player->playerSocket->FinishSocket(player->GetWorldPos());

		player->isConnect = false;

		player->bufferedPushSpace = false;

		//���o
		Vec3 scale = player->GetWorldTransForm()->scale;
		player->GetWorldTransForm()->scale = { scale.x * 0.1f,scale.y * 2.0f,scale.z * 0.1f };

		//�J�����؂�ւ�
		{
			player->cameraM->BegineLerpUsingCamera(
				player->cameraM->gameTurnCamera->GetEye(),
				player->cameraM->gameMainCamera->GetEye(),
				player->cameraM->gameTurnCamera->GetTarget(),
				player->cameraM->gameMainCamera->GetTarget(),
				player->cameraM->gameTurnCamera->GetUp(),
				player->cameraM->gameMainCamera->GetUp(),
				60);

		}

		player->ChangeStateTurnConnect(new StateNormalConTurP);
	}

}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


