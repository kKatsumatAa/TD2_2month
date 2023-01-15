#include "Block.h"

const int blockWidth = 13;
const int blockHeight = 13;

Block::~Block()
{

}

void Block::Initialize(ConnectingEffectManager* connectEM,
	Model* normal, Model* button, Model* goal, Model* socket)
{
	assert(normal);
	assert(button);
	assert(goal);
	assert(socket);

	normal_ = normal;
	button_ = button;
	goal_ = goal;
	socket_ = socket;


	//this->debugText_ = debugText_;

	this->connectEM = connectEM;

	worldTransform_.scale = { scaleTmp,scaleTmp,scaleTmp };
	worldTransform_.SetWorld();

	radius_ = scaleTmp;
}

void Block::Updata(Vec3& pos)
{

	worldTransform_.trans = { pos.x,pos.y,pos.z };


	//block_.SetWorldPos(pos);
	worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera, UINT64* texhandle, int form)
{
	//‰¼•\Ž¦
	XMFLOAT4 color;

	/*if (form == Form::BLOCK) { color = { 1.0f,1.0f,1.0f,1.0f }; }
	if (form == Form::BUTTON) { color = { 0,0,1.0f,1.0f }; }
	if (form == Form::GEAR) { color = { 1.0f,1.0f,0,1.0f }; }
	if (form == Form::GOAL) { color = { 1.0f,0,0,1.0f }; }*/

	if (form == Form::BLOCK) { draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0]); }
	if (form == Form::BUTTON) { draw[1].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &button_[0]); }
	if (form == Form::GEAR) { draw[2].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &goal_[0]); }
	if (form == Form::GOAL) { draw[3].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &socket_[0]); }

}

void Block::OnCollision(Collider& collider)
{
}

void Block::OnCollision2(Collider& collider)
{
}

void Block::SetWorldPos(Vec3& pos)
{
	worldTransform_.trans = pos;
	worldTransform_.SetWorld();
}
