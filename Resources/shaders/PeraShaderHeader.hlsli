
Texture2D<float4> tex : register(t0);

SamplerState smp : register(s0);

cbuffer ConstBufferEffectFlags : register(b0)
{
	//�t�H�O
	uint isFog;
	//�ڂ���
	uint isGaussian;
	//�K�E�V�A���ڂ���
	uint isGaussian2;
	//�G���{�X
	uint isEmboss;
	//�V���[�v�l�X
	uint isSharpness;
	//�~��
	uint isGradation;
	//�A�E�g���C��
	uint isOutLine;
	//�r�l�b�g
	uint isVignette;
	//�M��p��
	uint isBarrelCurve;
}

//vs����ps�ɑ���
struct Output
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCOORD;
};