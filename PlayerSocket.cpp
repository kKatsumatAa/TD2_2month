#include "PlayerSocket.h"


void PlayerSocket::ChangeState(PlayerSocketState* state)
{
	delete this->state;
	this->state = state;

	worldTransform_.scale = { 1.0f,0.5f,1.0f };
	worldTransform_.SetWorld();

	state->SetPlayerSocket(this);
}

void PlayerSocket::OnCollision(Collider& collider)
{
}

void PlayerSocket::OnCollision2(Collider& collider)
{
}

void PlayerSocket::Initialize(Model* model)
{
	assert(model);

	this->model = model;
	isUsed = false;

	if (obj == nullptr)
	{
		obj = new Object();
	}

	ChangeState(new StateNormalSocket);
}

void PlayerSocket::Update(Vec3 pos)
{
	state->Update(pos);

	worldTransform_.SetWorld();
}

void PlayerSocket::Draw(Camera* camera)
{
	state->Draw(camera, model);

	obj->DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, model, { 1.0f,1.0f,1.0f,1.0f });
}

void PlayerSocket::UseSocket(Vec3 endPos)
{
	if (!isUsed)
	{
		isUsed = true;
		this->endPos = endPos;
		startPos = GetWorldPos();

		plugInPosTmp = endPos;
	}
}

void PlayerSocket::FinishSocket(Vec3 playerPos)
{
	if (isUsed)
	{
		isUsed = false;
		endPos = playerPos;
		startPos = GetWorldPos();
	}
}

//--------------------------------------------------------------------------------------
void PlayerSocketState::SetPlayerSocket(PlayerSocket* playerSocket)
{
	this->playerSocket = playerSocket;
}

//--------------------------------------------------------------------------------
void StateNormalSocket::Update(Vec3 pos)
{
	playerSocket->SetWorldPos(pos);

	//�O���Ŏg�p�t���O����������
	if (playerSocket->GetIsUsed())
	{
		playerSocket->ChangeState(new StatePlugInSocket);
	}
}

void StateNormalSocket::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------------
void StatePlugInSocket::Update(Vec3 pos)
{
	count++;

	float t = (float)count / (float)countMax;

	playerSocket->SetWorldPos(LerpVec3(playerSocket->startPos, playerSocket->endPos, EaseIn(t)));

	//��莞�Ԍo�߂őJ��
	if (count >= countMax)
	{
		playerSocket->startPos = playerSocket->GetWorldPos();

		playerSocket->ChangeState(new StateUsingSocket);
	}
}

void StatePlugInSocket::Draw(Camera* camera, Model* model)
{
}

//--------------------------------------------------------------------------------
void StateUsingSocket::Update(Vec3 pos)
{
	effectCount++;

	Vec3 Pos = playerSocket->plugInPosTmp;
	playerSocket->SetWorldPos({ Pos.x,Pos.y + sinf(effectCount * 0.1f),Pos.z });

	//�O���ŁV
	if (!playerSocket->GetIsUsed())
	{
		count++;
		float t = (float)count / (float)countMax;

		playerSocket->SetWorldPos(LerpVec3(playerSocket->startPos, playerSocket->endPos, EaseIn(t)));

		if (count >= countMax)
		{
			playerSocket->ChangeState(new StateNormalSocket);
		}
	}
}

void StateUsingSocket::Draw(Camera* camera, Model* model)
{
}