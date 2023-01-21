#include "CameraManager.h"

CameraManager::CameraManager()
{
	usingCamera = new Camera();
	gameMainCamera = std::make_unique<Camera>();
	gameTurnCamera = std::make_unique<Camera>();
	goalEffectCamera = std::make_unique<Camera>();

	usingCamera->Initialize();
	gameMainCamera->Initialize();
	gameTurnCamera->Initialize();
	goalEffectCamera->Initialize();

}

CameraManager::~CameraManager()
{

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

	ChangeUsingCameraState(new UsingCameraNormalState);
}

void CameraManager::Update()
{
	state->Update();

	usingCamera->Update();
	//ゲーム中メインで使うカメラ
	gameMainCamera->Update();
	//ゲーム中、回転時に使うカメラ
	gameTurnCamera->Update();
	//ゴール演出時に使うカメラ
	goalEffectCamera->Update();
}

void CameraManager::BegineLerpUsingCamera(Vec3 startEye, Vec3 endEye, Vec3 startTarget, Vec3 endTarget, Vec3 startUp, Vec3 endUp,int time)
{
	this->startEye = startEye;
	this->endEye = endEye;
	this->startTarget = endTarget;
	this->endTarget = endTarget;
	this->startUp = startUp;
	this->endUp = endUp;
	this->lerpCountMax = time;
	this->lerpCount = 0;

	ChangeUsingCameraState(new UsingCameraLerpMoveState);
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

	cameraM->lerpCount++;

	cameraM->usingCamera->SetEye(LerpVec3(cameraM->startEye, cameraM->endEye, EaseOut(t)));
	cameraM->usingCamera->SetUp(LerpVec3(cameraM->startUp, cameraM->endUp, EaseOut(t)));
	cameraM->usingCamera->SetTarget(LerpVec3(cameraM->startTarget, cameraM->endTarget, EaseOut(t)));


	if (cameraM->lerpCount >= cameraM->lerpCountMax)
	{
		cameraM->ChangeUsingCameraState(new UsingCameraNormalState);
	}
}
