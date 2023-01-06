#pragma once
#include"ConstBuffTransform.h"
#include"TextureManager.h"
#include "WorldMat.h"
#include "ViewMat.h"
#include "ProjectionMat.h"
using namespace Microsoft::WRL;//


struct PipeLineSet
{
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
};
struct ConstBufferDataMaterial
{
	XMFLOAT4 color;//�F(RGBA)
};
struct SpriteBuff
{
	//���_�o�b�t�@
	ComPtr < ID3D12Resource> vertBuff = nullptr;
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@�̐���
};
struct VertexSprite
{
	XMFLOAT3 pos;//xyz���W
	XMFLOAT2 uv;//uv���W
};

class SpriteCommon
{
private:


public:
	PipeLineSet pipelineSet;
};

