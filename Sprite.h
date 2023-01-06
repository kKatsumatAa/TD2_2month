#pragma once
#include"SpriteCommon.h"
#include "Util.h"


class Sprite
{
private:
	//

public:
	VertexSprite vertices[4] = {
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}}
	};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

public:
	void Initialize();
	void SpriteDraw();

	void Update(const Vec3& pos, const float& scale,
		XMFLOAT4 color, const UINT64 textureHandle, const Vec2& ancorUV,
		const bool& isReverseX,const bool& isReverseY, float rotation,
		ConstBuffTransform* cbt, ConstBufferDataMaterial* constMapMaterial);

	void UpdateClipping(const Vec3& leftTop, const float& scale, const XMFLOAT2& UVleftTop, const XMFLOAT2& UVlength,
		XMFLOAT4 color, const UINT64 textureHandle, bool isPosLeftTop,
		const bool& isReverseX,const bool& isReverseY, float rotation, ConstBuffTransform* cbt, ConstBufferDataMaterial* constMapMaterial);
};

//���ʂ̏���
void SpriteCommonBeginDraw(PipeLineSet* pipelineSet);



//void SpriteCommonCreate(SpriteSet* spriteSet);