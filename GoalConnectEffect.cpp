#include "GoalConnectEffect.h"


void GoalConnectEffect::Initialize(Vec3 pos, Vec3 startScale, Vec3 endScale
	, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime, Vec3 startAngle, Vec3 endAngle)
{
	worldTransform.trans = pos;
	worldTransform.scale = startScale;
	worldTransform.SetWorld();

	this->startScale = startScale;
	this->endScale = endScale;
	this->color = color;
	this->startColor = startColor;
	this->endColor = endColor;
	this->startAngle = startAngle;
	this->endAngle = endAngle;

	this->lifeTime = lifeTime;
}

void GoalConnectEffect::Update()
{
	deathTime++;

	float t = (float)deathTime / (float)lifeTime;

	worldTransform.scale = LerpVec3(startScale, endScale, EaseOut(t));

	worldTransform.rot = LerpVec3(startAngle, endAngle, EaseOut(t));

	Vec3 colorRGB = LerpVec3({ startColor.x,startColor.y,startColor.z }, { endColor.x,endColor.y,endColor.y }, t);
	float colorA = LerpVec3({ startColor.w,0,0 }, { endColor.w,0,0 }, t).x;
	color = { colorRGB.x,colorRGB.y,colorRGB.z,colorA };

	worldTransform.SetWorld();

	if (deathTime >= lifeTime)
	{
		isAlive = false;
	}
}

void GoalConnectEffect::Draw(Camera& camera)
{
	obj.DrawCube3D(&worldTransform, &camera.viewMat, &camera.projectionMat, color, NULL, 1);
}


//-----------------------------------------------------------------------------------------------
void GoalConnectEffectManager::Initialize()
{
	goalConnectEffects_.clear();
}

void GoalConnectEffectManager::GenerateGoalConnectEffect(Vec3 pos, Vec3 startScale, Vec3 endScale
	, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime, Vec3 startAngle, Vec3 endAngle)
{
	//ê∂ê¨ÅAèâä˙âª
	std::unique_ptr<GoalConnectEffect> connectingEffect = std::make_unique<GoalConnectEffect>();
	connectingEffect->Initialize(pos, startScale, endScale, startColor, endColor, lifeTime, startAngle, endAngle);
	//ìoò^
	goalConnectEffects_.push_back(std::move(connectingEffect));
}

void GoalConnectEffectManager::Update()
{
	for (std::unique_ptr<GoalConnectEffect>& connectE : goalConnectEffects_)
	{
		connectE->Update();
	}

	goalConnectEffects_.remove_if([](std::unique_ptr<GoalConnectEffect>& connectE)
		{
			return (!connectE->GetIsAlive());
		}
	);
}

void GoalConnectEffectManager::Draw(Camera* camera)
{
	for (std::unique_ptr<GoalConnectEffect>& connectE : goalConnectEffects_)
	{
		connectE->Draw(*camera);
	}
}
