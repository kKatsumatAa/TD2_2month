#pragma once
#include"Object.h"
#include"Camera.h"
#include"StageManager.h"


class StageSelectManager
{
private:
	int selectNum = 0;
	int selectNumMax = 0;

	Object object[15];

	UINT64 texhandle[10];

	int lerpCount = 0;
	const int lerpCountMax = 20;

	bool isLerpMoving = false;

	float stageImageRadius = 7.0f;

	StageManager* stageM;

	int count = 0;

public:
	bool isTutorial = false;
	bool isSelect = false;



public:
	void Initialize(StageManager* stageM);

	void Update();
	void Draw(CameraManager* cameraM);
	void DrawSprite();
};

