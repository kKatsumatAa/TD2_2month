#pragma once
#include "Block.h"
#include "Tutorial.h"
#include <vector>
#include <string>
#include <sstream>
#include "GoalEffect.h"
#include "PredictBlock.h"
#include "ImGuiManager.h"
#include"GoalConnectEffect.h"
#include"RockOnImage.h"


class BlockManager
{
	using Form = Block::Form;
	using Action = Block::Action;

public:
	BlockManager& operator=(const BlockManager& obj);

	~BlockManager();

	//初期化
	void Initialize(RockOnImage* rockOnImage, ConnectingEffectManager* connectEM, PredictBlockManager* pBM, Tutorial* tutorial, CameraManager* cameraM, GoalEffect* goalEffect,
		GoalConnectEffectManager* goalConnectEM,
		Model* normal, Model* locked, Model* goal, Model* Socket, Model* Button, Model* disconnectedBlock,
		Model* disconnectedButton, Model* disconnectedSocketBlock, Model* electricBlock, Model* doorGoalClosed,Model* overLapBlock, Model* beforeButtonPop);

	//更新
	void Update();

	//描画
	void Draw(Camera* camera);

	//プレイヤーがブロックの上にいるかどうか
	bool CheckPlayerOnBlock(Vec3 pos);

	//ステージの関数で先にブロックあるか判定(endPosを引数)
	bool GetPosIsBlock(Vec3 pos);

	//ボタンがあるかどうか
	bool GetPosIsGear(Vec3 pos);

	//最初に繋ぐボタンを押したブロックを軸に登録する関数
	void RegistAxisGear(const Vec3& pos);

	//ブロック同士をつなぐ更新関数
	void UpdateConnect(Vec3 pos);

	//繋ぐ際に離したところが軸以外のボタンかどうか
	bool CheckAxisGear(Vec3 pos);

	//繋がれているブロックを全部解除する
	void ReleseConectedBlock();

	//キーボードによって回転
	void UpdateRotate(Vec3& rotatePos);

	//回転中だったら
	bool GetIsRollingLeftorRight();

	//ゴールがあるかどうか
	bool GetIsGoal(Vec3& Pos, bool isPlayer = false);

	//重なった時の処理
	void UpdateOverlap();

	//重なっていたブロックを元に戻す処理
	void RepositBlock();

	//ブロックとブロックの当たり判定
	bool CollisionBlockToBlock(Vec3 blockPos, Vec3 pos);

	//ブロックのリセット
	void ResetBlock();

	//回転し終わったときにパーティクル発生させる
	void GenerateParticleTurnBlock();

	//描画用にY座標をずらす関数
	void UpPosY();

	void DownPosY();

	//読み込んだステージをセットする関数
	void SetStage(const int& stageWidth, const int& stageHeight, std::vector<std::vector<WorldMat>>& worldmats, std::vector<std::vector<Form>>& forms);

	//半径を渡す関数
	float GetRadius() { return blockRadius_; };

	//ボタンと重なった時にゴールを出す仕組み
	void AppearGoal();

	//電気ブロックの判定
	void InitializeElectric();

	//隣接しているかどうか
	bool BlockJunction(Vec3 Pos1, Vec3 Pos2);

	void ConectElec();

	//電気を生成し終わったか確認
	bool GetCheckElec() { return isCheckElec_; };

	void SetCheckElec() { isCheckElec_ = false, checkCount = 0; };

	void GeneratePredictBlock();

	//電気確認用Imguiへのゲッター
	bool GetIsElec() const{ return &isElec; }

	//重なっているブロックに乗っているかどうか
	bool GetisLockedBlock(Vec3 pos);

	//ブロックの出現演出
	void PopEffect();

	//ゲームの横の長さ
	float GetGameWidth();
	//ゲームの縦の長さ
	float GetGameHeight();

public:
	bool isPopGoalEffect = false;

	//カメラ後に立つフラグ
	bool isPopedGoal2 = false;
	int  isPopedGoal2Count = 0;
	const int isPopedGoalCountTmp = 50;

	static const int blockWidth = 13;
	static const int blockHeight = 13;

	//外枠の長さ
	static const int gameAreaWidth = 28;
	static const int gameAreaHeight = 22;

	//半径
	const float blockRadius_ = 1.8f;

	//ずれているブロックのマス目の数
	static const int outOfBlockNum = 9;

