#pragma once
#include"Collider.h"
#include "ConnectingEffect2.h"

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

//何もしていない
class StateNormalSocket : public PlayerSocketState
{
private:

public:
	void Update(Vec3 pos/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//さす途中
class StatePlugInSocket : public PlayerSocketState
{
private:
	int count = 0;
	const int countMax = 20;

public:
	void Update(Vec3 pos/*Tutorial* tutorial = nullptr*/);
	void Draw(Camera* camera, Model* model);
};

//さしている
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

	ConnectingEffect2Manager* connectE2M;

	float blockRadius = 0;


public:
	PlayerSocket(){}
	PlayerSocket(const PlayerSocket& obj) {};

	void Initialize(ConnectingEffect2Manager* connectE2M, float blockRadius, Model* model);

	void Update(Vec3 pos);
	void Draw(Camera* camera);

	//使い始める
	void UseSocket(Vec3 endPos);
	//抜く
	void FinishSocket(Vec3 playerPos);

	bool GetIsUsed() { return isUsed; }
	void SetIsUsed(bool is) { isUsed = is; }

	~PlayerSocket();

	void ChangeState(PlayerSocketState* state);


	//
	void OnCollision(Collider& collider)override;
	//
	void OnCollision2(Collider& collider)override;


	PlayerSocket& operator=(const PlayerSocket& obj);
};

