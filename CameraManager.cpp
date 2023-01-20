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

void CameraManager::Initialize()
{
	
}

void CameraManager::Update()
{
	usingCamera->Update();
	//�Q�[�������C���Ŏg���J����
	gameMainCamera->Update();
	//�Q�[�����A��]���Ɏg���J����
	gameTurnCamera->Update();
	//�S�[�����o���Ɏg���J����
	goalEffectCamera->Update();
}
