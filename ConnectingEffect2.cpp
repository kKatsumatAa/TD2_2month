#include "ConnectingEffect2.h"

void ConnectingEffect2::Initialize(Vec3 pos, Vec3 startScale, Vec3 endScale
	, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime)
{
	worldTransform.trans = pos;
	worldTransform.scale = startScale;
	worldTransform.SetWorld();

	this->startScale = startScale;
	this->endScale = endScale;
	this->color = color;
	this->startColor = startColor;
	this->endColor = endColor;

	this->lifeTime = lifeTime;
}

void ConnectingEffect2::Update()
{
	deathTime++;

	float t = (float)deathTime / (float)lifeTime;

	worldTransform.scale = LerpVec3(startScale, endScale, EaseOut(t));

	Vec3 colorRGB = LerpVec3({ startColor.x,startColor.y,startColor.z }, { endColor.x,endColor.y,endColor.y }, t);
  	float colorA = LerpVec3({ startColor.w,0,0 }, { endColor.w,0,0 }, t).x;
	color = { colorRGB.x,colorRGB.y,colorRGB.z,colorA };

	worldTransform.SetWorld();

	if (deathTime >= lifeTime)
	{
		isAlive = false;
	}
}

void ConnectingEffect2::Draw(Camera& camera)
{
	obj.DrawCube3D(&worldTransform, &camera.viewMat, &camera.projectionMat, color);
}


//-----------------------------------------------------------------------------------------------
void ConnectingEffect2Manager::Initialize()
{
	connectingEffect2s_.clear();
}

void ConnectingEffect2Manager::GenerateConnectingEffect2(Vec3 pos, Vec3 startScale, Vec3 endScale
	, XMFLOAT4 startColor, XMFLOAT4 endColor, int lifeTime)
{
	//ê∂ê¨ÅAèâä˙âª
	std::unique_ptr<ConnectingEffect2> connectingEffect = std::make_unique<ConnectingEffect2>();
	connectingEffect->Initialize(pos, startScale, endScale, startColor, endColor, lifeTime);
	//ìoò^
	connectingEffect2s_.push_back(std::move(connectingEffect));
}

void ConnectingEffect2Manager::Update()
{
	for (std::unique_ptr<ConnectingEffect2>& connectE : connectingEffect2s_)
	{
		connectE->Update();
	}

	connectingEffect2s_.remove_if([](std::unique_ptr<ConnectingEffect2>& connectE)
		{
			return (!connectE->GetIsAlive());
		}
	);
}

void ConnectingEffect2Manager::Draw(Camera* camera)
{
	for (std::unique_ptr<ConnectingEffect2>& connectE : connectingEffect2s_)
	{
		connectE->Draw(*camera);
	}
}
