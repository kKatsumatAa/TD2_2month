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

void Block::Updata(Vec3 pos)
{

	//worldTransform_.trans = { pos.x,pos.y,pos.z };

	if (worldTransform_.scale.x > scaleTmp) { worldTransform_.scale.x -= 0.05f; }
	if (worldTransform_.scale.y > scaleTmp) { worldTransform_.scale.y -= 0.05f; }
	if (worldTransform_.scale.z > scaleTmp) { worldTransform_.scale.z -= 0.05f; }

	//block_.SetWorldPos(pos);
	worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera, UINT64* texhandle, int form, Action action)
{
	//���\��
	if (action == Action::Connect) { color = { 0.2f,0.1f,0.8f,0.95f }; }
	else {
		if (color.x > 1.0f) { color.x -= 0.05f; }
		if (color.x < 1.0f) { color.x += 0.05f; }
		if (color.y > 1.0f) { color.y -= 0.05f; }
		if (color.y < 1.0f) { color.y += 0.05f; }
		if (color.z > 1.0f) { color.z -= 0.05f; }
		if (color.z < 1.0f) { color.z += 0.05f; }
		if (color.w > 1.0f) { color.w -= 0.05f; }
		if (color.w < 1.0f) { color.w += 0.05f; }
	}

	if (form == Form::BLOCK) { draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color); }
	if (form == Form::BUTTON) { draw[1].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &socket_[0], color); }
	if (form == Form::GEAR) { draw[2].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &goal_[0],color); }
	if (form == Form::GOAL) {
		count++;
		if (count % 240 == 0 || count % 240 == 10 || count % 240 == 20 || count % 240 == 30)
		{
			worldTransform_.scale = { scaleTmp + scaleTmp / 4.0f,scaleTmp + scaleTmp / 4.0f ,scaleTmp + scaleTmp / 4.0f };
		}
		draw[3].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &goal_[0], color);
	}

	if (form == Form::LOCKED) { draw[2].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &button_[0], color); }

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
