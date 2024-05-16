#include "fullscreen_quad.hlsli"

#define POINT 0
#define LINEAR 1
#define ANISOTROPIC 2
#define LINEAR_BORDER_BLACK 3
#define LINEAR_BORDER_WHITE 4
Texture2D g_texture[2] : register(t0); //t0,t1
SamplerState g_sampler[5] : register(s0); //s0~s4

float4 main(VS_OUT pin) : SV_TARGET
{
    float2 uv = pin.texcoord;
    
    //UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 720.0f), //��
        float2(0.0f, -1.0f / 720.0f), //��
        float2(1.0f / 1280.0f, 0.0f), //�E
        float2(-1.0f / 1280.0f, 0.0f), //��
        float2(1.0f / 1280.0f, 1.0f / 720.0f), //�E��
        float2(-1.0f / 1280.0f, 1.0f / 720.0f), //����
        float2(1.0f / 1280.0f, -1.0f / 720.0f), //�E��
        float2(-1.0f / 1280.0f, -1.0f / 720.0f), //����
    };
    
    float depth = g_texture[1].Sample(g_sampler[LINEAR], uv).x;
    
    float depth2 = 0.0f;
    for (int i = 0; i < 8; i++)
    {
        depth2 += g_texture[1].Sample(g_sampler[LINEAR], uv + uvOffset[i]).x;
    }
    
    depth2 /= 8.0f;
    if (abs(depth2 - depth) > 0.00105f)
    {
        //return g_texture[0].Sample(g_sampler[LINEAR], pin.texcoord);

        return float4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    return g_texture[0].Sample(g_sampler[LINEAR], pin.texcoord);
}