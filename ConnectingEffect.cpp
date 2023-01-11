#include "ConnectingEffect.h"

void ConnectingEffect::Initialize(Vec3 pos, Vec3 vec, int lifeTime)
{
	worldTransform.trans = pos;
	worldTransform.SetWorld();


	Vec3 normVec = vec.GetNormalized();
	this->pos[0] = pos + normVec;
	this->pos[1] = pos - normVec;
}
