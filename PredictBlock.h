#pragma once
#include "Collider.h"
class PredictBlock
{
private:
	Object obj;
	int count = 0;
	Model* model;

	WorldMat worldTransform_;

	bool isRight = false;


public:
	void Initialize(Vec3 pos, Vec3 scale, bool isRight, Model* model);
	void Update(int count);
	void Draw(Camera* camera);
};

class PredictArrow
{
private:
	UINT64 texhandle[6];
	int count = 0;
	Object obj[4];

	WorldMat worldTransform_;

	float scaleArrow;
	float colorAlpha = 0.0f;

public:
	void Initialize(Vec3 pos, Vec3 scale);
	void Update(int count);
	void Draw(Camera* camera, bool isArrowDraw);
	float GetColor() { return colorAlpha; }
	void fadeColor();
};

class PredictBlockManager
{
private:


public:
	int count = 0;
	std::list<PredictBlock> predictBlocks_;
	std::list<PredictArrow> predictArrows_;

	//PredictBlockManager& operator=( PredictBlockManager& obj);

	void Initialize();
	void Update();

	void Draw(Camera* camera, bool isArrowDraw);

	void AddPredictBlock(Vec3 pos, Vec3 scale, bool isRight, Model* model);
	void AddPredictArrow(Vec3 pos, Vec3 scale);

	void ClearPredictBlock();
};

