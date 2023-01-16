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

void Player::Initialize(float moveDistance, BlockManager* blockM, PlayerSocket* playerSocket, Model* model, DebugText* debugText_)
{
	assert(model);

	model_ = model;
	this->debugText_ = debugText_;
	this->moveDistance = moveDistance;
	this->blockM = blockM;
	this->playerSocket = playerSocket;

	isPlayer = true;
	isDead = false;
	isGoal = false;

	{
		//���S�t���O
		isDead = false;

		velocity = { 0,0,0 };
	}

	//this->tutorial = tutorial;

	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.trans = { 0,moveDistance,0 };
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

	//�Փˑ���
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
	//�ړ��̏ꍇ(��]���͈ړ����Ȃ�)
	if ((KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) ||
		KeyboardInput::GetInstance().KeyPush(DIK_UPARROW) || KeyboardInput::GetInstance().KeyPush(DIK_DOWNARROW))
		&& player->isTurnNow == false)
	{
		player->blockM->UpdateOverlap();

		if (KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x - player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x + player->moveDistance , player->GetWorldPos().y, player->GetWorldPos().z };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_UPARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + player->moveDistance };
		}
		if (KeyboardInput::GetInstance().KeyPush(DIK_DOWNARROW))
		{
			player->moveEndPos = { player->GetWorldPos().x, player->GetWorldPos().y,player->GetWorldPos().z + -player->moveDistance };
		}

		//�i�񂾐�Ƀu���b�N
		if (player->blockM->GetPosIsBlock(player->moveEndPos))
		{
			//�t���O�A�X�s�[�h�Ȃǂ��Z�b�g
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

	//������
	player->SetWorldPos(LerpVec3(player->moveStartPos, player->moveEndPos, EaseOut(t)));

	//�S�[��������
	if (player->blockM->GetIsGoal(player->GetWorldPos()))
	{
		player->isGoal = true;
	}
	//�ړ����I�������X�e�[�g�߂�
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
	//�q��
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		//�{�^������������
		if (player->blockM->GetPosIsButton(player->GetWorldPos()))
		{
			//����o�^
			player->blockM->RegistAxisButton(player->GetWorldPos());

			//�R���Z���g������
			player->playerSocket->UseSocket(player->GetWorldPos());

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
	player->blockM->UpdateConnect(player->GetWorldPos());

	if (KeyboardInput::GetInstance().KeyReleaseTrigger(DIK_SPACE))
	{
		//�������Ƃ��낪�{�^����������
		if(player->blockM->CheckAxisButton(player->GetWorldPos()))
		{
			player->isTurnNow = true;
			player->ChangeStateTurnConnect(new StateTurnP);
		}
		else
		{
			//�q����Ă���u���b�N��S����������X�e�[�W�֐�
			player->blockM->ReleseConectedBlock();
			//�R���Z���g�𔲂�
			player->playerSocket->FinishSocket(player->GetWorldPos());
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


	//��]�I���
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE) && !player->blockM->GetIsRollingLeftorRight())
	{
		player->isTurnNow = false;
		//�q����Ă���u���b�N��S����������X�e�[�W�֐�()
		player->blockM->ReleseConectedBlock();
		//�R���Z���g�𔲂�
		player->playerSocket->FinishSocket(player->GetWorldPos());
		player->ChangeStateTurnConnect(new StateNormalConTurP);
	}

}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


