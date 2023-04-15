#include "ConstantBuffer.hlsli"

#define PT_EMITTER 0
#define PT_FLARE 1

static const float2 QuadTex[4] =
{
    float2(0.f, 1.f),
    float2(1.f, 1.f),
    float2(0.f, 0.f),
    float2(1.f, 0.f)
};

struct GS_OUT_PLANE
{
    float4 _pos     : SV_Position;
    
    float4 _color   : COLOR;
    
    float2 _uv      : TEXCOORD;
};

struct VS_OUT
{
    float3 _world : POSITION;
    
    float2 _size : SIZE;
    
    float4 _color : COLOR;
    
    uint _type : TYPE;
};

Texture2D Texture_DMRAO : register(t0); // ���� ��

SamplerState Sam_Clamp : register(s0);

[maxvertexcount(4)]
void GS_Particle_Draw(point VS_OUT input[1],
    inout TriangleStream<GS_OUT_PLANE> triStream)
{
    // �� ��ƼŬ �ý��� ������ ���� ���ڴ� �׸��� �ʾƿ�
    if (input[0]._type != PT_EMITTER)
    {
        float3 look = normalize(_cameraInfo._cameraWorldPos.xyz - input[0]._world);
        float3 right = normalize(cross(float3(0.f, 1.f, 0.f), look));
        float3 up = cross(look, right);

        // �簢���� �����ϴ� �ﰢ�� �� ���� 4���� ����Ѵ�.
        float halfWidth = 0.5f * input[0]._size.x;
        float halfHeight = 0.5f * input[0]._size.y;
        
        float4 v[4];
        
        v[0] = float4(input[0]._world + halfWidth * right - halfHeight * up, 1.f);
        v[1] = float4(input[0]._world + halfWidth * right + halfHeight * up, 1.f);
        v[2] = float4(input[0]._world - halfWidth * right - halfHeight * up, 1.f);
        v[3] = float4(input[0]._world - halfWidth * right + halfHeight * up, 1.f);
        
        GS_OUT_PLANE output;
        
        float4x4 viewProj = mul(_cameraInfo._viewMatrix, _cameraInfo._projMatrix);
        
        [unroll]
        for (int i = 0; i < 4; i++)
        {
            output._pos = mul(v[i], viewProj);
                        
            // NDC ������ ��ǥ�� Texture Space�� �����Ѵ�.
            float2 ndcXY = output._pos.xy / output._pos.w;
    
            ndcXY.y = -ndcXY.y;
            
            ndcXY = ndcXY * 0.5f + float2(0.5f, 0.5f);
            
            float currPixelDepth = Texture_DMRAO.SampleLevel(Sam_Clamp, ndcXY, 0).x;
            
            float currParticleDepth = output._pos.z / output._pos.w;
            
            // �̹� ��ϵ� Depth���� �� �ָ� ������ �׸��� ����.
            if (currPixelDepth < currParticleDepth)
            {
                break;
            }
            else
            {
                output._uv = QuadTex[i];
                output._color = input[0]._color;
            
                // 4���� ������ �ﰢ�� �츦 �����ϴ� !
                triStream.Append(output);
            }
        }
    }
}