	PredictBlockManager* predictBlockM;
private:
	Model* normal; Model* locked; Model* goal; Model* Socket; Model* button; Model* disconnectedBlock;
	Model* disconnectedButton; Model* disconnectedSocketBlock; Model* electricBlock; Model* doorGoalClosed;

	CameraManager* cameraM;

	Tutorial* tutorial;

	GoalEffect* goalEffect;

	GoalConnectEffectManager* goalConnectEM;

	UINT64 texhandle[10];

	const int width = 13;

	Model* model_ = nullptr;

	std::vector<Vec3> goalCameraPoses;

	//ブロックの二次元配列
	//std::unique_ptr < std::vector <std::vector<Block>> > blocks_ ;
	std::unique_ptr <Block> block_;
	std::vector<std::vector<std::unique_ptr <Block>>> blocks_;

	//各ブロック用のワールドトランスフォーム
	WorldMat worldmat_;
	std::vector<std::vector<WorldMat>> worldmats_;

	//状態変化変数
	//現在の形
	Form form_[blockWidth][blockHeight];

	//ブロックが行動を行っているかどうか
	Action action_[blockWidth][blockHeight] = { Action::None };

	//軸になっているかどうか
	bool isAxis_[blockWidth][blockHeight];

	//軸になっているブロックの座標
	Vec3 axis_pos_;

	Object draw[10];

	//右回転しているかどうか
	bool isRightRolling;
	//左回転
	bool isLeftRolling;

	int rotateCount;
	const int rotateCountMax = 30;

	float angle_;

	Vec3 distancePos[blockWidth][blockHeight];
	Vec3 distancePosPlayer;

	ConnectingEffectManager* connectEM;

	int effectCount = 0;
	int effectCount2 = 0;
	int count = 0;

	const int effectCountMax = 10;

	//回転する前の形状
	Form beforeTurn_[blockWidth][blockHeight];

	//描画用に高さを調整する変数
	bool isUp[blockWidth][blockHeight];
	bool isDown[blockWidth][blockHeight];

	//回転したかどうか
	bool isTurn[blockWidth][blockHeight];

	//読み込んだブロックの形
	Form loadForm_;
	std::vector<std::vector<Block::Form>> loadForms_;
	//読み込んだブロックの初期座標
	std::vector<std::vector<WorldMat>> loadWorldmats_;

	//読み込んだステージの長さ
	int stageWidth_;
	int stageHeight_;

	//ボタンが押されたかどうか(配列にしているのは複数ボタンに対応するため)
	bool isPushed[blockWidth][blockHeight];
	//押されたボタンの数
	int pushedCount_;

	//ゴールの位置を保存する配列
	bool isGoal_[blockWidth][blockHeight];
	//読み込んだボタンの数から必要な押す数を入れておく
	int needGoalCount;

	//電気ブロック用のワールド座標
	Vec3 elecPos;
	//ゴールの保管用ポジション
	Vec3 goalPos;
	WorldMat goalMat;

	//通った場所を記憶するためのフラグ
	bool isElec[blockWidth][blockHeight];
	
	//ゴールが出現しているかどうか
	bool isPopGoal;

	//一回のみゴール出現演出
	bool isPopedGoal = false;

	//これはfor文の条件で消せそう
	int  goalPopX, goalPopY;

	bool isCheckElec_ = false;

	int checkCount = 0;

	//ゴールがブロックとつながっているかどうか
	bool isConectedGoal;
	//ゴール判定用のfor文のストッパー
	bool isChangedConectGoal = false;
	//ゴールに電気が入っていて、ゴールができる状態のフラグ
	bool isElecConectedGoal = false;
	//ゴール判定用のfor文のストッパー
	bool isStopElecConectedGoal = false;

	//回転しているかどうか
	bool isTurning[blockWidth][blockHeight];

	float maxTime = 60.0f;						//　全体時間[s]
	float timeRate[blockWidth][blockHeight];	// 何% 時間が進んだか(率)

	//ポップ用のカウント
	size_t elecCount_[blockWidth][blockHeight] = { 0 };

	//変更するアルファ値
	float elecWaitAlpha_[blockWidth][blockHeight] = { 0 };
	//変更するアルファ値の移動量
	float timerMoveResult_[blockWidth][blockHeight];

	//リセットの出現フラグ
	bool isPopWait = false;
	//スタートの出現フラグ
	bool isStartPop = true;
	//フェードアウトの出現フラグ
	bool isPopOut = false;

	RockOnImage* rockOnImage;

	//距離制限用のゲーム内の長さ
	float gameWidthX;
	float gameWidthY;

	bool axisLocked;
};

