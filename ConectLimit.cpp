#include "ConectLimit.h"

void ConectLimit::Initialize()
{
	if (texhandle == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/UI/UI_Number2.png", texhandle);
	}

	drawNum.Initialize(texhandle);
}

void ConectLimit::Update(Vec3 pos, Camera* camera)
{
	Vec2 posNum = Vec3toVec2(pos, camera->viewMat.matView, camera->projectionMat.matProjection);

	drawNum.SetNum(this->drawConnectCount, posNum, { 0.1f, 1.0f }, { 64.0f,64.0f }, 1.0f, { 1.0f,1.0f,1.0f,0.7f });
}

void ConectLimit::Draw()
{
	drawNum.Draw();
}
