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
		//���S�t���O
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

		//if (/*�X�e�[�W�̊֐��Ő�Ƀu���b�N���邩����(endPos������)*/)
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
		//if (/*�X�e�[�W�̊֐��Ŕ���(player->GetWorldPos()�ɁA�{�^�������邩)*/)
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
	/*�X�e�[�W�̊֐��łȂ��X�V�֐�(player->GetWorldPos())*/

	if (KeyboardInput::GetInstance().KeyReleaseTrigger(DIK_SPACE))
	{
		//if(/*�X�e�[�W�֐� /�������Ƃ��낪�Ⴄ�{�^����������(player->GetWorldPos())*/)
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
	/*�X�e�[�W�֐�/ �L�[�{�[�h�ɂ���ĉ�](player->getWorldPos())*/
	//�L�[�{�[�h���֐����Ŏ擾

	//��]�I���
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		player->isTurnNow = false;
		player->ChangeStateTurnConnect(new StateNormalConTurP);
	}
}

void StateTurnP::Draw(Camera* camera, Model* model)
{
}


