#pragma once

#include"BlockManager.h"
#include"Player.h"
#include"BlockManager.h"

class SaveData
{
public:
	std::unique_ptr < Player> player;
	std::unique_ptr < PlayerSocket> playerSocket;
	std::unique_ptr < BlockManager> blockManager;
	std::unique_ptr < CameraManager> cameraManager;
	std::unique_ptr < PredictBlockManager> predictBlockM;
	std::unique_ptr < RockOnImage> rockOnImage;

public:
	SaveData(const SaveData& obj) {};
	SaveData() { ; }
	~SaveData() { ; }
};

/// <summary>
/// 一手戻るための情報を持ったクラス
/// </summary>
class GetBackManager
{
private:
	GetBackManager() {}
	~GetBackManager();

	//一手戻すときにこのポインタにセーブした情報入れる
	Player* player_;
	PlayerSocket* playerSocket_;
	BlockManager* blockManager_;
	CameraManager* cameraManager_;
	PredictBlockManager* predictBlockM;
	RockOnImage* rockOnImage;

	std::unique_ptr< SaveData> saveData;

	std::list < std::unique_ptr< SaveData>> saveDatas_;

public:
	//コピーコンストラクタを無効
	GetBackManager(const GetBackManager& obj) = delete;
	//代入演算子も
	GetBackManager& operator=(const GetBackManager& obj) = delete;
	//インスタンスを取得（シングルトン）
	static GetBackManager* GetInstance();

	void Initialize(Player* player, PlayerSocket* playerSocket, BlockManager* blockManager, CameraManager* cameraM, PredictBlockManager* predictBlockM, RockOnImage* rockOnImage);

	//現在の状態をセーブ
	void SaveDatas();

	void Update();

	//一手戻る
	void GetBack();
};

