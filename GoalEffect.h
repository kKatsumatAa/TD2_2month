#pragma once
#include"Camera.h"

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
	int countMax = 40;

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
	Camera goalEffectCamera;
	bool isEnd = false;
	Vec3 target;
	float time = 0;
	float timeMax = 0;

	std::vector<Vec3> poses;

	int index = 0;

	float timeRate = 0;

public:
	void ChangeState(GoalEffectState* state);
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time);
};

