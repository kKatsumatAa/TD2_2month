#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"

//ブロック
class Block : public Collider
{

public:
	//形態フェーズ
	enum Form
	{
		BLOCK = 0,		//ブロック状態
		GEAR,			//ギア
		BUTTON,			//ボタン
		LOCKED,			//固定ブロック
		GOAL,			//ゴール
		None,			//何もない状態
	};

	enum Action
	{
		Rotate,		//回転しているとき
		Overlap,	//重なっているとき
		Connect,	//繋がっているとき
		None,		//何もしていないとき
	};

private:

	Model* model_ = nullptr;

	//テクスチャハンドル
	UINT64 textureHandle_[10];

	const float scaleTmp = 1.8f;

	bool isRotate_;		//回転しているかどうか
	bool isOverlap_;	//重なっているかどうか
	bool isConnect_;	//繋がっているかどうか
	bool isAxis_;		//軸になっているかどうか

	//ブロックの大きさ
	Vec3 scale_;
	//半径
	float radius_;

	//座標
	Vec3 pos_;

	//移動方向
	float moveDistance_;

	

public:

	Object draw[10];
	DebugText* debugText_ = nullptr;

	void Initialize(Model* model, DebugText* debugText_);

	void Updata();

	void Draw(Camera* camera);

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//衝突時に呼ばれる
	void OnCollision(Collider& collider)override;
	//手と敵の判定用
	void OnCollision2(Collider& collider)override;
	
	//ゲッター
	const Vec3 GetPos(Vec3 pos) const { return pos_; };
	const Vec3 GetScale(Vec3 scale) const { return scale_; };

	const WorldMat GetWorldTransform() { return worldTransform_; };


	//セッター
};

