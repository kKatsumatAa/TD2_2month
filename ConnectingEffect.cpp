#include "ConnectingEffect.h"


void ConnectingEffect::Initialize(Vec3 pos, Vec3 rot, float length, int lifeTime, float t)
{
	worldTransform.rot = rot;
	worldTransform.trans = pos;
	//zÇ…ïœÇ¶ÇÈâ¬î\ê´Ç†ÇË
	worldTransform.scale = { 0,0.05f,0.05f };
	scaleTmp = length;
	worldTransform.SetWorld();

	this->lifeTime = lifeTime;

	this->t = t;
}

void ConnectingEffect::Update()
{
	deathTime++;

	worldTransform.scale.x = LerpVec3({ scaleTmp,0,0 }, { 0,0,0 }, EaseOut((float)deathTime / (float)lifeTime) * (/*1.0 - */t)).x;
	worldTransform.SetWorld();

	if (deathTime >= lifeTime)
	{
		isAlive = false;
	}
}

void ConnectingEffect::Draw(Camera& camera)
{
	obj.DrawCube3D(&worldTransform, &camera.viewMat, &camera.projectionMat, { 1.0f,1.0f,0.0f,1.0f });
}


//----------------------------------------------------------------------------------------------------------
void ConnectingEffectSet::GenerateConnectingEffect(Vec3 pos, Vec3 rot, float length, int lifeTime, float t)
{
	//ãÖÇê∂ê¨ÅAèâä˙âª
	std::unique_ptr<ConnectingEffect> connectE = std::make_unique<ConnectingEffect>();
	connectE->Initialize(pos, rot, length, lifeTime, t);
	//ãÖÇìoò^
	connectingEffects.push_back(std::move(connectE));
}

void ConnectingEffectSet::GenerateRandomConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int numMax)
{
	static std::uniform_real_distribution<float> posDist(-radius, radius);
	static std::uniform_real_distribution<float> lengthDist(lengthMax / 2.0f, lengthMax);
	static std::uniform_real_distribution<float> lifeTimeDist(0, (float)lifeTimeMax / (float)numMax);
	static std::uniform_real_distribution<float> rotDist(0, pi);

	int lifeTimeTmp = lifeTimeDist(engine);

	for (int i = 0; i < numMax; i++)
	{
		if (i == 0)
		{
			oldPos = { pos.x + posDist(engine),pos.y + posDist(engine),pos.z + posDist(engine) };
			oldRot = { rotDist(engine),rotDist(engine),rotDist(engine) };
			oldLength = lengthDist(engine);

			GenerateConnectingEffect(oldPos, oldRot, oldLength, lifeTimeTmp, (float)i / (float)numMax);
		}
		else
		{
			Vec3 vec = { 1.0f,0,0 };

			vec *= oldLength;

			WorldMat worldTransform;
			worldTransform.rot = oldRot;
			worldTransform.SetWorld();
			Vec3xM4(vec, worldTransform.matWorld, 0);
			oldPos += vec;


			/*vec = { 1.0f,0,0 };
			oldLength = lengthDist(engine);
			vec *= oldLength;*/

			oldRot = { rotDist(engine),rotDist(engine),rotDist(engine) };
			worldTransform.rot = oldRot;
			worldTransform.SetWorld();

			vec = { 1.0f,0,0 };
			Vec3xM4(vec, worldTransform.matWorld, 0);

			oldPos += vec;

			GenerateConnectingEffect(oldPos, oldRot, oldLength, lifeTimeTmp, (float)i / (float)numMax);
		}
	}
}

void ConnectingEffectSet::Initialize(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num, int countMax)
{
	count = 0;
	this->countMax = countMax;
	isEnd = false;

	oldPos = pos;
	oldLength = lengthMax;
	this->radius = radius;
	lifeTime = lifeTimeMax;
	this->num = num;

	GenerateRandomConnectingEffect(pos, radius, lengthMax, lifeTimeMax, countMax);

	//ç≈èâÇÃÉ|ÉCÉìÉ^
	itr = connectingEffects.begin();

}

void ConnectingEffectSet::Update()
{
	if (!isFinal)
	{
		if (itr->get()->GetIsAlive())
		{
			itr->get()->Update();
		}
		else if (count < countMax)
		{
			itr++;
			count++;
		}

		if (count >= countMax)
		{
			isFinal = true;
			itr = connectingEffects.begin();
		}
	}

	if (isFinal)
	{
		connectingEffects.erase(itr);
		itr = connectingEffects.begin();

		if (connectingEffects.size() <= 0)
		{
			isEnd = true;
		}
	}
}


void ConnectingEffectSet::Draw(Camera& camera)
{
	for (std::unique_ptr<ConnectingEffect>& connectEffects : this->connectingEffects)
	{
		connectEffects->Draw(camera);
	}
}

void ConnectingEffectSet::PushBackConnectEffect(ConnectingEffect* connectE)
{
}


//----------------------------------------------------------------------------------------------------------

void ConnectingEffectManager::Initialize()
{
	connectingEffects_.clear();
}

void ConnectingEffectManager::Update()
{
	for (std::unique_ptr<ConnectingEffectSet>& connectEffects : connectingEffects_)
	{
		connectEffects->Update();
	}

	connectingEffects_.remove_if([](std::unique_ptr<ConnectingEffectSet>& connect)
		{
			return connect->GetIsEnd();
		}
	);
}

void ConnectingEffectManager::Draw(Camera& camera)
{
	for (std::unique_ptr<ConnectingEffectSet>& connectEffects : connectingEffects_)
	{
		connectEffects->Draw(camera);
	}
}

void ConnectingEffectManager::GenerateConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num, int count)
{
	{
		//ãÖÇê∂ê¨ÅAèâä˙âª
		std::unique_ptr<ConnectingEffectSet> connectSet = std::make_unique<ConnectingEffectSet>();
		connectSet->Initialize(pos, radius, lengthMax, lifeTimeMax, num, count);
		//ãÖÇìoò^
		connectingEffects_.push_back(std::move(connectSet));
	}
}

void ConnectingEffectManager::GenerateRandomConnectingEffect(Vec3 pos, float radius, float lengthMax, int lifeTimeMax, int num, int count)
{
	static std::uniform_int_distribution<int> numDist(0, num);
	num = numDist(engine);

	GenerateConnectingEffect(pos, radius, lengthMax, lifeTimeMax, num, count);
}


