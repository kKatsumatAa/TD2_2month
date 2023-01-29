#pragma once
#include "Collider.h"
class PredictBlock :
    public Collider
{
private:
    Object obj;


public:
    void Initialize(Vec3 pos, Vec3 scale);
    void Update(int count);
    void Draw(Camera* camera);
};



class PredictBlockManager
{
private:
    std::list<std::unique_ptr<PredictBlock>> predictBlocks_;

public:
    void Initialize();
    void Update();
    void Draw(Camera* camera);

    void AddPredictBlock(Vec3 pos, Vec3 scale);
    void ClearPredictBlock();
};

