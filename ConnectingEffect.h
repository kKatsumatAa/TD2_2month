#pragma once
#include"Collider.h"

//雷の一個
class ConnectingEffect
{
private:
	bool isAlive = true;
	int deathTime = 0;
	int lifeTime = 0;
	float scaleTmp = 0.0f;

	WorldMat worldTransform;

	float t = 0;

	Object obj;

public:
	void Initialize(Vec3 pos, Vec3 rot, float length, int lifeTime, float t);
	void Update();
	void Draw(Camera& camera);

	bool GetIsAlive() { return isAlive; }
	void SetIsAlive(bool is) { isAlive = is; }
};

//雷の塊クラス
class ConnectingEffectSet
{
private:
	std::vector<std::unique_ptr<ConnectingEffect>> connectingEffects;
	std::vector<std::unique_ptr<ConnectingEffect>>::iterator itr;
	int count = 0;
	int countMax = 0;
	bool isEnd = false;
	bool isFinal = false;

	Vec3 oldPos;
	Vec3 oldRot;
	float oldLength;
	float radius;
	int lifeTime;
	int num;
	

public:
	void Initialize(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num, int countMax);
	void Update();
	void Draw(Camera& camera);

	void PushBackConnectEffect(ConnectingEffect* connectE);

	bool GetIsEnd() { return isEnd; }

	void GenerateConnectingEffect(Vec3 pos, Vec3 rot, float length, int lifeTime, float t);
	void GenerateRandomConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num);
};

class ConnectingEffectManager
{
private:
	int countMax = 0;

public:
	//雷の塊を,まとめたリスト
	std::list<std::unique_ptr<ConnectingEffectSet>> connectingEffects_;


public:
	void Initialize();
	void Update();
	void Draw(Camera& camera);

	void GenerateConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num, int count);
	void GenerateRandomConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num,int count);
};


