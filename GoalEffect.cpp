#include "GoalEffect.h"

void GoalEffect::Initialize()
{
	isBegine = false;

	time = 0;
	timeMax = 0;

	target;

	poses.clear();

	index = 0;

	timeRate = 0;

	isEnd = false;
}

void GoalEffect::Update()
{
	if (isBegine) {

		time++;

		//tが1.0以上になったら次の区間に進む
		timeRate = time / timeMax;

		if (timeRate >= 1.0f)
		{
			if (index < poses.size() - 3)
			{
				index++;
				timeRate -= 1.0f;
				time = 0;
			}
			else
			{
				timeRate = 1.0f;

				isEnd = true;
				//poses.clear();

			}
		}

		this->goalEffectCamera.viewMat.eye = SplinePosition(poses, index, timeRate);
		this->goalEffectCamera.viewMat.target = target;

		goalEffectCamera.UpdateViewMatrix();



	}
}

void GoalEffect::Draw(Camera* camera)
{
}

void GoalEffect::BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time)
{
	//一番最初と最後に同じのを入れる処理を省くため
	std::vector<Vec3> Poses;
	Poses.push_back(poses[0]);
	for (int i = 0; i < poses.size(); i++)
	{
		Poses.push_back(poses[i]);
		if (i == poses.size() - 1) { Poses.push_back(poses[i]); }
	}

	this->poses = Poses;
	this->target = target;
	this->timeMax = time;
	this->time = 0;
	this->isBegine = true;

	this->index = 1;

}
