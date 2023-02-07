#include "DrawIntNumImage.h"
#include <math.h>

void DrawIntNumImage::Initialize(UINT64 texhandle)
{
	//�]��
	numRemainder = 0;
	//���̌�
	numDigit = 0;

	this->texhandle = texhandle;

	numCount = 0;

	for (int i = _countof(numImages) - 1; i >= 0; i--)
	{
		this->numImages[i].isTrue = false;
	}
}

void DrawIntNumImage::SetNum(int num, Vec2 pos, Vec2 sizeUV, Vec2 numImageSize, float scale, XMFLOAT4 color)
{
	//�ŏ��ɐ����S�������
	numRemainder = num;

	//�����n�܂����炻������S���`�悷��p
	isStartDigit = false;

	//�����̐����J�E���g
	numCount = 0;

	for (int i = _countof(numImages) - 1; i >= 0; i--)
	{
		//���̌��̐������o��
		numDigit = numRemainder / (int)std::pow(10, i);
		//�]����o���Ď��̌��Ŏg��
		numRemainder = numRemainder % (int)std::pow(10, i);

		//���̌������邩�A�O�̌�����������,0��������
		if (numDigit > 0 || isStartDigit == true || i == 0)
		{
			isStartDigit = true;

			this->numImages[i].num = numDigit;
			this->numImages[i].pos = pos;
			this->numImages[i].sizeUV = sizeUV;
			this->numImages[i].scale = scale;
			this->numImages[i].color = color;
			this->numImages[i].numImageSize = numImageSize;
			this->numImages[i].isTrue = true;

			numCount++;
		}
		else
		{
			this->numImages[i].isTrue = false;
		}
	}
}

void DrawIntNumImage::Draw()
{
	if (isStartDigit)
	{
		//��ԑ傫��������X�^�[�g
		for (int i = numCount - 1; i >= 0; i--)
		{
			if (this->numImages[i].isTrue)
			{
				//���̐����l�����āA���S���W����̋������o���A�^�̍��W�Ƃ���@�@�@�@�@�@�@�@�@�@//�傫��������X�^�[�g�Ȃ̂ŋt���ɂ��āA��������ׂ�i���W�j
				Vec2 pos = { numImages[i].pos.x + numImages[i].numImageSize.x / 2.0f * ((float)(numCount - 1) - (float)i - (float)numCount / 2.0f) * numImages[i].scale
					+ (numImages[i].numImageSize.x / 4.0f * numImages[i].scale)
					,numImages[i].pos.y };

				pos.x -= numImages[i].numImageSize.x / 2.0f * numImages[i].scale;
				pos.y -= numImages[i].numImageSize.y / 2.0f * numImages[i].scale;

				numImages[i].obj.DrawClippingBoxSprite({ pos.x,pos.y,0 }, numImages[i].scale, { numImages[i].num * numImages[i].sizeUV.x,0 },
					{ numImages[i].sizeUV.x,numImages[i].sizeUV.y }, numImages[i].color, texhandle, true);
			}
		}
	}
}
