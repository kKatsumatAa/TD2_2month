#include "DrawIntNumImage.h"
#include <math.h>

void DrawIntNumImage::Initialize(UINT64 texhandle)
{
	//余り
	numRemainder = 0;
	//その桁
	numDigit = 0;

	this->texhandle = texhandle;

	numImages.clear();
}

void DrawIntNumImage::SetNum(int num, Vec2 pos, Vec2 sizeUV, Vec2 numImageSize, float scale, XMFLOAT4 color)
{
	this->numImages.clear();

	//最初に数字全部入れる
	numRemainder = num;

	//桁が始まったらそこから全部描画する用
	isStartDigit = false;

	for (int i = 4; i >= 0; i--)
	{
		//その桁の数字を出す
		numDigit = numRemainder / (int)std::pow(10, i);
		//余りを出して次の桁で使う
		numRemainder = numRemainder % (int)std::pow(10, i);

		//その桁があるか、前の桁があったら,0だったら
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
			//桁の数も考慮して、中心座標からの距離を出し、真の座標とする
			Vec2 pos = { num->pos.x + num->numImageSize.x / 2.0f * (i - (int)this->numImages.size() / 2) * num->scale,num->pos.y };

			pos.x -= num->numImageSize.x / 2.0f * num->scale;
			pos.y -= num->numImageSize.y / 2.0f * num->scale;

			num->obj.DrawClippingBoxSprite({ pos.x,pos.y,0 }, num->scale, { num->num * num->sizeUV.x,0 }, { num->sizeUV.x,num->sizeUV.y }, num->color, texhandle, true);

			i++;
		}
	}
}
