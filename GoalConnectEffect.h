#pragma once
#include"Collider.h"


class GoalConnectEffect
{
private:
	bool isAlive = true;
	int deathTime = 0;
	int lifeTime = 0;
	float scaleTmp = 0.0f;

	WorldMat worldTransform;

	float t = 0;

	Object obj;

	XMFLOAT4 color;
	XMFLOAT4 startColor;
	XMFLOAT4 endColor;

	Vec3 startScale;
	Vec3 endScale;

	Vec3 startAngle;
	Vec3 endAngle;

public:

	void Initialize(Vec3 pos, Vec3 startScale, Vec3 endScale
		, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime, Vec3 startAngle = { 0,0,0 }, Vec3 endAngle = { 0,0,0 });
	void Update();
	void Draw(Camera& camera);

	bool GetIsAlive() { return isAlive; }
	void SetIsAlive(bool is) { isAlive = is; }
};


class GoalConnectEffectManager
{
private:

public:
	//
	std::list<std::unique_ptr<GoalConnectEffect>> goalConnectEffects_;


	void Initialize();

	void GenerateGoalConnectEffect(Vec3 pos, Vec3 startScale, Vec3 endScale
		, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime, Vec3 startAngle = { 0,0,0 }, Vec3 endAngle = { 0,0,0 });
	void Update();
	void Draw(Camera* camera);

	//ÉäÉXÉgÇéÊìæ(constéQè∆)
	const std::list<std::unique_ptr<GoalConnectEffect>>& GetGoalConnectEffects()
	{
		return goalConnectEffects_;
	}
};