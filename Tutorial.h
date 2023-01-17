#pragma once
#include"Object.h"

static enum TUTORIAL
{
	MOVE,     //移動
	CONNECT,  //繋ぐ
	TURN,     //回転
	OVERLAP,  //重なる
	LAST      //終わり
};

static float width = 768/2;
static float height = 319;

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

	void Update();
	void Initialize();
	void Draw();

	int GetState() { return state2; }
	void AddState2() { state2++; }
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