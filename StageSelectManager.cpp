#include "StageSelectManager.h"

void StageSelectManager::Initialize(StageManager* stageM)
{
	this->selectNumMax = stageM->stageMax;
	this->selectNum = stageM->selectStage;

	for (int i = 0; i < this->selectNumMax; i++)
	{
		object[i].worldMat->scale = { stageImageRadius,stageImageRadius,1.0f };
		//object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f),0,0 };
		object[i].worldMat->SetWorld();
	}
	//背景
	object[selectNumMax].worldMat->scale = { 12.8f * 6.0f,7.2f * 6.0f,1.0f };
	object[selectNumMax].worldMat->trans = { 0,0,1.0f };
	object[selectNumMax].worldMat->SetWorld();
	//スペースボタン
	object[selectNumMax + 1].worldMat->scale = { stageImageRadius * 1.6f,stageImageRadius * 0.6f,1.0f };
	object[selectNumMax + 1].worldMat->trans = { 0,-30.0f,0.0f };
	object[selectNumMax + 1].worldMat->SetWorld();
	//十字キー
	object[selectNumMax + 2].worldMat->scale = { stageImageRadius * 1.1f,stageImageRadius * 0.9f,1.0f };
	object[selectNumMax + 2].worldMat->trans = { 50.0f,-30.0f,0.0f };
	object[selectNumMax + 2].worldMat->SetWorld();

	this->isLerpMoving = true;
	this->stageM = stageM;
	isTutorial = false;
	isSelect = false;

	if (texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlameTutorial.png", texhandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame1.png", texhandle[1]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame2.png", texhandle[2]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame3.png", texhandle[3]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/selectBackground.png", texhandle[4]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/spaceKey.png", texhandle[5]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowKey.png", texhandle[6]);
	}
}

void StageSelectManager::Update()
{
	//十字キーで選択
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_A)) /*&& !this->isLerpMoving*/)
	{
		if (this->selectNum > 0)
		{
			selectNum--;
			this->isLerpMoving = true;
			this->lerpCount = 0;
		}
	}
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_D)) /*&& !this->isLerpMoving*/)
	{
		if (this->selectNum < this->selectNumMax - 1)
		{
			selectNum++;
			this->isLerpMoving = true;
			this->lerpCount = 0;
		}
	}

	//イージングで動かす
	if (isLerpMoving)
	{
		lerpCount++;

		for (int i = 0; i < this->selectNumMax; i++)
		{
			object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),0,0.0f };
			object[i].worldMat->SetWorld();
		}

		if (lerpCount >= lerpCountMax)
		{
			isLerpMoving = false;
		}
	}

	//選択確定
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		if (selectNum < STAGE::STAGE3)
		{
			isTutorial = true;
		}
		isSelect = true;

		stageM->SelectStage(selectNum);
	}
}

void StageSelectManager::Draw(CameraManager* cameraM)
{
	//ステージ1とか
	count++;

	object[selectNum].worldMat->scale = { stageImageRadius + sinf(count * 0.1f),stageImageRadius + sinf(count * 0.1f) ,stageImageRadius };

	for (int i = 0; i < this->selectNumMax; i++)
	{
		if (object[i].worldMat->scale.x != stageImageRadius && selectNum != i)
		{
			object[i].worldMat->scale = { stageImageRadius ,stageImageRadius ,stageImageRadius };
		}

		object[i].DrawBox(object[i].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat, { 1.0f,1.0f,1.0f,1.0f }, texhandle[i]);
	}

	//背景など
	for (int i = selectNumMax; i < this->selectNumMax + 3; i++)
	{
		object[i].DrawBox(object[i].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat,
			{ 1.0f,1.0f,1.0f,1.0f }, texhandle[i]);
	}
}

void StageSelectManager::DrawSprite()
{
}
