// 레퍼런스 : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Light Informations For PBR
struct DirectionalLightInfo
{
    float3 _color;          // 빛의 색이요
    float _lumen;           // 빛의 세기
    
    float3 _direction;      // 방향
    float _pad;
};

struct PointLightInfo
{
    float3 _color;
    float _lumen;
    
    float3 _position;       // 위치
    float _range;           // 범위
};

struct SpotLightInfo
{
    float3 _color;
    float _lumen;
    
    float3 _direction;      // 범위
    float _halfAngle;       // Spot Half Angle (== FOV) => 스포트 라이트가 퍼지는 범위
    
    float3 _position;       // 위치
    float _range;           // 반지름 범위
};

// Physically Based shading model: Lambetrtian diffuse BRDF + Cook-Torrance microfacet specular BRDF + IBL for ambient.

// This implementation is based on "Real Shading in Unreal Engine 4" SIGGRAPH 2013 course notes by Epic Games.
// See: http://blog.selfshadow.com/publications/s2013-shading-course/karis/s2013_pbs_epic_notes_v2.pdf

static const float PI = 3.141592f;
static const float Epsilon = 0.00001f;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04f;

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// Returns number of mipmap levels for specular IBL environment map.
uint querySpecularTextureLevels(TextureCube specularTexture)
{
    uint width, height, levels;

    specularTexture.GetDimensions(0, width, height, levels);
    
    return levels;
}

// 평행광에게는 범위따위 없다.
float CalcAttenuation(float distance, float range)
{
    float distance2 = pow(distance, 2.f);
    
    float range2 = pow(range, 2.f);
    
    float distanceAttenuation = 1.f / (distance2 + 1);
    
    float lightRadiusMask = pow(saturate(1 - pow(distance2 / range2, 2.f)), 2.f);

    return distanceAttenuation * lightRadiusMask;
}

float3 CalcDirectionalLight(in DirectionalLightInfo lightInfo, float3 albedo, float3 F0, float3 Lo, float cosLo,
    float3 N, float metallic, float roughness)
{
    float3 Li = -normalize(lightInfo._direction);
    
    float3 Lradiance = lightInfo._color * lightInfo._lumen;
    
    // Half-Vector Li and Lo
    float3 Lh = normalize(Li + Lo);
    
    float cosLi = max(0.f, dot(N, Li));
    float cosLh = max(0.f, dot(N, Lh));
    
    float3 F = fresnelSchlick(F0, max(0.f, dot(Lh, Lo)));
    
    float D = ndfGGX(cosLh, roughness);
    
    float G = gaSchlickGGX(cosLi, cosLo, roughness);
    
    float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);
    
    float3 diffuseBRDF = kd * albedo;
    
    float3 specularBRDF = (F * D * G) / max(Epsilon, 4.f * cosLi * cosLo);      // 원주율 어디갔어
    
    return (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
}

//float3 CalcSpotLight(in SpotLightInfo lightInfo, float3 pixelPosition, float3 albedo, float F0, float3 Lo, float cosLo,
//    float3 N, float metallic, float roughness)
//{
//    float3 Li = -normalize(lightInfo._direction);
    
//    // 1. Spot Light의 Half Angle 안에 들어오는가
//    float3 PixelToLight = normalize(lightInfo._position - pixelPosition);
    
    
//    // 2. PBR 계산
//    float3 Li = -lightInfo._direction
    
//    // 3. 감쇠
//    float distance = length(lightInfo._position - pixelPosition);
    
//    float attenuation = CalcAttenuation(distance, lightInfo._range);

//}

//float3 CalcPointLight()
//{
    
//    // Attenuation
//}