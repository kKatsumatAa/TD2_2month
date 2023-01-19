#include "Tutorial.h"
#include "TextureManager.h"

void Tutorial::Initialize()
{
	//外部から今行ってるチュートリアルが分かるようにした変数
	state2 = MOVE;
	isEnd = false;
	count = 0;
	skipTimer = 0;

	if (texhandle[0] == NULL)
	{
		TextureManager::LoadGraph(L"Resources/image/Explanation/Movement_Explanation.png", texhandle[MOVE]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Connecting.png", texhandle[CONNECT]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Rotate.png", texhandle[TURN]);
		TextureManager::LoadGraph(L"Resources/image/Explanation/Explanation_Overlapping.png", texhandle[OVERLAP]);
		TextureManager::LoadGraph(L"Resources/image/restart.png", texhandle[OVERLAP + 1]);
	}


	ChangeState(new MoveTutorial);
}

void Tutorial::Update()
{
	state->Update();
}

void Tutorial::Draw()
{
	state->Draw();

	sprite[0].DrawBoxSprite({ 10,10,0 }, 1.0f, { 1.0f,1.0f,1.0f,1.0f }, texhandle[OVERLAP + 1]);
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
	count += 0.1f;

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.25f,30.0f + sinf(count) * 2.4f,0 }, 0.58f, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[MOVE]);
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
	count += 0.1f;

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.25f,30.0f + sinf(count) * 2.4f,0 }, 0.58f, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[CONNECT]);
}

//------------------------------------------------------------------
TurnTutorial::TurnTutorial()
{

}

void TurnTutorial::Update()
{
	if (num >= numMax)
	{
		tutorial->AddStateNum();
		tutorial->AddState2();
		tutorial->ChangeState(new OverlapTutorial);
	}
}

void TurnTutorial::Draw()
{
	count += 0.1f;

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.25f,30.0f + sinf(count) * 2.4f,0 }, 0.58f, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[TURN]);
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
	count += 0.1f;

	sprite[0].DrawBoxSprite({ WindowsApp::GetInstance().window_width / 3.0f * 2.25f,30.0f + sinf(count) * 2.4f,0 }, 0.58f, { 1.0f,1.0f,1.0f,1.0f }, tutorial->texhandle[OVERLAP]);
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
	count += 0.1f;

}