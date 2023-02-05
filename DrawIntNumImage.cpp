#include "DrawIntNumImage.h"
#include <math.h>

void DrawIntNumImage::Initialize(UINT64 texhandle)
{
	//�]��
	numRemainder = 0;
	//���̌�
	numDigit = 0;

	this->texhandle = texhandle;

	numImages.clear();
}

void DrawIntNumImage::SetNum(int num, Vec2 pos, Vec2 sizeUV, Vec2 numImageSize, float scale, XMFLOAT4 color)
{
	this->numImages.clear();

	//�ŏ��ɐ����S�������
	numRemainder = num;

	//�����n�܂����炻������S���`�悷��p
	isStartDigit = false;

	for (int i = 4; i >= 0; i--)
	{
		//���̌��̐������o��
		numDigit = numRemainder / (int)std::pow(10, i);
		//�]����o���Ď��̌��Ŏg��
		numRemainder = numRemainder % (int)std::pow(10, i);

		//���̌������邩�A�O�̌�����������,0��������
		if (numDigit > 0 || isStartDigit == true || i == 0)
		{
			isStartDigit = true;

			std::unique_ptr< NumImage > numImage = std::make_unique<NumImage>();

			numImage->num = numDigit;
			numImage->pos = pos;
			numImage->sizeUV = sizeUV;
			numImage->scale = scale;
			numImage->color = color;
			numImage->numImageSize = numImageSize;

			this->numImages.push_back(std::move(numImage));
		}
	}
}

void DrawIntNumImage::Draw()
{
	if (this->numImages.size())
	{
		int i = 0;

		for (std::unique_ptr< NumImage >& num : numImages)
		{
			//���̐����l�����āA���S���W����̋������o���A�^�̍��W�Ƃ���
			Vec2 pos = { num->pos.x + num->numImageSize.x / 2.0f * (i - (int)this->numImages.size() / 2) * num->scale,num->pos.y };

			pos.x -= num->numImageSize.x / 2.0f * num->scale;
			pos.y -= num->numImageSize.y / 2.0f * num->scale;

			num->obj.DrawClippingBoxSprite({ pos.x,pos.y,0 }, num->scale, { num->num * num->sizeUV.x,0 }, { num->sizeUV.x,num->sizeUV.y }, num->color, texhandle, true);

			i++;
		}
	}
}
