#include"PeraShaderHeader.hlsli"


float4 PS(Output input) : SV_TARGET
{
	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	float4 RGBA = tex.Sample(smp, input.uv);
	float3 RGB = RGBA.rgb;
	float  A = RGBA.a;

	float w, h, levels;
	//�~�b�v�}�b�v�A�����A�c���A�~�b�v�}�b�v���x��
	tex.GetDimensions(0, w, h, levels);

	float dx = 1.0f / w;
	float dy = 1.0f / h;
	float4 ret = float4(0, 0, 0, 0);
	bool isEffect = false;

	//�ڂ���
	if (isGaussian == true)
	{
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)); // ���� 
		ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)); // �� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy));// �E �� 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)); // �� 
		ret += tex.Sample(smp, input.uv);                       // ���� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)); // �E
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)); // ���� 
		ret += tex.Sample(smp, input.uv + float2(0, 2 * dy));// �� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)); // �E ��
		ret /= 9.0f;

		isEffect = true;
	}

	//�G���{�X
	if (isEmboss == true)
	{
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)) * 2; // ����
		ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)); // ��
		ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy)) * 0; // �E ��
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)); // ��
		ret += tex.Sample(smp, input.uv); // ����
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)) * -1; // �E
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)) * 0; // ����
		ret += tex.Sample(smp, input.uv + float2(0, 2 * dy)) * -1;// �� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)) * -2; // �E �� 

		isEffect = true;
	}

	//�V���[�v�l�X
	if (isSharpness == true)
	{
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)) * 0; // ���� 
		ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)); // �� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy)) * 0;// �E �� 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)); // �� 
		ret += tex.Sample(smp, input.uv) * 5.0f;                       // ���� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)); // �E
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)) * 0; // ���� 
		ret += tex.Sample(smp, input.uv + float2(0, 2 * dy));// �� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)) * 0; // �E ��

		isEffect = true;
	}

	//�֊s
	if (isOutLine == true)
	{
		ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)) * -1; // ��
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)) * -1; // ��
		ret += tex.Sample(smp, input.uv) * 4; // ���� 
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)) * -1; // �E 
		ret += tex.Sample(smp, input.uv + float2(0, 2 * dy)) * -1; // �� 
		// ���] 
		float Y = dot(ret.rgb * RGBA.rgb, float3(0.299, 0.587, 0.114));
		Y = pow(1.0f - Y, 10.0f);
		Y = step(0.2, Y);
		ret = float4(Y, Y, Y, A);

		isEffect = true;
	}

	//�K�E�V�A��
	if (isGaussian2 == true)
	{
		float dx = 2.0f / w;
		float dy = 2.0f / h;
		// �� �� �s�N�Z�� �� ���S �� �c�� 5 �� ���� �� �Ȃ� �悤 ���Z ���� 
		// �� ��i 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)) * 1 / 256;
		ret += tex.Sample(smp, input.uv + float2(-1 * dx, 2 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(0 * dx, 2 * dy)) * 6 / 256;
		ret += tex.Sample(smp, input.uv + float2(1 * dx, 2 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)) * 1 / 256;
		// 1 �� ��i 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 1 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(-1 * dx, 1 * dy)) * 16 / 256;
		ret += tex.Sample(smp, input.uv + float2(0 * dx, 1 * dy)) * 24 / 256;
		ret += tex.Sample(smp, input.uv + float2(1 * dx, 1 * dy)) * 16 / 256;
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 1 * dy)) * 4 / 256;
		// ���i 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0 * dy)) * 6 / 256;
		ret += tex.Sample(smp, input.uv + float2(-1 * dx, 0 * dy)) * 24 / 256;
		ret += tex.Sample(smp, input.uv + float2(0 * dx, 0 * dy)) * 36 / 256;
		ret += tex.Sample(smp, input.uv + float2(1 * dx, 0 * dy)) * 24 / 256;
		ret += tex.Sample(smp, input.uv + float2(2 * dx, 0 * dy)) * 6 / 256;
		// 1 �� ���i 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, -1 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(-1 * dx, -1 * dy)) * 16 / 256;
		ret += tex.Sample(smp, input.uv + float2(0 * dx, -1 * dy)) * 24 / 256;
		ret += tex.Sample(smp, input.uv + float2(1 * dx, -1 * dy)) * 16 / 256;
		ret += tex.Sample(smp, input.uv + float2(2 * dx, -1 * dy)) * 4 / 256;
		// �� ���i 
		ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)) * 1 / 256;
		ret += tex.Sample(smp, input.uv + float2(-1 * dx, -2 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(0 * dx, -2 * dy)) * 6 / 256;
		ret += tex.Sample(smp, input.uv + float2(1 * dx, -2 * dy)) * 4 / 256;
		ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy)) * 1 / 256;

		isEffect = true;
	}

	//�K�E�V�A���Q
	{
		/*ret += bkweights[0] * RGBA;
		for (int i = 1; i < 8; ++i)
		{
			ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(i * dx, 0));
			ret += bkweights[i >> 2][i % 4] * tex.Sample(smp, input.uv + float2(-i * dx, 0));
		}
		return float4(ret.rgb * RGBA.rgb, A);*/
	}

	//�~��
	if (isGradation)
	{
		ret = float4(RGB - fmod(RGB, 0.25f), A);

		isEffect = true;
	}

	if (isEffect)
	{
		return ret;
	}

	return RGBA;
}