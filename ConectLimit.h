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

private:

	//繋ぐ回数制限保存用のカウント
	int limitCount;
	
	//繋ぐ回数
	int conectCount;

};

