#include "ConectLimit.h"

void ConectLimit::Initialize()
{
	if (texhandle == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/UI/UI_Number2.png", texhandle);
	}

	drawNum.Initialize(texhandle);


	effectCount = 0;
	effectCountTmp = 0;

	effectAddScale = 0;
	effectAddScaleTmp = 0;

	effectAddColor = {0,0,0,0};
	effectAddColorTmp = { 0,0,0,0 };
}

void ConectLimit::Update(Vec3 pos, Camera* camera)
{
	float scale = 1.0f;
	XMFLOAT4 color = { 1.0f,1.0f,1.0f,0.7f };

	//
	if (effectCount > 0)
	{
		float t = (float)effectCount / (float)effectCountTmp;

		scale = effectAddScale * EaseOut(t);
		color.x = effectAddColor.x * EaseOut(t);
		color.y = effectAddColor.y * EaseOut(t);
		color.z = effectAddColor.z * EaseOut(t);
		color.w = effectAddColor.w * EaseOut(t);

		effectCount--;
	}


	Vec2 posNum = Vec3toVec2(pos, camera->viewMat.matView, camera->projectionMat.matProjection);

	drawNum.SetNum(this->drawConnectCount, posNum, { 0.1f, 1.0f }, { 64.0f,64.0f }, scale, color);
}

void ConectLimit::Draw()
{
	drawNum.Draw();
}

void ConectLimit::BeginNumEffect(int time, float addScale, XMFLOAT4 addColor)
{
	effectCount = time;
	effectCountTmp = time;
	effectAddScale = addScale;
	effectAddColor = addColor;
}
