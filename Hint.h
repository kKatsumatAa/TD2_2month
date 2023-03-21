#pragma once
#include"StageManager.h"


class Hint;

//ステート親クラス
class HintState
{
protected:
	Hint* hint = nullptr;
	//
	Object object;

	//タイマー系
	int timer = 0;
	const int timerMax = 20;
	float t = 0;

public:
	void SetHint(Hint* hint);
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

//表示されてない　ステート子クラス
class HintStateNormal : public HintState
{
private:

public:
	void Update()override;
	void Draw()override;
};

//表示している　ステート子クラス
class HintStateDisplay : public HintState
{
private:


public:
	void Update()override;
	void Draw()override;
};


class Hint final
{
	//変数
private:
	UINT64 texHandle[StageManager::stageMax];

	//表示中か
	bool isDisplayingHint = false;

	int stageNum = 0;

	//ステート
	HintState* state = nullptr;

public:



	//関数
private:
	Hint() { ; }
	~Hint() { ; }
public:
	//コピーコンストラクタを無効
	Hint(const Hint& obj) = delete;
	//代入演算子も
	Hint& operator=(const Hint& obj) = delete;

	static Hint& GetInstance();

public:
	void Initialize();
	void Update();
	void Draw();

	//現在のステージを指定
	void SetStageNum(const int& num) { this->stageNum = num; }

	//ヒントを表示中かどうか
	bool GetIsDisplayingHint() { return isDisplayingHint; }
	//
	int GetStageNum() { return stageNum; }

	//ステート変更
	void ChangeState(HintState* state);

	UINT64* GetTexHandle() { return texHandle; }
};

