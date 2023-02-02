#pragma once
#include "BlockManager.h"
#include "ConectLimit.h"

enum STAGE
{
	TUTORIAL,
	STAGE1,
	STAGE2,
	STAGE3,
};

class StageManager
{
	using Form = Block::Form;

public:

	~StageManager();

	//初期化
	void Initialize(BlockManager* blockManager, Tutorial* tutorial);

	//更新
	void Update();

	//ワールド行列セット
	void SetWorldMat(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// チュートリアルをセット
	/// </summary>
	void SetTutorial(const int& blockWidth, const int& blockHeight);
	/// <summary>
	/// ステージ1をセット
	/// </summary>
	void SetStage1(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// ステージ2をセット
	/// </summary>
	void SetStage2(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// ステージ2をセット
	/// </summary>
	void SetStage3(const int& blockWidth, const int& blockHeight);

	/// <summary>
	/// ステージをリセット
	/// </summary>
	void ResetStage();

	/// <summary>
	/// 引数によってステージ選ぶ
	/// </summary>
	/// <param name="stageNum"></param>
	void SelectStage(int stageNum);

	//各ステージのカウントフラグをセット
	void SetisCountReset(int stageNum);

	ConectLimit* GetConectLimit() { return conectLimit_; };

	//チュートリアルステージの長さ
	static const int TutorialWidth = 13;
	static const int TutorialHeight = 13;
	//ステージ1の長さ
	static const int stage1Width = 13;
	static const int stage1Height = 13;

	//ステージ数
	static const int stageMax = 4;
	static int selectStage;

	//外部で参照するための
	int stageWidth = 0;

	//playerの位置
	bool playerPos[13][13] = { false };

private:


	Tutorial* tutorial;

	//ステージ名
	enum Stage
	{
		TUTORIAL,
		Stage1,

	};

	BlockManager* blockManager_;

	//各ブロック用のワールドトランスフォーム
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	Form form_;
	std::vector<std::vector<Block::Form>> forms_;

	//std::unique_ptr<ConectLimit> conectLimit_;
	ConectLimit* conectLimit_;

	bool isCountReset[stageMax] = { 0 };

	int stageNum = 0; 


};

