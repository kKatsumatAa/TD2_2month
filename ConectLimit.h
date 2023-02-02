#pragma once
class ConectLimit
{
public:

	//ゲッター
	int GetConectcount() { return conectCount; };
	//セッター
	void SetConectCount(int playConectCount) { conectCount -= playConectCount; };
	//リセット
	void ResetCount() { conectCount = limitCount; };

	//残量設定(引数で残量のMAX制限を設定)
	void SetCount(int setCountMax) { limitCount = setCountMax; };

private:

	//繋ぐ回数制限保存用のカウント
	int limitCount;
	
	//繋ぐ回数
	int conectCount;

};

