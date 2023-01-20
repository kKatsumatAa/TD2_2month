#pragma once
#include"Camera.h"

class CameraManager
{
private:


public:
	//ここにいろんなカメラのポインタを入れて、描画時に使う
	Camera* usingCamera;
	//ゲーム中メインで使うカメラ
	std::unique_ptr<Camera> gameMainCamera;
	//ゲーム中、回転時に使うカメラ
	std::unique_ptr<Camera> gameTurnCamera;
	//ゴール演出時に使うカメラ
	std::unique_ptr<Camera> goalEffectCamera;


public://関数
	CameraManager();
	~CameraManager();

	void Initialize();

	void Update();

};