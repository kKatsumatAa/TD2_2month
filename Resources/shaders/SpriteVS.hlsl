#include "Sprite.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	//�s�N�Z���V�F�[�_�ɓn���l
	VSOutput output;//�s�N�Z���V�F�[�_�ɓn���l
	output.svpos = mul(mul(viewproj, world), pos);
	output.uv = uv;

	return output;
}