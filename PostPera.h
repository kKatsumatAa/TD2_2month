#pragma once
#include"TextureManager.h"
#include "Directx.h"


//��ʌ��ʗp�̃t���O
struct EffectConstBuffer
{
	//�t�H�O
	unsigned int isFog = false;
	//�ڂ���
	unsigned int isGaussian = false;
	//�K�E�V�A���ڂ���
	unsigned int isGaussian2 = false;
	//�G���{�X
	unsigned int isEmboss = false;
	//�V���[�v�l�X
	unsigned int isSharpness = false;
	//�~��
	unsigned int isGradation = false;
	//�A�E�g���C��
	unsigned int isOutLine = false;
	//�r�l�b�g
	unsigned int isVignette = false;
	//�M��p��
	unsigned int isBarrelCurve = false;
};

struct PeraVertex
{
	XMFLOAT3 pos;
	float pad1 = 1.0f;
	XMFLOAT2 uv;
};

class PostPera
{
private:
	PeraVertex pv[4] = {
		{{-1, -1,0.1f},1,{0,1}},//����
		{{-1, 1,0.1f},1,{0,0}},//����
		{{1, -1,0.1f},1,{1,1}},//�E��
		{{1, 1,0.1f},1,{1,0}}//�E��
	};

	ComPtr<ID3D12Resource> _peraVB;
	D3D12_VERTEX_BUFFER_VIEW _peraVBV;

	ComPtr<ID3DBlob> vs;
	ComPtr<ID3DBlob> ps;
	ComPtr<ID3DBlob> errBlob;

	ComPtr<ID3D12RootSignature> _peraRS;
	ComPtr<ID3D12PipelineState> _peraPipeline = NULL;

	//��ʌ��ʗp
	ComPtr <ID3D12Resource> effectFlagsBuff;
	EffectConstBuffer* mapEffectFlagsBuff;

	//���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[1] = {};

public:
	void Initialize();
	void GenerateRSPL();
	void Draw(EffectConstBuffer effectFlags);
};

