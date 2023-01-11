#pragma once
#include "Sound.h"
#include "Collider.h"
#include "Camera.h"

class Block;

//ブロックの状態
class BlockState
{
protected:
	Block* block_ = nullptr;

public:
	void SetBlock(Block* player);
	virtual void Update() = 0;
	virtual void Draw(Camera* camera, Model* model) = 0;

};

//何もしていない状態
class StateNormalB : BlockState
{
private:

public:
	void Update(/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//ブロック
class Block : public Collider
{



private:

	Model* model_ = nullptr;

	//テクスチャハンドル
	UINT64 textureHandle[10];

	const float scaleTmp = 1.8f;


public:

	Object draw[10];
	DebugText* debugText_ = nullptr;

	void Initialize(Model* model, DebugText* debugText_);

	void Updata();

	void Draw(Camera* camera);

	void SetWorldPos(const Vec3& pos) { worldTransform_.trans = pos; };

};

