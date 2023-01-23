#include "CameraManager.h"

CameraManager::CameraManager()
{
	usingCamera = new Camera();
	stageSelectCamera = std::make_unique<Camera>();
	gameMainCamera = std::make_unique<Camera>();
	gameTurnCamera = std::make_unique<Camera>();
	goalEffectCamera = std::make_unique<Camera>();

	usingCamera->Initialize();
	stageSelectCamera->Initialize();
	gameMainCamera->Initialize();
	gameTurnCamera->Initialize();
	goalEffectCamera->Initialize();

}

CameraManager::~CameraManager()
{
	stageSelectCamera.reset();
	gameMainCamera.reset();
	gameTurnCamera.reset();
	goalEffectCamera.reset();
}

void CameraManager::ChangeUsingCameraState(UsingCameraState* state)
{
	delete this->state;
	this->state = state;
	this->state->SetCameraM(this);
}

void CameraManager::Initialize()
{
	lerpCount = 0;
	lerpCountMax = 0;
	afterCount = 0;
	afterCamera = nullptr;

	ChangeUsingCameraState(new UsingCameraNormalState);
}

void CameraManager::Update()
{
	state->Update();

	usingCamera->Update();

	stageSelectCamera->Update();
	//�Q�[�������C���Ŏg���J����
	gameMainCamera->Update();
	//�Q�[�����A��]���Ɏg���J����
	gameTurnCamera->Update();
	//�S�[�����o���Ɏg���J����
	goalEffectCamera->Update();
}

void CameraManager::BegineLerpUsingCamera(Vec3 startEye, Vec3 endEye, Vec3 startTarget, Vec3 endTarget, Vec3 startUp, Vec3 endUp, int time, Camera* afterCamera, int afterCount)
{
	this->startEye = startEye;
	this->endEye = endEye;
	this->startTarget = endTarget;
	this->endTarget = endTarget;
	this->startUp = startUp;
	this->endUp = endUp;
	this->lerpCountMax = time;
	this->lerpCount = 0;
	this->afterCamera = afterCamera;
	this->afterCount = afterCount;

	ChangeUsingCameraState(new UsingCameraLerpMoveState);
}

CameraManager& CameraManager::operator=(const CameraManager& obj)
{
	this->Initialize();

	*this->state = *obj.state;//�X�e�[�g�ȂǁA�|�C���^�͍폜�����\��������̂Œ��g�̂݃R�s�[
	this->usingCamera = obj.usingCamera;
	*this->stageSelectCamera.get() = *obj.stageSelectCamera.get();
	*this->gameMainCamera.get() = *obj.gameMainCamera.get();
	*this->gameTurnCamera.get() = *obj.gameTurnCamera.get();
	*this->goalEffectCamera.get() = *obj.goalEffectCamera.get();
	this->startEye = obj.startEye;
	this->endEye = obj.endEye;
	this->startTarget = obj.startTarget;
	this->endTarget = obj.endTarget;
	this->startUp = obj.startUp;
	this->endUp = obj.endUp;
	this->lerpCountMax = obj.lerpCountMax;
	this->lerpCount = obj.lerpCount;
	if (afterCamera) { *this->afterCamera = *obj.afterCamera; }
	this->afterCount = obj.afterCount;

	return *this;
}


//----------------------------------------------------------------------------------------------------------------------------------------
void UsingCameraState::SetCameraM(CameraManager* cameraM)
{
	this->cameraM = cameraM;
}


//-------------------------------------------------------------------------------------------------------
void UsingCameraNormalState::Update()
{

}

//-------------------------------------------------------------------------------------------------------
void UsingCameraLerpMoveState::Update()
{
	float t = (float)cameraM->lerpCount / (float)cameraM->lerpCountMax;

	if (cameraM->lerpCount < cameraM->lerpCountMax)
	{
		cameraM->lerpCount++;
	}

	cameraM->usingCamera->SetEye(LerpVec3(cameraM->startEye, cameraM->endEye, EaseOut(t)));
	cameraM->usingCamera->SetUp(LerpVec3(cameraM->startUp, cameraM->endUp, EaseOut(t)));
	cameraM->usingCamera->SetTarget(LerpVec3(cameraM->startTarget, cameraM->endTarget, EaseOut(t)));


	if (cameraM->lerpCount >= cameraM->lerpCountMax)
	{
		cameraM->afterCount--;

		if (cameraM->afterCount <= 0)
		{

			if (cameraM->afterCamera)
			{
				cameraM->usingCamera = cameraM->afterCamera;
			}

			cameraM->ChangeUsingCameraState(new UsingCameraNormalState);
		}
	}
}
