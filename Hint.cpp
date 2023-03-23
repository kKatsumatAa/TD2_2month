#include "Hint.h"
#include "KeyboardInput.h"

void HintState::SetHint(Hint* hint)
{
	this->hint = hint;
}


//--------------------------------------
//表示していない
void HintStateNormal::Update()
{
	//外部でヒントフラグがたったら
	if (hint->GetIsDisplayingHint())
	{
		Object::effectFlags.isGrayScale = true;

		this->timer++;

		//一定時間たったら
		if (timer >= timerMax)
		{
			//音
			Sound::GetInstance().StopWave("Stage_BGM.wav");
			Sound::GetInstance().StopWave("tutu4.wav");
			Sound::GetInstance().PlayWave("hint.wav", 1.5f, true);
			hint->ChangeState(new HintStateDisplay);
		}
	}
}

void HintStateNormal::Draw()
{
	//割合
	t = (float)timer / (float)timerMax;

	//画面サイズまで大きくなるまで表示する（ステート矛盾してね！？）
	if (hint->GetIsDisplayingHint())
	{
		object.DrawBoxSprite({ WindowsApp::GetInstance().window_width / 2.0f,WindowsApp::GetInstance().window_height / 2.0f,0 },
			t, { 1.0f,1.0f,1.0f,t }, hint->GetTexHandle()[hint->GetStageNum()], { 0.5f,0.5f });
	}
}

//--------------------------------------
//表示中
void HintStateDisplay::Update()
{

	//外部で表示フラグがオフになったら
	if (!hint->GetIsDisplayingHint())
	{
		

		timer++;

		if (timer >= timerMax)
		{
			Object::effectFlags.isGrayScale = false;

			//音
			Sound::GetInstance().StopWave("hint.wav");
			Sound::GetInstance().PlayWave("Stage_BGM.wav", 0.5f, true);
			//Sound::GetInstance().PlayWave("tutu4.wav", 0.25f, true);

			hint->ChangeState(new HintStateNormal);
		}
	}
}

void HintStateDisplay::Draw()
{
	//割合
	t = 1.0f - (float)timer / (float)timerMax;

	object.DrawBoxSprite({ WindowsApp::GetInstance().window_width / 2.0f,WindowsApp::GetInstance().window_height / 2.0f,0 },
		t, { 1.0f,1.0f,1.0f,t }, hint->GetTexHandle()[hint->GetStageNum()], { 0.5f,0.5f });
}


//-----------------------------------------------------------------------------------------
Hint& Hint::GetInstance()
{
	static Hint inst;
	return inst;
}

void Hint::Initialize()
{
	if (texHandle[0] == NULL)
	{
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/tutorial.png", texHandle[0]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/1.png", texHandle[1]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/2.png", texHandle[2]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/3.png", texHandle[3]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/4.png", texHandle[4]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/5.png", texHandle[5]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/6.png", texHandle[6]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/7.png", texHandle[7]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/8.png", texHandle[8]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/9.png", texHandle[9]);
		TextureManager::GetInstance().LoadGraph(L"Resources/image/hint/10.png", texHandle[10]);
	}

	isDisplayingHint = false;

	//stageNum = 0;

	ChangeState(new HintStateNormal);
}

void Hint::Update()
{
	if (KeyboardInput::GetInstance().KeyTrigger(DIK_C))
	{
		if (isDisplayingHint) { isDisplayingHint = false; }
		else { isDisplayingHint = true; }
	}

	state->Update();
}

void Hint::Draw()
{
	state->Draw();
}

void Hint::ChangeState(HintState* state)
{
	//もともと使ってたステートを削除
	delete this->state;
	//新しいステートを入れる
	this->state = state;
	//ステートのインスタンスに自分のインスタンスを持たせる
	this->state->SetHint(this);
}
