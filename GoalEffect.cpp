#include "GoalEffect.h"
#include "ParticleManager.h"

//—””ÍˆÍ
static std::uniform_real_distribution<float> scaleDist(0.3f, 2.0f);
static std::uniform_real_distribution<float> colorDist(0.1f, 1.0f);


void GoalEffect::ChangeState(GoalEffectState* state)
{
	delete this->state;
	this->state = state;
	state->SetGoalEffect(this);
	this->state->Initialize();
}

void GoalEffect::Initialize()
{
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

void GoalEffect::BegineGoalEffect(std::vector<Vec3> poses, Vec3 target, int time)
{
	//ˆê”ÔÅ‰‚ÆÅŒã‚É“¯‚¶‚Ì‚ğ“ü‚ê‚éˆ—‚ğÈ‚­‚½‚ß
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

	ChangeState(new StateGoalParticle);

}


//-----------------------------------------------------------------------------------
void GoalEffectState::SetGoalEffect(GoalEffect* goalEffect)
{
	this->goalEffect = goalEffect;
}

//-----------------------------------------------------------------------------------
void StateGoalParticle::Initialize()
{
	for (int i = 0; i < 20; i++)
	{
		XMFLOAT4 color = { colorDist(engine),colorDist(engine) ,colorDist(engine) ,colorDist(engine) };
		XMFLOAT4 color2 = { colorDist(engine),colorDist(engine) ,colorDist(engine) ,colorDist(engine) };
		float scale = scaleDist(engine);

		ParticleManager::GetInstance()->GenerateRandomParticle(5, 300, scale * 4.0f, goalEffect->target, scale / 1.2f, 0, color, color2);
	}

	//ƒJƒƒ‰‚ÌˆÊ’u‚¾‚¯“ü‚ê‚é
	goalEffect->goalEffectCamera.viewMat.eye = goalEffect->poses[0];
	goalEffect->goalEffectCamera.viewMat.target = goalEffect->target;

	goalEffect->goalEffectCamera.UpdateViewMatrix();
}

void StateGoalParticle::Update()
{
	count++;

	if (count >= countMax)
	{
		goalEffect->ChangeState(new StateGoalCamera);
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

	//t‚ª1.0ˆÈã‚É‚È‚Á‚½‚çŸ‚Ì‹æŠÔ‚Éi‚Ş
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
			//poses.clear();

		}
	}

	goalEffect->goalEffectCamera.viewMat.eye = SplinePosition(goalEffect->poses, goalEffect->index, goalEffect->timeRate);
	goalEffect->goalEffectCamera.viewMat.target = goalEffect->target;

	goalEffect->goalEffectCamera.UpdateViewMatrix();
}

void StateGoalCamera::Draw()
{
}
