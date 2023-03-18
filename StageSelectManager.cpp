#include "StageSelectManager.h"
#include "ParticleManager.h"

void StageSelectManager::Initialize(StageManager* stageM)
{
	this->selectNumMax = stageM->stageMax;
	this->selectNum = stageM->selectStage;

	for (int i = 0; i < this->selectNumMax; i++)
	{
		object[i].worldMat->scale = { stageImageRadius * 1.6f,stageImageRadius * 1.6f,1.0f };
		object[i].worldMat->rot = { 0,0,0 };
		object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),-stageImageRadius * 2.5f,0.0f };
		//object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f),0,0 };
		object[i].worldMat->SetWorld();

		//ステージイメージ
		objectSI[i].worldMat->scale = { 0,0 ,1.0f };
		objectSI[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),stageImageLength.y * 0.3f,0.1f };
		objectSI[i].worldMat->SetWorld();
	}
	//背景
	object[selectNumMax].worldMat->scale = { 12.8f * 5.8f,7.2f * 5.8f,1.0f };
	object[selectNumMax].worldMat->trans = { 0,0,1.0f };
	object[selectNumMax].worldMat->SetWorld();
	//スペースボタン
	object[selectNumMax + 1].worldMat->scale = { stageImageRadius * 1.6f,stageImageRadius * 0.6f,1.0f };
	object[selectNumMax + 1].worldMat->trans = { 0,-33.0f,0.0f };
	object[selectNumMax + 1].worldMat->SetWorld();
	//十字キー
	object[selectNumMax + 2].worldMat->scale = { stageImageRadius * 1.1f,stageImageRadius * 0.9f,1.0f };
	object[selectNumMax + 2].worldMat->trans = { 50.0f,-33.0f,0.0f };
	object[selectNumMax + 2].worldMat->SetWorld();

	//枠
	objSIF.worldMat->trans = { 0,+stageImageLength.y * 0.3f,0.01f };
	objSIF.worldMat->scale = { 0,0,0 };
	objSIF.worldMat->SetWorld();

	stageImageColor = { 1000,1000,1000,0.3f };
	this->isLerpMoving = true;
	this->stageM = stageM;
	isTutorial = false;
	isSelect = false;

	isRight = false;
	isLeft = false;

	moveCool = 0;

	lerpCount = 0;

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
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageFrame10.png", texhandle[11]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/selectBackground.png", texhandle[12]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/spaceKey.png", texhandle[13]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/arrowKey.png", texhandle[14]);
		//ステージイメージ
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s0.png", texhandleSI[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s1.png", texhandleSI[1]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s2.png", texhandleSI[2]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s3.png", texhandleSI[3]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s4.png", texhandleSI[4]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s5.png", texhandleSI[5]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s6.png", texhandleSI[6]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s7.png", texhandleSI[7]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s8.png", texhandleSI[8]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s9.png", texhandleSI[9]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s10.png", texhandleSI[10]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/stageImage/s10.png", texhandleSI[11]);
		//枠
		TextureManager::GetInstance().LoadGraph(L"Resources/image/UI_LevelStage_Frame.png", texhandleSIF);
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

			objectSI[selectNum].worldMat->scale = { stageImageLength.x * 0,stageImageLength.y * 0,1.0f };
			stageImageColor = { 1000,1000,1000,0.6f };
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

			objectSI[selectNum].worldMat->scale = { stageImageLength.x * 0,stageImageLength.y * 0,1.0f };
			stageImageColor = { 1000,1000,1000,0.6f };
			//
			Sound::GetInstance().PlayWave("arrow (2).wav", 0.5f);
		}
	}

	//イージングで動かす
	if (isLerpMoving)
	{
		for (int i = 0; i < this->selectNumMax; i++)
		{
			object[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),-stageImageRadius * 2.5f,0.0f };
			//ステージイメージ
			objectSI[i].worldMat->trans = { (i - selectNum) * (stageImageRadius * 2.0f + stageImageRadius / 2.0f) * EaseOut((float)lerpCount / (float)lerpCountMax),+stageImageLength.y * 0.3f,0.1f };
			objectSI[i].worldMat->scale = { LerpVec3({stageImageLength.x,0,1.0f},{stageImageLength.x,stageImageLength.y,1.0f},EaseOut((float)lerpCount * 3.0f / (float)lerpCountMax)) };

			if (isRight)
			{
				object[i].worldMat->rot.z = { LerpVec3({0,0,0},{0,0,-pi * 2.0f}, EaseOut((float)lerpCount / (float)lerpCountMax)).z };
			}
			else if (isLeft)
			{
				object[i].worldMat->rot.z = { LerpVec3({0,0,0},{0,0,pi * 2.0f}, EaseOut((float)lerpCount / (float)lerpCountMax)).z };
			}
			object[i].worldMat->SetWorld();
			objectSI[i].worldMat->SetWorld();
		}

		//イメージの枠
		{
			objSIF.worldMat->trans = { 0,+stageImageLength.y * 0.3f,0.01f };
			objSIF.worldMat->scale = { LerpVec3({0,0,1.0f},{stageImageLength.x * 1.5f,stageImageLength.y * 1.5f,1.0f},EaseIn((float)lerpCount / (float)lerpCountMax)) };
			objSIF.worldMat->SetWorld();
		}
		

		if (lerpCount >= lerpCountMax)
		{
			isLerpMoving = false;
			isLeft = false;
			isRight = false;
		}

		lerpCount++;
	}
	else
	{
		//ステージイメージ
		objectSI[selectNum].worldMat->scale = { stageImageLength.x * 1.3f,stageImageLength.y * 1.3f ,stageImageRadius };
		objSIF.worldMat->scale = { stageImageLength.x * 1.5f,stageImageLength.y * 1.5f ,stageImageRadius };
	}

	//選択確定
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_SPACE))
	{
		//ここでチュートリアルを表示するか決める
		if (selectNum == STAGE::TUTORIAL || selectNum == STAGE::STAGE1 || selectNum == STAGE::STAGE2 || selectNum == STAGE::STAGE3 || selectNum == STAGE::STAGE5)
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

	//ステージイメージ
	if (!isLerpMoving) {
		stageImageColor.x = 0.5f + fabsf(sinf(count * 0.05f)) * 0.5f;
		stageImageColor.y = 0.5f + fabsf(sinf(count * 0.05f)) * 0.5f;
		stageImageColor.z = 0.5f + fabsf(sinf(count * 0.05f)) * 0.5f;
		stageImageColor.w = 1.0f;
	}
	objectSI[selectNum].DrawBox(objectSI[selectNum].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat, stageImageColor, texhandleSI[selectNum]);
	objSIF.DrawBox(objSIF.worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat, { 1.0f,1.0f,1.0f,1.0f }, texhandleSIF);

	//ステージ1とか
	count++;

	object[selectNum].worldMat->scale = { stageImageRadius + fabsf(sinf(count * 0.05f)) * 1.5f,stageImageRadius + fabsf(sinf(count * 0.05f)) * 1.5f ,stageImageRadius };

	for (int i = 0; i < this->selectNumMax; i++)
	{
		if (object[i].worldMat->scale.x != stageImageRadius && selectNum != i)
		{
			object[i].worldMat->scale = { stageImageRadius * 1.1f  ,stageImageRadius * 1.1f  ,stageImageRadius };
		}

		object[i].DrawBox(object[i].worldMat, &cameraM->usingCamera->viewMat, &cameraM->usingCamera->projectionMat, { 1.0f,1.0f,1.0f,1.0f }, texhandle[i]);
	}
}

void StageSelectManager::DrawSprite()
{
}
