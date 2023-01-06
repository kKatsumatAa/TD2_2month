#include "Basic.hlsli"

Texture2D<float4> tex : register(t0); //0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      //0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[


float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
			float4 texcolor = tex.Sample(smp, input.uv);

			// ����x
			const float shininess = 4.0f;
			// ���_���王�_�ւ̕����x�N�g��
			float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

			// �����ˌ�
			float3 ambient = 0.06f;

			// �V�F�[�f�B���O�ɂ��F
			float4 shadecolor = float4(ambientColor * ambient, 1.0f);

			//���s����
			for (int i = 0; i < DIRLIGHT_NUM; i++) {
				if (dirLights[i].active) {
					// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
					float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
					// ���ˌ��x�N�g��
					float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
					// �g�U���ˌ�
					float3 diffuse = dotlightnormal * 0.8f;
					// ���ʔ��ˌ�
					float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 0.1f;

					// �S�ĉ��Z����
					shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
				}
			}

			//�_����
			for (int i = 0; i < POINTLIGHT_NUM; i++) {
				if (pointLights[i].active) {
					//���C�g�ւ̃x�N�g��
					float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
					//�x�N�g���̒���
					float d = length(lightv);
					//���K�����A�P�ʃx�N�g���ɂ���
					lightv = normalize(lightv);
					//���������W��
					float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d +
						pointLights[i].lightatten.z * d * d);
					// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
					float3 dotlightnormal = dot(lightv, input.normal);
					// ���ˌ��x�N�g��
					float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
					// �g�U���ˌ�
					float3 diffuse = dotlightnormal * 0.8f;
					// ���ʔ��ˌ�
					float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * 0.1f;

					// �S�ĉ��Z����
					shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
				}
			}

			// �V�F�[�f�B���O�ɂ��F�ŕ`��
			float4 RGBA = (shadecolor * texcolor * color);
			float4 RGBA2 = (shadecolor * color);
			float3 RGB = RGBA.rgb;
			float  A = RGBA.a;

			float w, h, levels;
			//�~�b�v�}�b�v�A�����A�c���A�~�b�v�}�b�v���x��
			tex.GetDimensions(0, w, h, levels);

			float dx = 1.0f / w;
			float dy = 1.0f / h;
			float4 ret = float4(0, 0, 0, 0);

			//�ڂ���
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)); // ���� 
			//ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)); // �� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy));// �E �� 
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)); // �� 
			//ret += tex.Sample(smp, input.uv);                       // ���� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)); // �E
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)); // ���� 
			//ret += tex.Sample(smp, input.uv + float2(0, 2 * dy));// �� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)); // �E ��
			//return ret / 9.0f * RGBA;

			//�V���[�v�l�X
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, -2 * dy)) * 0; // ���� 
			//ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)); // �� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, -2 * dy)) * 0;// �E �� 
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)); // �� 
			//ret += tex.Sample(smp, input.uv) * 5.0f;                       // ���� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)); // �E
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 2 * dy)) * 0; // ���� 
			//ret += tex.Sample(smp, input.uv + float2(0, 2 * dy));// �� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, 2 * dy)) * 0; // �E ��
			//return ret * RGBA;

			//�֊s
			//ret += tex.Sample(smp, input.uv + float2(0, -2 * dy)) * -1; // ��
			//ret += tex.Sample(smp, input.uv + float2(-2 * dx, 0)) * -1; // ��
			//ret += tex.Sample(smp, input.uv) * 4; // ���� 
			//ret += tex.Sample(smp, input.uv + float2(2 * dx, 0)) * -1; // �E 
			//ret += tex.Sample(smp, input.uv + float2(0, 2 * dy)) * -1; // �� 
			//// ���] 
			//float Y = dot(ret.rgb * RGBA2.rgb, float3(0.299, 0.587, 0.114));
			//Y = pow(1.0f - Y, 10.0f);
			//Y = step(0.2, Y);
			//return float4(Y, Y, Y, A);

			//�K�E�V�A��
			{
				float dx = 2.0f / w;
				//float dy = 2.0f / h;
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

				return ret * RGBA;
			}


			//�~��
			//return float4(RGB - fmod(RGB, 0.25f), A);
}
