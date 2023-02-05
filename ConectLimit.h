#pragma once
#include"TextureManager.h"
#include"Util.h"
#include"Camera.h"
#include"DrawIntNumImage.h"

class ConectLimit
{
public:
	void Initialize();

	void Update(Vec3 pos, Camera* camera);

	void Draw();


	//ゲッター
	int GetConectcount() { return conectCount; };
	int GetLimitCount() { return limitCount; };
	//セッター
	void SetConectCount(int playConectCount) { conectCount -= playConectCount; }
	//リセット
	void ResetCount() { conectCount = limitCount; };

	//リセット
	void ResetCounts() { conectCount = 0; limitCount = 0; }

	//残量設定(引数で残量のMAX制限を設定)
	void SetCount(int setCountMax) { limitCount = setCountMax; };

	void SetDrawCount(int count) { drawConnectCount = count; }

private:
	UINT64 texhandle;

	DrawIntNumImage drawNum;

	//繋ぐ回数制限保存用のカウント
	int limitCount;

	//繋ぐ回数
	int conectCount;

	//表示用
	int drawConnectCount;
};

