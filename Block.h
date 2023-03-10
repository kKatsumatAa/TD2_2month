#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"
#include "ConnectingEffect.h"

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
		Electric,		//電気出力ブロック
		NONE,			//何もない状態
	};

	enum Action
	{
		Connect,	//繋がっているとき
		None,		//何もしていないとき
	};

private:

	ConnectingEffectManager* connectEM;

	Model* normal_ = nullptr;
	Model* locked_ = nullptr;
	Model* button_ = nullptr;
	Model* goal_ = nullptr;
	Model* socket_ = nullptr;
	Model* disconnectedBlock_ = nullptr;
	Model* disconnectedButton_ = nullptr;
	Model* disconnectedSocketBlock_ = nullptr;
	Model* electricBlock_ = nullptr;
	Model* doorGoalClosed_ = nullptr;
	Model* overlapBlock_ = nullptr;
	//ゴールが出てくる前の枠組み
	Model* beforePopGoal_ = nullptr;

	const float scaleTmp = 1.8f;

	//ブロックの大きさ
	Vec3 scale_;

	//座標
	Vec3 pos_;

	XMFLOAT4 color;
	
	WorldMat goalMat_;

public:

	~Block();

	Object draw[20];

	void Initialize(ConnectingEffectManager* connectEM,
		Model* normal, Model* locked, Model* goal, Model* Socket, Model* button, Model* disconnectedBlock,
		Model* disconnectedButton, Model* disconnectedSocketBlock, Model* electricBlock, Model* doorGoalClosed,
		Model* overlapBlock,Model* beforePopGoal);

	void Updata(Vec3 pos, int form, Action action, bool isElec, int count, WorldMat goalMat, bool isPosGoal);
	void SetAlpha(float blockAlpha);
	void SetColor(Vec3 blockColor);

	void Draw(Camera* camera, UINT64* texhandle, int form, Action action, bool isElec, bool isPushed, int count, float popAlpha,bool isPosGoal,bool isPopGoal);
	
	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

	//今回は使用していない
	void OnCollision(Collider& collider)override;
	void OnCollision2(Collider& collider)override;

	//ゲッター
	const Vec3 GetPos(Vec3 pos) const { return pos_; };
	const Vec3 GetScale(Vec3 scale) const { return scale_; };
	float GetRadius() { return radius_; };

	//セッター
	void SetWorldPos(Vec3& pos);
	void SetScale(const Vec3& scale);
};

