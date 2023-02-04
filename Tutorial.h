#pragma once
#include"Object.h"

static enum TUTORIAL
{
	MOVE,     //移動
	CONNECT,  //繋ぐ
	TURN,     //回転
	OVERLAP,  //重なる
	CONNECT_LIMIT,  //限度
	BUTTON,   //ボタン
	ELECTRIC, //電気ブロック
	LAST      //終わり
};


class Tutorial;

class TutorialState
{
protected:
	int num = 0;
	Tutorial* tutorial;
	float count = 0;

public:
	void SetTutorial(Tutorial* tutorial) { this->tutorial = tutorial; }
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void AddNum() = 0;
	virtual int GetNum() = 0;
	virtual int GetMaxNum() = 0;
};


class Tutorial
{
private:
	//シンプルなステート（処理用
	TutorialState* state = nullptr;
	//外部から今行ってるチュートリアルが分かるようにした変数
	int state2 = MOVE;
	bool isEnd = false;
	float count = 0;



	int skipTimer = 0;
	static const int skipMaxTime = 40;

public:
	UINT64 texhandle[10];
	Object sprite[5];

	int spriteCount = 0;
	const int spriteCountMax = 30;
	float spriteSize = 0;
	const float spriteSizeMax = 0.58f;
	float spriteRot = 0;
	const float spriteRotMax = 360.0f;


	void Update();
	//最初のチュートリアルの
	void Initialize();
	//
	void ConnectLimitInitialize();
	//
	void OverlapInitialize();
	//ボタンのチュートリアル
	void ButtonInitialize();
	//電気ブロックのチュートリアル
	void ElectricInitialize();

	void InitializeCommon();

	void Draw();

	int GetState() { return state2; }
	void AddState2() { state2++; spriteCount = 0; }
	void AddStateNum() { state->AddNum(); }

	int GetStateNum() { return state->GetNum(); }
	int GetStateNumMax() { return state->GetMaxNum(); }

	bool GetIsEnd() { return isEnd; }

	void SetIsEnd(bool isEnd) { this->isEnd = isEnd; }

	void ChangeState(TutorialState* state)
	{
		delete this->state;
		this->state = state;
		state->SetTutorial(this);
	}
};


//----------------------------------------------------------
class MoveTutorial :public TutorialState
{
private:
	const int numMax = 3;
	int texhandle[5];
	Object sprite[5];

public:
	MoveTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//繋ぐ
class ConnectTutorial : public TutorialState
{
private:
	const int numMax = 1;
	int texhandle[5];
	Object sprite[5];

public:
	ConnectTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//モード
class TurnTutorial : public TutorialState
{
private:

	const int numMax = 3;
	int texhandle[5];
	Object sprite[5];

public:
	TurnTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//重なり
class OverlapTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Object sprite[5];

public:
	OverlapTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

// 限度
class ConnectLimitTutorial : public TutorialState
{
private:

	const int numMax = 4;
	int texhandle[5];
	Object sprite[5];

public:
	ConnectLimitTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//ボタン
class ButtonTutorial : public TutorialState
{
private:

	const int numMax = 2;
	int texhandle[5];
	Object sprite[5];

public:
	ButtonTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//電気ブロック
class ElectricTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Object sprite[5];

public:
	ElectricTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};

//
class LastTutorial : public TutorialState
{
private:

	const int numMax = 1;
	int texhandle[5];
	Object sprite[5];

public:
	LastTutorial();


	void AddNum()override { num++; }
	int GetNum()override { return num; }
	int GetMaxNum()override { return numMax; }

	void Update() override;
	void Draw() override;
};