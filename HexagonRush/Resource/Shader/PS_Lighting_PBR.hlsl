#include "Shadow.hlsli"

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 world : POSITION;
};

// G-Buffers
Texture2D DMRAO : register(t0); // Depth, Normal, Roughness, Ambient Occlusion

Texture2D Normal : register(t1); // Normal

Texture2D Position : register(t2); // Position

Texture2D Albedo : register(t3); // Albedo Color

Texture2D Emissive : register(t4);  // Emissive (G Buffer�� �ƴ϶� ������Ʈ�� GBUFFER)

Texture2D<uint> ObjectID : register(t5); // ObjectID 
// G-Buffers



// For Indirect Light (Image Based Lighting)
TextureCube Irradiance : register(t6);                   // Irradiance Map

TextureCube Specular_Pre_Filtered : register(t7);        // Pre-Filtered Specular

Texture2D Specular_BRDF_LookUp : register(t8); // Pre-Computed Spacular term

Texture2D SSAO : register(t9);          // Screen Space Ambient Occlusion

Texture2D Shadow : register(t10);        // Shadow Maps

Texture2DArray CascadeShadow : register(t11); // Cascade Shadow Map

SamplerState Sam_Clamp : register(s0);

SamplerComparisonState Sam_PCF : register(s1);

// PBR Deferred Rendering
float4 PS_Lighting_PBR(VS_OUT input) : SV_TARGET
{
    float depth = DMRAO.Sample(Sam_Clamp, input.uv).x;
	
    float3 normal = Normal.Sample(Sam_Clamp, input.uv).xyz;
    
    float4 position = Position.Sample(Sam_Clamp, input.uv).xyzw;
    
    float3 albedo = Albedo.Sample(Sam_Clamp, input.uv).xyz;
    
    uint objectID = (uint) ObjectID.Load(int3(input.uv.x * _screenInfo.x, input.uv.y * _screenInfo.y, 0));
    
#ifdef USE_SSAO
    float ambientAccess = SSAO.Sample(Sam_Clamp, input.uv).x;
#else
    float ambientAccess = 1.f;
#endif
    
    // �ݼӼ�
    float metallic = DMRAO.Sample(Sam_Clamp, input.uv).y;
    
    // ��ĥ��
    float roughness = DMRAO.Sample(Sam_Clamp, input.uv).z;
    
    // ����
    float ambientOcclusion = DMRAO.Sample(Sam_Clamp, input.uv).w * ambientAccess;
    
    // ��ȭ, ������ �� �ش�.
    ambientOcclusion = pow(ambientOcclusion, 3.f);
    
    float4 emissive = Emissive.Sample(Sam_Clamp, input.uv).xyzw;
    
    // ������ ���ϴ� ����
    float3 Lo = _cameraInfo._cameraWorldPos - position.xyz;
    
    Lo = normalize(Lo);
    
    // Normal�� Outgoing Light Direction ������ ��
    float cosLo = max(0.f, dot(normal, Lo));
    
    // Specular Reflection Light Vector
    float3 Lr = 2.0f * cosLo * normal - Lo;
    
    // ���� �Ի翡���� ������ �ݻ� (metal�� ���� ������)
    float3 F0 = lerp(Fdielectric, albedo, metallic);
    
    /// Deferred Lighting
    if ((objectID & g_LightCullMask) != g_LightCullMask)
    {
        // Direct Light (������)
        float3 directLighting = (float3) 0.f;
        
        for (uint i = 0; i < _dirLightCount; i++)
        {
            // Directional Light Shadow
            float4 posView = mul(position, _cameraInfo._viewMatrix);
    
            float dirShadow = CalcCascadeShadowColor(Sam_PCF, CascadeShadow, position, posView.z);
            // Directional Light Shadow
            
            directLighting += CalcDirectionalLight(_directionalLightInfo[i], albedo, F0, Lo, cosLo, normal, metallic, roughness) * dirShadow;
        }
        
        //for (uint j = 0; j < _pointLightCount; j++)
        //{
        //    // Point Light�� ������� �������� �ʽ��ϴ� ..
        //}
        
        //for (uint k = 0; k < _spotLightCount; k++)
        //{
        //     // Spot Light Shadow
        //    float4 lightViewProjPos = mul(position, _lightViewProj);
    
        //    float2 projTexCoord = lightViewProjPos.xy / lightViewProjPos.w;
    
        //    projTexCoord.y = -projTexCoord.y;
    
        //    projTexCoord = projTexCoord * 0.5f + 0.5f;
    
        //    float shadow;
    
        //    if ((saturate(projTexCoord.x) != projTexCoord.x) || (saturate(projTexCoord.y) != projTexCoord.y))
        //        shadow = 1.f;
        //    else
        //    {
        //        float currentDepthValue = lightViewProjPos.z / lightViewProjPos.w;
    
        //        shadow = CalcShadowFactor(Sam_PCF, Shadow, float3(projTexCoord.x, projTexCoord.y, currentDepthValue));
        //    }
        //    // Spot Light Shadow
            
        //}
        
        // Ambient Light (������) => IBL
        float3 ambientLighting = 0.f;
        {
#ifdef USE_IBL
            // �븻 ���� ���⿡���� Diffuse Irradiace�� �����´�.
            float3 irradiance = Irradiance.Sample(Sam_Clamp, normal).rgb;
            
            // �ں��Ʈ ����Ʈ�� ������ ����
            float3 F = fresnelSchlick(F0, cosLo);
            
            // Diffuse ������ ������ �⿩��
            float3 kd = lerp(1.f - F, 0.f, metallic);
            
            float3 ks = float3(1.f, 1.f, 1.f) - kd;
            
            float3 diffuseIBL = kd * albedo * irradiance;
            
            uint specularTextureLevels = querySpecularTextureLevels(Specular_Pre_Filtered);
            
            // Gamma => Linear
            float3 specularIrradiance = pow(Specular_Pre_Filtered.SampleLevel(Sam_Clamp, Lr, roughness * specularTextureLevels).rgb, 2.2f);
            
            float2 specularBRDF = Specular_BRDF_LookUp.Sample(Sam_Clamp, float2(cosLo, roughness)).rg;
            
            float3 specularIBL = (ks * specularBRDF.x + specularBRDF.y) * specularIrradiance;
            
            ambientLighting = diffuseIBL + specularIBL;
            
            // �ں��Ʈ ���󵵱��� ����
            //ambientLighting *= ambientOcclusion;
#else
            ambientLighting = albedo * (1.1f - roughness) * 0.3f;
#endif
        }
        
        // �������� �������� ���Ͽ� ��ȯ�Ѵ�. �׸��� Emissive���� ..!
        return float4(directLighting + ambientLighting + emissive.rgb, 1.f) * ambientOcclusion;
    }
     
    // Linear -> Gamma �۾��� Final Shader����.
    return float4(albedo + emissive.rgb, 1.f);
}