#include "sprite.hlsli"

Texture2D color_map : register(t0);
SamplerState point_sampler_state : register(s0);
SamplerState linear_sampler_state : register(s1);
SamplerState anisotropic_sampler_state : register(s2);

float4 main(VS_OUT pin) : SV_TARGET
{
	float4 color = color_map.Sample(anisotropic_sampler_state,pin.texcoord);
	float alpha = color.a;
#if 1
	//Inverse ganma process
	const float GANMA = 2.2;
	color.rgb = pow(color.rgb, GANMA);
#endif
	return float4(color.rgb,alpha) * pin.color;
}