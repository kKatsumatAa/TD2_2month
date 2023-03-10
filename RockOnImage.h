#pragma once
#include"Object.h"
#include"Camera.h"

class RockOnImage
{
private:
	//ゴール通れるときのロックオン
	bool isGoalConnect = false;
	int isGoalConnectCount = 0;
	const int isGoalConnectCountTmp = 25;
	float isGoalConnectScale = 1.0f;
	float isGoalConnectRot = 0.0f;

	Vec3 pos;

	Object obj;

	UINT64 texhandle;

	int count = 0;

public:
	RockOnImage& operator=(const RockOnImage& obj);

	void Initialize();

	void Update(/*Vec3 pos*/);

	void Draw(Camera* camera);

	//回転、拡縮あり
	void BeginEffect(Vec3 pos);
	//回転、拡縮ないパターン
	void BeginEffect2(Vec3 pos);
	//ロックオン消す
	void FinishEffect();
	//ロックオン表示がオンになっているか
	bool GetIsEffectFlag() { return isGoalConnect; }
};

