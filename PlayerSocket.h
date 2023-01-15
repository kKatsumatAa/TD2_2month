#pragma once
#include"Collider.h"

class PlayerSocket;

class PlayerSocketState
{
protected:
	PlayerSocket* playerSocket = nullptr;

public:
	void SetPlayerSocket(PlayerSocket* playerSocket);
	virtual void Update(Vec3 pos) = 0;
	virtual void Draw(Camera* camera, Model* model) = 0;
};

//‰½‚à‚µ‚Ä‚¢‚È‚¢
class StateNormalSocket : public PlayerSocketState
{
private:

public:
	void Update(Vec3 pos/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//‚³‚·“r’†
class StatePlugInSocket : public PlayerSocketState
{
private:
	int count = 0;
	const int countMax = 20;

public:
	void Update(Vec3 pos/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//‚³‚µ‚Ä‚¢‚é
class StateUsingSocket : public PlayerSocketState
{
private:
	int count = 0;
	const int countMax = 20;

	int effectCount = 0;

public:
	void Update(Vec3 pos/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//---------------------------------------------------------

class PlayerSocket : public Collider
{
private:
	Model* model;
	Object* obj;

	bool isUsed = false;

	PlayerSocketState* state;

public:
	Vec3 startPos;
	Vec3 endPos;

	Vec3 plugInPosTmp;


public:
	void Initialize(Model* model);

	void Update(Vec3 pos);
	void Draw(Camera* camera);

	//Žg‚¢Žn‚ß‚é
	void UseSocket(Vec3 endPos);
	//”²‚­
	void FinishSocket(Vec3 playerPos);

	bool GetIsUsed() { return isUsed; }
	void SetIsUsed(bool is) { isUsed = is; }

	void ChangeState(PlayerSocketState* state);


	//
	void OnCollision(Collider& collider)override;
	//
	void OnCollision2(Collider& collider)override;
};

