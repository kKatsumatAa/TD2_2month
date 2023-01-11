#pragma once
#include"Collider.h"

class ConnectingEffect
{
private:
	bool isAlive = true;
	int deathTime = 0;
	int lifeTime = 0;

	Vec3 pos[2];

	WorldMat worldTransform;

public:
	void Initialize(Vec3 pos, Vec3 vec, int lifeTime);

};

