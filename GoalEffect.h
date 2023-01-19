#pragma once
#include"Camera.h"


class GoalEffect
{
private:
	bool isBegine = false;

	float time = 0;
	float timeMax = 0;

	Vec3 target;

	std::vector<Vec3> poses;

	int index = 0;

	float timeRate = 0;

	//���Ԍv���ɕK�v�ȃf�[�^
	long long startCount;
	long long nowCount;
	long long elapsedCount;//�o�߃J�E���g��

public:
	Camera goalEffectCamera;
	bool isEnd = false;

public:
	void Initialize();
	void Update();
	void Draw(Camera* camera);

	void BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time);
};

