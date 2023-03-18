#pragma once
#include"Object.h"
#include"Camera.h"
#include"StageManager.h"


class StageSelectManager
{
private:
	int selectNum = 0;
	int selectNumMax = 0;

	Object object[20];
	Object objectSI[12];
	Object objSIF;
	//ステージイメージのサイズ
	const Vec2 stageImageLength = { 21.0f ,14.0f };
	XMFLOAT4 stageImageColor = { 0.5f,0.5f,0.5f,1.0f };

	UINT64 texhandle[20];
	UINT64 texhandleSI[12];
	UINT64 texhandleSIF;

	int lerpCount = 0;
	const int lerpCountMax = 20;

	bool isLerpMoving = false;

	float stageImageRadius = 7.0f;

	StageManager* stageM;

	int count = 0;

	bool isRight = false;
	bool isLeft = false;

	int moveCool = 0;
	const int moveCoolTmp = 10;

public:
	bool isTutorial = false;
	bool isSelect = false;



public:
	void Initialize(StageManager* stageM);

	void Update();
	void Draw(CameraManager* cameraM);
	void DrawSprite();
};

