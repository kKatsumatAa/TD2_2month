#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = TUTORIAL::MOVE;

	InitializeCommon();

	ChangeState(new MoveTutorial);
}

void Tutorial::OverlapInitialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = TUTORIAL::OVERLAP;

	InitializeCommon();

	ChangeState(new OverlapTutorial);
}

void Tutorial::ConnectLimitInitialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = TUTORIAL::CONNECT_LIMIT;

	InitializeCommon();

	ChangeState(new ConnectLimitTutorial);
}


void Tutorial::ButtonInitialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = TUTORIAL::BUTTON;

	InitializeCommon();

	ChangeState(new ButtonTutorial);
}

void Tutorial::ElectricInitialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = TUTORIAL::ELECTRIC;

	InitializeCommon();

	ChangeState(new ElectricTutorial);
}

void Tutorial::InitializeCommon()
{
	isEnd = false;
	count = 0;
	skipTimer = 0;
	spriteSize = 0;
	spriteRot = 0;
	spriteCount = 0;
	if (texhandle[0] == NULL)
	{
		TextureManager::LoadGraph(L"Resources/image/Explanation/Movement_Explanation.png", texhandle[MOVE]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Connecting.png", texhandle[CONNECT]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Rotate.png", texhandle[TURN]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Overlapping.png", texhandle[OVERLAP]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Tutorial_Consume.png", texhandle[CONNECT_LIMIT]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Tutorial_Exhaust.png", texhandle[CONNECT_LIMIT + 1]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanartion_Button.png", texhandle[BUTTON + 1]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanartion_Goal.png", texhandle[BUTTON + 2]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanartion_ElectlicBlock.png", texhandle[ELECTRIC + 2]);
	}
}

void Tutorial::Update()
{
	if (spriteCount < spriteCountMax)
	{
		spriteCount++;
		spriteSize = LerpVec3({ 0,0,0 }, { spriteSizeMax,0,0 }, EaseIn((float)spriteCount / (float)spriteCountMax)).x;
		spriteRot = LerpVec3({ 0,0,0 }, { spriteRotMax,0,0 }, EaseIn((float)spriteCount / (float)spriteCountMax)).x;
	}

	state->Update();
}

void Tutorial::Draw()
{
	state->Draw();
}


//-----------------------------------------------------------
MoveTutorial::MoveTutorial()
{

}

void MoveTutorial::Update()
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new ConnectTutorial);
	}
}

void MoveTutorial::Draw()
{
	/* count += 0.1f; */

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[MOVE],
		{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
}

//-------------------------------------------------------------------------------
ConnectTutorial::ConnectTutorial()
{

}

void ConnectTutorial::Update()
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new TurnTutorial);
	}
}

void ConnectTutorial::Draw()
{
	/* count += 0.1f; */

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[CONNECT],
		{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
}

//------------------------------------------------------------------
TurnTutorial::TurnTutorial()
{

}

void TurnTutorial::Update()
{
	/*if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new ConnectLimitTutorial);
	}*/
}

void TurnTutorial::Draw()
{
	/* count += 0.1f; */

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[TURN],
		{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
}


//------------------------------------------------------------------------------------
ConnectLimitTutorial::ConnectLimitTutorial()
{

}

void ConnectLimitTutorial::Update()
{
	//if (num >= numMax)
	//{
	//	//num = numMax - 1;
	//	tutorial->AddStateNum();
	//	tutorial->AddState2();
	//	//tutorial->ChangeState(new OverlapTutorial);
	//}
}

void ConnectLimitTutorial::Draw()
{
	/* count += 0.1f; */

	if (num == 0)
	{
		sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[CONNECT_LIMIT],
			{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
	}
	else
	{
		sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[CONNECT_LIMIT + 1],
			{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
	}
}

//-----------------------------------------------------------------------
OverlapTutorial::OverlapTutorial()
{

}

void OverlapTutorial::Update()
{
	if (num >= numMax)
	{
		//num = numMax - 1;
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LastTutorial);
	}
}

void OverlapTutorial::Draw()
{
	/* count += 0.1f; */

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[OVERLAP],
		{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
}

//-------------------------------------------------------------------------------------
ButtonTutorial::ButtonTutorial()
{

}

void ButtonTutorial::Update()
{
	//if (num >= numMax)
	//{
	//	//num = numMax - 1;
	//	tutorial->AddStateNum();
	//	tutorial->AddState2();
	//	tutorial->ChangeState(new ElectricTutorial);
	//}
}

void ButtonTutorial::Draw()
{
	/* count += 0.1f; */

	/*if (num == 0)*/
	{
		sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[BUTTON + 1],
			{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
	}
	/*else
	{
		sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[BUTTON + 2],
			{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
	}*/
}


//-------------------------------------------------------------------------------------
ElectricTutorial::ElectricTutorial()
{

}

void ElectricTutorial::Update()
{
	if (num >= numMax)
	{
		//num = numMax - 1;
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new LastTutorial);
	}
}

void ElectricTutorial::Draw()
{
	/* count += 0.1f; */

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.5f,180.0f + sinf(count) * 2.0f,0 }, tutorial->spriteSize, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[ELECTRIC + 2],
		{ 0.5f,0.5f }, false, false, tutorial->spriteRot);
}


//----------------------------------------------------------------------------
LastTutorial::LastTutorial()
{

}

void LastTutorial::Update()
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->SetIsEnd(true);
	}
}

void LastTutorial::Draw()
{
	/* count += 0.1f; */

}


