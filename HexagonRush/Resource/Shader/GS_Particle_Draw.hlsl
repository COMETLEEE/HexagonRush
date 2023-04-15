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

Texture2D Texture_DMRAO : register(t0); // 뎁스 값

SamplerState Sam_Clamp : register(s0);

[maxvertexcount(4)]
void GS_Particle_Draw(point VS_OUT input[1],
    inout TriangleStream<GS_OUT_PLANE> triStream)
{
    // 이 파티클 시스템 유형은 방출 입자는 그리지 않아요
    if (input[0]._type != PT_EMITTER)
    {
        float3 look = normalize(_cameraInfo._cameraWorldPos.xyz - input[0]._world);
        float3 right = normalize(cross(float3(0.f, 1.f, 0.f), look));
        float3 up = cross(look, right);

        // 사각형을 구성하는 삼각형 띠 정점 4개를 계산한다.
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
                        
            // NDC 공간의 좌표를 Texture Space로 변경한다.
            float2 ndcXY = output._pos.xy / output._pos.w;
    
            ndcXY.y = -ndcXY.y;
            
            ndcXY = ndcXY * 0.5f + float2(0.5f, 0.5f);
            
            float currPixelDepth = Texture_DMRAO.SampleLevel(Sam_Clamp, ndcXY, 0).x;
            
            float currParticleDepth = output._pos.z / output._pos.w;
            
            // 이미 기록된 Depth보다 더 멀리 있으면 그리지 말자.
            if (currPixelDepth < currParticleDepth)
            {
                break;
            }
            else
            {
                output._uv = QuadTex[i];
                output._color = input[0]._color;
            
                // 4개의 점으로 삼각형 띠를 묶습니다 !
                triStream.Append(output);
            }
        }
    }
}