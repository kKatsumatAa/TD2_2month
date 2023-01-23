#pragma once

#include"BlockManager.h"
#include"Player.h"
#include"BlockManager.h"

class SaveData
{
public:
	Player player;
	PlayerSocket playerSocket;
	BlockManager blockManager;

public:
	SaveData(const SaveData& obj){};
	SaveData(){}
};

/// <summary>
/// 一手戻るための情報を持ったクラス
/// </summary>
class GetBackManager
{
private:
	GetBackManager(){}
	~GetBackManager();

	//一手戻すときにこのポインタにセーブした情報入れる
	 Player* player_;
	 PlayerSocket* playerSocket_;
	 BlockManager* blockManager_;



	 SaveData saveData;

	 std::list<SaveData> saveDatas;

public:
	//コピーコンストラクタを無効
	GetBackManager(const GetBackManager& obj) = delete;
	//代入演算子も
	GetBackManager& operator=(const GetBackManager& obj) = delete;
	//インスタンスを取得（シングルトン）
	static GetBackManager* GetInstance();

	 void Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager);

	//現在の状態をセーブ
	 void SaveDatas();

	//一手戻る
	 void GetBack();
};

