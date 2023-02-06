#include "StageSelectManager.h"
#include "ParticleManager.h"

void StageSelectManager::Initialize(StageManager* stageM)
{
	this->selectNumMax = stageM->stageMax;
	this->selectNum = stageM->selectStage;

	for (int i = 0; i < this->selectNumMax; i++)
	{
		object[i].worldMat->scale = { stageImageRadius,stageImageRadius,1.0f };
		object[i].worldMat->rot = { 0,0,0 };
		//object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f),0,0 };
		object[i].worldMat->SetWorld();
	}
	//背景
	object[selectNumMax].worldMat->scale = { 12.8f * 5.7f,7.2f * 5.7f,1.0f };
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

	isRight = false;
	isLeft = false;

	moveCool = 0;

	if (texhandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlameTutorial.png", texhandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame1.png", texhandle[1]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame2.png", texhandle[2]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFlame3.png", texhandle[3]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame4.png", texhandle[4]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame5.png", texhandle[5]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame6.png", texhandle[6]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame7.png", texhandle[7]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame8.png", texhandle[8]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame9.png", texhandle[9]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame10.png", texhandle[10]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/selectBackground.png", texhandle[11]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/spaceKey.png", texhandle[12]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowKey.png", texhandle[13]);
	}
}

void StageSelectManager::Update()
{
	moveCool--;

	//十字キーで選択
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_A)) /*&& !this->isLerpMoving*/
		|| ((KeyboardInput::GetInstance().KeyPush(DIK_LEFTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_A)) && moveCool <= 0)
		)
	{
		if (this->selectNum > 0)
		{
			moveCool = moveCoolTmp;

			ParticleManager::GetInstance()->GenerateRandomParticle(20, 120, 3.0f, object[selectNum].worldMat->trans, 1.0f, 0.1f, { 1.0f,1.0f,0,0.7f }, { 0,0,0,0 });

			selectNum--;
			this->isLerpMoving = true;
			this->lerpCount = 0;
			isLeft = true;

			//音
			Sound::GetInstance().PlayWave("arrow (2).wav", 0.5f);
		}
	}
	if ((KeyboardInput::GetInstance().KeyTrigger(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyTrigger(DIK_D)) /*&& !this->isLerpMoving*/
		|| ((KeyboardInput::GetInstance().KeyPush(DIK_RIGHTARROW) || KeyboardInput::GetInstance().KeyPush(DIK_D)) && moveCool <= 0)
		)
	{
		if (this->selectNum < this->selectNumMax - 1)
		{
			moveCool = moveCoolTmp;

			ParticleManager::GetInstance()->GenerateRandomParticle(20, 120, 2.0f, object[selectNum].worldMat->trans, 1.0f, 0.1f, { 1.0f,1.0f,0,0.7f }, { 0,0,0,0 });

			selectNum++;
			this->isLerpMoving = true;
			this->lerpCount = 0;
			isRight = true;

			//
			Sound::GetInstance().PlayWave("arrow (2).wav", 0.5f);
		}
	}

	//イージングで動かす
	if (isLerpMoving)
	{
		lerpCount++;

		for (int i = 0; i < this->selectNumMax; i++)
		{
			object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),0,0.0f };
			if (isRight)
			{
				object[i].worldMat->rot.z = { LerpVec3({0,0,0},{0,0,-pi * 2.0f}, EaseOut((float)lerpCount / (float)lerpCountMax)).z };
			}
			else if (isLeft)
			{
				object[i].worldMat->rot.z = { LerpVec3({0,0,0},{0,0,pi * 2.0f}, EaseOut((float)lerpCount / (float)lerpCountMax)).z };
			}
			object[i].worldMat->SetWorld();
		}

		if (lerpCount >= lerpCountMax)
		{
			isLerpMoving = false;
			isLeft = false;
			isRight = false;
		}
	}

	//選択確定
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		//ここでチュートリアルを表示するか決める
		if (selectNum == STAGE::TUTORIAL || selectNum == STAGE::STAGE1 || selectNum == STAGE::STAGE2 || selectNum == STAGE::STAGE4 || selectNum == STAGE::STAGE5)
		{
			isTutorial = true;
		}
		isSelect = true;

		stageM->SelectStage(selectNum);

		//音
		Sound::GetInstance().PlayWave("select.wav", 0.7f);
	}
}

void StageSelectManager::Draw(CameraManager* cameraM)
{
	//背景など
	for (int i = selectNumMax; i < this->selectNumMax + 3; i++)
	{
		object[i].DrawBox(object[i].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat,
			{ 1.0f,1.0f,1.0f,1.0f }, texhandle[i]);
	}

	//ステージ1とか
	count++;

	object[selectNum].worldMat->scale = { stageImageRadius + sinf(count * 0.1f),stageImageRadius + sinf(count * 0.1f) ,stageImageRadius };

	for (int i = 0; i < this->selectNumMax; i++)
	{
		if (object[i].worldMat->scale.x != stageImageRadius && selectNum != i)
		{
			object[i].worldMat->scale = { stageImageRadius  ,stageImageRadius  ,stageImageRadius };
		}

		object[i].DrawBox(object[i].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat, { 1.0f,1.0f,1.0f,1.0f }, texhandle[i]);
	}


}

void StageSelectManager::DrawSprite()
{
}
