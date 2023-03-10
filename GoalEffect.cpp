#include "GoalEffect.h"
#include "ParticleManager.h"

//乱数範囲
static std::uniform_real_distribution<float> scaleDist(0.3f, 2.0f);
static std::uniform_real_distribution<float> colorDist(0.1f, 1.0f);


void GoalEffect::ChangeState(GoalEffectState* state)
{
	delete this->state;
	this->state = state;
	state->SetGoalEffect(this);
	this->state->Initialize();
}

void GoalEffect::Initialize(CameraManager* cameraM)
{
	this->cameraM = cameraM;

	isBegine = false;

	time = 0;
	timeMax = 0;

	target;

	poses.clear();

	index = 0;

	timeRate = 0;

	if (state) { delete state; }
	state = nullptr;

	isEnd = false;

	this->particleCountMax = 0;
	this->particleCoolTmp = 0;
	this->particleCool = 0;
}

void GoalEffect::Update()
{
	if (isBegine) {
		state->Update();
	}
}

void GoalEffect::Draw(Camera* camera)
{
}

void GoalEffect::BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time, int particleCount, int particleCool)
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

	this->particleCountMax = particleCount;
	this->particleCoolTmp = particleCool;
	this->particleCool = particleCool;

	//音
	Sound::GetInstance().PlayWave("goal.wav", 1.2f);

	ChangeState(new StateGoalParticle);

	Update();
}


//-----------------------------------------------------------------------------------
void GoalEffectState::SetGoalEffect(GoalEffect* goalEffect)
{
	this->goalEffect = goalEffect;
}

//-----------------------------------------------------------------------------------
void StateGoalParticle::Initialize()
{
	//カメラの位置だけ入れる
	goalEffect->cameraM->goalEffectCamera->viewMat.eye = goalEffect->poses[0];
	goalEffect->cameraM->goalEffectCamera->viewMat.target = goalEffect->target;

	goalEffect->cameraM->goalEffectCamera->UpdateViewMatrix();
}

void StateGoalParticle::Update()
{
	goalEffect->particleCool++;

	if (goalEffect->particleCool >= goalEffect->particleCoolTmp && particleCount < goalEffect->particleCountMax)
	{
		goalEffect->particleCool = 0;
		particleCount++;

		for (int i = 0; i < 100; i++)
		{
			XMFLOAT4 color = { colorDist(engine),colorDist(engine) ,colorDist(engine) ,colorDist(engine) };
			XMFLOAT4 color2 = { colorDist(engine),colorDist(engine) ,colorDist(engine) ,colorDist(engine) };
			float scale = scaleDist(engine);

			ParticleManager::GetInstance()->GenerateRandomParticle(1, 300, scale * 2.0f, goalEffect->target, scale *1.3f, 0, color, color2);
		}
	}

	if (particleCount >= goalEffect->particleCountMax)
	{
		count++;

		if (count >= countMax)
		{
			goalEffect->ChangeState(new StateGoalCamera);
		}
	}
}

void StateGoalParticle::Draw()
{
}

//--------------------------------------------------------------------------------------
void StateGoalCamera::Initialize()
{
}

void StateGoalCamera::Update()
{
	goalEffect->time++;

	//tが1.0以上になったら次の区間に進む
	goalEffect->timeRate = goalEffect->time / goalEffect->timeMax;

	if (goalEffect->timeRate >= 1.0f)
	{
		if (goalEffect->index < goalEffect->poses.size() - 3)
		{
			goalEffect->index++;
			goalEffect->timeRate -= 1.0f;
			goalEffect->time = 0;
		}
		else
		{
			goalEffect->timeRate = 1.0f;

			goalEffect->isEnd = true;
		}
	}

	goalEffect->cameraM->goalEffectCamera->viewMat.eye = SplinePosition(goalEffect->poses, goalEffect->index, goalEffect->timeRate);
	goalEffect->cameraM->goalEffectCamera->viewMat.target = goalEffect->target;

	goalEffect->cameraM->goalEffectCamera->UpdateViewMatrix();
}

void StateGoalCamera::Draw()
{
}
