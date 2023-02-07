#include "RockOnImage.h"

RockOnImage& RockOnImage::operator=(const RockOnImage& obj)
{
	isGoalConnect = obj.isGoalConnect;
	isGoalConnectCount = obj.isGoalConnectCount;
	isGoalConnectScale = obj.isGoalConnectScale;
	isGoalConnectRot = obj.isGoalConnectRot;

	pos = obj.pos;

	texhandle = obj.texhandle;

	count = obj.count;

	return *this;
}

void RockOnImage::Initialize()
{
	isGoalConnect = false;
	isGoalConnectCount = 0;
	isGoalConnectScale = 1.0f;
	isGoalConnectRot = 0.0f;

	count = 0;

	if (texhandle == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/UI_LockOn.png", texhandle);
	}

	pos = { 0,0,0 };
}

void RockOnImage::Update(/*Vec3 pos*/)
{
	if (isGoalConnectCount < isGoalConnectCountTmp)
	{
		isGoalConnectCount++;
		float t = (float)isGoalConnectCount / (float)isGoalConnectCountTmp;

		isGoalConnectRot = LerpVec3({ 180.0f * 2.5f,0,0 }, { 0,0,0 }, EaseIn(t)).x;
		isGoalConnectScale = LerpVec3({ 0.5f,0,0 }, { 0.1f,0,0 }, EaseIn(t)).x;
	}
	else
	{
		count++;

		isGoalConnectScale = 0.1f + fabs(sinf(count * 0.05f)) * 0.03f;
	}
}

void RockOnImage::Draw(Camera* camera)
{
	if (isGoalConnect)
	{
		Vec2 pos = Vec3toVec2(this->pos, camera->viewMat.matView, camera->projectionMat.matProjection);

		obj.DrawBoxSprite({ pos.x,pos.y,0 }, this->isGoalConnectScale, { 1.0f,1.0f,1.0f,0.9f }, texhandle, { 0.5f,0.5f }, false, false, this->isGoalConnectRot);
	}
}

void RockOnImage::BeginEffect(Vec3 pos)
{
	isGoalConnect = true;
	isGoalConnectCount = 0;

	this->pos = pos;
}
