#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

//float4 main(VSOutput input) : SV_TARGET
//{
//	float3 light = normalize(float3(1, -1, 1));  //�E���� �����̃��C�g
//	float diffuse = saturate(dot(-light, input.normal));  //diffuse��[0, 1]�͈̔͂�Clamp����
//	float brightness = diffuse + 0.3f;  //�A���r�G���g����0.3�Ƃ��Čv�Z
//	float4 texcolor = float4(tex.Sample(smp, input.uv));
//	//return float4(brightness, brightness, brightness, 1); //�P�x��RGB�ɑ�����ďo��
//	return float4(texcolor.rgb * brightness, texcolor.a) * color;
//}

float4 main(VSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1)); // �E�����@�����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 texcolor = tex.Sample(smp, input.uv) * color;
	return float4(texcolor.rgb * brightness, texcolor.a);
	//return float4(1, 1, 1, 1);
}