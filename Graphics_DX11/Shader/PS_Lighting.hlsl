#include "ConstantBuffer.hlsli"

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

Texture2D Depth : register(t0); // 뎁스 값

Texture2D Normal : register(t1); // 노말 벡터 값

Texture2D Position : register(t2); // 위치 값

Texture2D Albedo : register(t3); // 색상 값

Texture2D<uint> ObjectID : register(t4); // Object ID 값

Texture2D SSAO : register(t5);          // Screen Space Ambient Occlusion

Texture2D Shadow : register(t6);        // Shadow Maps

Texture2DArray CascadeShadow : register(t7); // Cascade Shadow Map

SamplerState Sam_Clamp : register(s0);

SamplerComparisonState Sam_PCF : register(s1);

// Dist : World - View Space Z => Depth로 하면 쓸데 없이 거의 뒷부분으로 넘어감 .. 텍스쳐 낭비도 생기고
uint CalcCascadeIndex(float Dist)
{
    uint index = MAX_CASCADE_COUNT - 1;

    for (uint i = 0; i < MAX_CASCADE_COUNT; i++)
    {
        if (Dist <= _cascadeEnds[i])
        {
            index = i;
            break;
        }
    }
    
    return index;
}

float CalcShadowFactor(SamplerComparisonState samShadow, Texture2D shadowMap, float3 shadowPosNDC)
{    
    float depth = shadowPosNDC.z;
    
    // 쉐도우 맵의 텍셀 크기
    const float dx = _textureInfo.z;
    
    const float dy = _textureInfo.w;
    
    float percentLit = 0.0f;
    
    const float2 offsets[9] =
    {
        float2(-dx, -dy), float2(0.0f, -dy), float2(dx, -dy),
        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
        float2(-dx, +dy), float2(0.0f, +dy), float2(dx, +dy)
    };

    [unroll]
    for (int i = 0; i < 9; i++)
    {
        percentLit += shadowMap.SampleCmpLevelZero(samShadow, shadowPosNDC.xy + offsets[i], depth).r;
    }
    
    return percentLit /= 9.0f;
}

// Dist : World - View Space Z
float CalcCascadeShadowColor(SamplerComparisonState samShadow, Texture2DArray cascadeShadow, float4 PosWorld, float Dist)
{
    uint index = CalcCascadeIndex(Dist);
        
    float4 PosShadowSpace = mul(PosWorld, _lightViewProjList[index]);
    
    float4 texCoord = PosShadowSpace / PosShadowSpace.w;
    
    // NDC -> UV Space
    texCoord.y = -texCoord.y;
    
    texCoord = texCoord * 0.5f + 0.5f;
    
    if ((saturate(texCoord.x) != texCoord.x) || (saturate(texCoord.y) != texCoord.y))
    {
        return 1.f;
    }
    
    float compare_depth = PosShadowSpace.z;
   
    float percentLit = 0.0f;

    int2 offsets[9] =
    {
        -1, -1, 0, -1, 1, -1,
        -1, 0, 0, 0, 1, 0,
        -1, 1, 0, 1, 1, 1
    };
    
    [unroll]
    for (int i = 0; i < 9; i++)
    {
        percentLit += cascadeShadow.SampleCmpLevelZero(samShadow, float3(texCoord.xy, index), compare_depth, offsets[i]).r;
    }
    
    return percentLit /= 9.0f;
}

float4 PS_Lighting(VS_OUT input) : SV_TARGET
{
    float depth = Depth.Sample(Sam_Clamp, input.uv).x;
	
    float3 normal = Normal.Sample(Sam_Clamp, input.uv).xyz;
    
    float4 position = Position.Sample(Sam_Clamp, input.uv).xyzw;
    
    float4 color = Albedo.Sample(Sam_Clamp, input.uv).xyzw;
    
    uint objectID = (uint) ObjectID.Load(int3(input.uv.x * _screenInfo.x, input.uv.y * _screenInfo.y, 0));
    
#ifdef USE_SSAO
    float ambientAccess = SSAO.Sample(Sam_Clamp, input.uv).x;
#else
    float ambientAccess = 1.f;
#endif
    
    // Spot Light Shadow
    float4 lightViewProjPos = mul(position, _lightViewProj);
    
    float2 projTexCoord = lightViewProjPos.xy / lightViewProjPos.w;
    
    projTexCoord.y = -projTexCoord.y;
    
    projTexCoord = projTexCoord * 0.5f + 0.5f;
    
    float shadow;
    
    if ((saturate(projTexCoord.x) != projTexCoord.x) || (saturate(projTexCoord.y) != projTexCoord.y))
        shadow = 1.f;
    else
    {
        float currentDepthValue = lightViewProjPos.z / lightViewProjPos.w;
    
        shadow = CalcShadowFactor(Sam_PCF, Shadow, float3(projTexCoord.x, projTexCoord.y, currentDepthValue));
    }
    // Spot Light Shadow
    
    // Directional Light Shadow
    float4 posView = mul(position, _cameraInfo._viewMatrix);
    
    float dirShadow = CalcCascadeShadowColor(Sam_PCF, CascadeShadow, position, posView.z);
    // Directional Light Shadow
        
    /// Deferred Lighting
    if ((objectID & g_LightCullMask) != g_LightCullMask)
    {
        float3 toEye = _cameraInfo._cameraWorldPos - position.xyz;
    
        toEye = normalize(toEye);
    
        float4 ambient = 0;
        float4 diffuse = 0;
        float4 specular = 0;
    
        float4 outAmbient, outDiffuse, outSpecular;
        
        for (int i = 0; i < _dirLightCount; i++)
        {
            ComputeDirectionalLight(float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
                            float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
                            _directionalLightInfo[i], normal, toEye,
                            outAmbient, outDiffuse, outSpecular);
            
            ambient += outAmbient * ambientAccess;
            diffuse += outDiffuse * dirShadow;
            specular += outSpecular * dirShadow;
        }
        
        for (int j = 0; j < _spotLightCount; j++)
        {
            ComputeSpotLight(float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
                            float4(0.3f, 0.3f, 0.3f, 1.f), float4(0.3f, 0.3f, 0.3f, 1.f),
                            _spotLightInfo[j], position.xyz, normal, toEye,
                            outAmbient, outDiffuse, outSpecular);
            
            ambient += outAmbient * ambientAccess;
            diffuse += outDiffuse * shadow;
            specular += outSpecular * shadow;
        }
        
        for (int k = 0; k < _pointLightCount; k++)
        {
            ComputePointLight(float4(0.6f, 0.6f, 0.6f, 1.f), float4(0.6f, 0.6f, 0.6f, 1.f),
                            float4(0.6f, 0.6f, 0.6f, 1.f), float4(0.6f, 0.6f, 0.6f, 1.f),
                            _pointLightInfo[k], position.xyz, normal, toEye,
                            outAmbient, outDiffuse, outSpecular);
            
            ambient += outAmbient * ambientAccess;
            diffuse += outDiffuse;
            specular += outSpecular;
        }

        color = color * (ambient + diffuse) + specular;
    
        color.a = 1.f;
    }
     
    // Linear -> Gamma 작업은 Final Shader에서 ..
    return color;
}