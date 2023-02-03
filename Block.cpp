#include "Block.h"
#include "ParticleManager.h"

const int blockWidth = 13;
const int blockHeight = 13;

Block::~Block()
{

}

void Block::Initialize(ConnectingEffectManager* connectEM,
	Model* normal, Model* locked, Model* goal, Model* socket,Model* button,Model* disconnectedBlock, 
	Model* disconnectedButton, Model *disconnectedSocketBlock, Model* electricBlock, Model *doorGoalClosed,
	Model* overlapBlock)
{
	assert(normal);
	assert(locked);
	assert(button);
	assert(goal);
	assert(socket);
	assert(overlapBlock);

	normal_ = normal;
	locked_ = locked;
	button_ = button;
	goal_ = goal;
	socket_ = socket;
	disconnectedBlock_ = disconnectedBlock;
	disconnectedButton_ = disconnectedButton;
	disconnectedSocketBlock_ = disconnectedSocketBlock;
	electricBlock_ = electricBlock;
	doorGoalClosed_ = doorGoalClosed;
	overlapBlock_ = overlapBlock;

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
	if (worldTransform_.scale.x < scaleTmp) { worldTransform_.scale.x += 0.025f; }
	if (worldTransform_.scale.y < scaleTmp) { worldTransform_.scale.y += 0.025f; }
	if (worldTransform_.scale.z < scaleTmp) { worldTransform_.scale.z += 0.025f; }

	//block_.SetWorldPos(pos);
	worldTransform_.SetWorld();
}

void Block::Draw(Camera* camera, UINT64* texhandle, int form, Action action, bool isElec, WorldMat goalMat,bool isPushed)
{
	//‰¼•\Ž¦
	if (action == Action::Connect) 
	{ 
		color = { 0.2f,0.1f,0.8f,0.95f }; 
	}
	else if(isElec == false)
	{
		color = { 0.6f,0.6f,0.6f,0.95f };

	}
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



	if(isElec == true)
	{
		if(form == Form::BLOCK) { draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color); }
		if(form == Form::GEAR) { draw[1].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &socket_[0], color); }
		if(form == Form::BUTTON) { draw[2].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color); }
		if(form == Form::BUTTON && isPushed == false)
		{ 
			worldTransform_.trans.y = 1.2f;
			draw[3].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &button_[0], color); 
		}
		else if(form == Form::BUTTON && isPushed == true)
		{
			worldTransform_.trans.y = -1.0f;
			draw[4].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &overlapBlock_[0], color);
		}

		/*goalMat.trans.y = -0.5;
		draw[5].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &normal_[0], color);*/

		if(form == Form::GOAL)
		{
			/*connectEM->GenerateRandomConnectingEffect({ worldTransform_.trans.x,worldTransform_.trans.y + radius_ * 4.0f,worldTransform_.trans.z }
			, radius_*1.0f, radius_*1.5f, 5, 3, { 0.1f,0.2,1.0f,0.5f });*/
			/*ParticleManager::GetInstance()->GenerateRandomParticle(2, 60, 0.6f, { worldTransform_.trans.x,worldTransform_.trans.y + radius_ * 4.8f,worldTransform_.trans.z },
				0.2f, 0, { 0,1.0f,0.5f,1.0f }, { 0,0,0,0 });*/

			/*goalMat.trans.y = -0.5;
			draw[11].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &normal_[0], color);*/

			isGoalElec = true;
			count++;
			if(count % 240 == 0 || count % 240 == 10 || count % 240 == 20 || count % 240 == 30)
			{
				worldTransform_.scale = { scaleTmp + scaleTmp / 4.0f,scaleTmp + scaleTmp / 4.0f ,scaleTmp + scaleTmp / 4.0f };
			}
			worldTransform_.trans.y = 0.2f;
			draw[6].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &goal_[0], color);
		}
	}
	else
	{
		if(form == Form::BLOCK) 
		{
			draw[7].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &disconnectedBlock_[0], color); 
		}
		if(form == Form::GEAR) 
		{
			draw[8].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &socket_[0], color);
		}


		if(form == Form::BUTTON) { draw[7].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color); }
		if(form == Form::BUTTON && isPushed == false) 
		{ 
			worldTransform_.trans.y = 1.2f;
			draw[9].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &disconnectedButton_[0], color);
		}
		else if(form == Form::BUTTON && isPushed == true)
		{
			worldTransform_.trans.y = -1.0f;
			draw[10].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &overlapBlock_[0], color);
		}

		goalMat.trans.y = -0.5;
		draw[15].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &disconnectedBlock_[0], color);


		if(form == Form::GOAL)
		{
			isGoalElec = false;
			/*goalMat.trans.y = -0.5;
			draw[11].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &normal_[0], color);*/
			count++;
			if(count % 240 == 0 || count % 240 == 10 || count % 240 == 20 || count % 240 == 30)
			{
				worldTransform_.scale = { scaleTmp + scaleTmp / 4.0f,scaleTmp + scaleTmp / 4.0f ,scaleTmp + scaleTmp / 4.0f };
			}
			worldTransform_.trans.y = 0.2f;
			draw[11].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &doorGoalClosed_[0], color);
		}

	}

	if(isGoalElec == true)
	{
		goalMat.trans.y = -0.5;
		draw[5].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &normal_[0], color); 
	}
	else
	{
		goalMat.trans.y = -0.5;
		draw[15].DrawModel(&goalMat, &camera->viewMat, &camera->projectionMat, &disconnectedBlock_[0], color);
	}

	/*if (form == Form::BLOCK) { draw[0].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color); }
	if (form == Form::BUTTON) { draw[1].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &button_[0], color); }*/
	//if (form == Form::GEAR) { draw[2].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &socket_[0], color); }
	

	if (form == Form::LOCKED) { draw[12].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &locked_[0], color); }

	if (form == Form::Electric) 
	{
		color = { 0.9f,0.9f,0.9f,0.95f };

		draw[13].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &normal_[0], color);
		worldTransform_.trans.y = 0.2f;
		draw[14].DrawModel(&worldTransform_, &camera->viewMat, &camera->projectionMat, &electricBlock_[0], color); 
	}

	
}

void Block::OnCollision(Collider& collider)
{
}

void Block::OnCollision2(Collider& collider)
{
}

void Block::SetScale(const Vec3& scale)
{
	worldTransform_.scale = scale;
	worldTransform_.SetWorld();
}

void Block::SetWorldPos(Vec3& pos)
{
	worldTransform_.trans = pos;
	worldTransform_.SetWorld();
}
