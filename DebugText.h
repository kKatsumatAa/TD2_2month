#pragma once
#include"Object.h"

static const int maxCharCount = 256;
static const int fontWidth = 9;
static const int fontHeight = 18;
static const int fontLineCount = 14;
static const int fontIndexMaxX = 14;
static const int fontIndexMaxY = 7;

//
class DebugText
{
private:
	Object sprites[maxCharCount];
	Vec3 leftTop[maxCharCount]; float scale[maxCharCount]; XMFLOAT2 UVleftTop[maxCharCount]; XMFLOAT2 UVlength[maxCharCount];

	//�Y�����p�̕ϐ�
	int spriteIndex = 0;

public:
	void Print(const std::string& text, float x, float y, int variable = 114514, float scale = 1.0f);
	void Printf(const std::string& text, float x, float y, float variable = 114514.0f, float scale = 1.0f);
	
	void DrawAll(UINT64 texhandle);
};

