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

void PlayerSocket::Initialize(ConnectingEffect2Manager* connectE2M, float blockRadius, Model* model)
{
	assert(model);

	this->model = model;
	isUsed = false;

	this->connectE2M = connectE2M;
	this->blockRadius = blockRadius;

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

	//外部で使用フラグが立ったら
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

	//一定時間経過で遷移
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

	if (effectCount % 240 == 240 / 2)
	{
		//エフェクト
		Vec3 startScale = { playerSocket->blockRadius / 1.5f ,playerSocket->blockRadius * 5.0f,playerSocket->blockRadius / 1.5f };
		Vec3 endScale = { playerSocket->blockRadius * 0.01f,playerSocket->blockRadius * 100.0f,playerSocket->blockRadius * 0.01f };
		playerSocket->connectE2M->GenerateConnectingEffect2(playerSocket->GetWorldPos(), startScale, endScale
			, { 1.0f,1.0f,0,0.9f }, { 1.0f,1.0f,1.0f,0.3f }, 240, { 0,pi * 10.0f,0 });
	}

	//外部で〃
	if (!playerSocket->GetIsUsed())
	{
		count++;
		float t = (float)count / (float)countMax;

		playerSocket->SetWorldPos(LerpVec3(playerSocket->startPos, playerSocket->endPos, EaseIn(t)));

		if (count >= countMax)
		{
			//エフェクト消す
			playerSocket->connectE2M->connectingEffect2s_.clear();

			playerSocket->ChangeState(new StateNormalSocket);
		}
	}
}

void StateUsingSocket::Draw(Camera* camera, Model* model)
{
}