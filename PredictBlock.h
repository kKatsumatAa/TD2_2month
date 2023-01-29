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



class PredictBlockManager
{
private:



public:
	int count = 0;
	std::list<PredictBlock> predictBlocks_;


	//PredictBlockManager& operator=( PredictBlockManager& obj);

	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void AddPredictBlock(Vec3 pos, Vec3 scale, bool isRight, Model* model);
	void ClearPredictBlock();
};

