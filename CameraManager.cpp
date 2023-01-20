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
	//ゲーム中メインで使うカメラ
	gameMainCamera->Update();
	//ゲーム中、回転時に使うカメラ
	gameTurnCamera->Update();
	//ゴール演出時に使うカメラ
	goalEffectCamera->Update();
}
