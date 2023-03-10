#pragma once
#include"CameraManager.h"

class GoalEffect;

class GoalEffectState
{
protected:
	GoalEffect* goalEffect = nullptr;

public:
	void SetGoalEffect(GoalEffect* goalEffect);

	virtual void Initialize() = 0;

	virtual void Update() = 0;
	virtual void Draw() = 0;

};

//パーティクル
class StateGoalParticle : public GoalEffectState
{
protected:
	int count = 0;
	int countMax = 30;
	int particleCount = 0;

public:
	void Initialize() override;

	void Update() override;
	void Draw() override;

};

//カメラ
class StateGoalCamera : public GoalEffectState
{
protected:


public:
	void Initialize() override;

	void Update() override;
	void Draw() override;

};

//-----------------------------------------------
class GoalEffect
{
private:
	bool isBegine = false;

	//
	GoalEffectState* state;

public:
	CameraManager* cameraM;
	bool isEnd = false;
	Vec3 target;
	float time = 0;
	float timeMax = 0;

	std::vector<Vec3> poses;

	int index = 0;

	float timeRate = 0;

	//パーティクル用
	int particleCountMax = 0;
	int particleCoolTmp = 0;
	int particleCool = 0;

public:
	void ChangeState(GoalEffectState* state);
	void Initialize(CameraManager* cameraM);
	void Update();
	void Draw(Camera* camera);

	void BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time, int particleCount = 1, int particleCool = 10);
};

