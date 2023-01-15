#pragma once
#include"Util.h"
#include<random>

//�����V�[�h������
static std::random_device seed_gen;
//�����Z���k�c�C�X�^�[
static std::mt19937_64 engine(seed_gen());

class Shake
{
private:
	float shakeLength = 0.0f;
	int   lifeTimer = 0;
	int   lifeTimerTmp = 0;
	float t = 0.0f;

private:
	float GetRandomShakeInternal(float min, float max);
	void SetRandomLength(float length);
	void SetLifeTime(int time);

public:
	void SetShake(int time, float length);
	float GetShake();

	void Initialize();

	void Update();

};

