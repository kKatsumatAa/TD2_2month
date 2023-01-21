#pragma once
#include"Camera.h"

class CameraManager;

class UsingCameraState
{
protected:
	CameraManager* cameraM = nullptr;

public:
	void SetCameraM(CameraManager* cameraM);
	virtual void Update() = 0;
};

class UsingCameraNormalState : public UsingCameraState
{
private:


public:
	void Update() override;
};

class UsingCameraLerpMoveState : public UsingCameraState
{
private:

public:
	void Update() override;
};


class CameraManager
{
private:
	UsingCameraState* state = nullptr;

public:
	//ここにいろんなカメラのポインタを入れて、描画時に使う
	Camera* usingCamera;
	//ゲーム中メインで使うカメラ
	std::unique_ptr<Camera> gameMainCamera;
	//ゲーム中、回転時に使うカメラ
	std::unique_ptr<Camera> gameTurnCamera;
	//ゴール演出時に使うカメラ
	std::unique_ptr<Camera> goalEffectCamera;

	//lerp用
	int lerpCount = 0;
	int lerpCountMax = 0;
	Vec3 startEye, endEye, startTarget, endTarget, startUp, endUp;

public://関数
	CameraManager();
	~CameraManager();

	void ChangeUsingCameraState(UsingCameraState* state);

	void Initialize();

	void Update();

	//移動の線形補完を始める
	void BegineLerpUsingCamera(Vec3 startEye, Vec3 endEye, Vec3 startTarget, Vec3 endTarget, Vec3 startUp, Vec3 endUp, int time);
};