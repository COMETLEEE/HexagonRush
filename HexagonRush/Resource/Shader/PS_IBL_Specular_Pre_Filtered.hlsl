struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 color : COLOR;
};

struct VS_OUT
{
    float4 posH : SV_Position;
    float3 posL : POSITON;
    float4 posW : POSITION2;
    
    uint objectID : OBJECTID;
};

static const float PI = 3.141592;
static const float TwoPI = 2 * PI;
static const float Epsilon = 0.00001;

static const uint NumSamples = 1024;
static const float InvNumSamples = 1.0 / float(NumSamples);

cbuffer SpecularMapFilterSettings : register(b0)
{
	// Roughness value to pre-filter for.
    float roughness;
};

TextureCube TexCube_Env : register(t0);

SamplerState Sam_Clamp : register(s0);

float3 FresnelSchlickRoughness(float NdotV, float3 F0, float roughness)   // cosTheta is n.v and F0 is the base reflectivity
{
    float roughnessPercent = 1.0f - roughness;
    
    return F0 + (max(float3(roughnessPercent, roughnessPercent, roughnessPercent), F0) - F0) * pow(1.0 - NdotV, 5.0f);
}

float3 FresnelLerp(float NdotV, float3 F0, float3 F90)
{
    float t = pow(1 - NdotV, 5.0f); // ala Schlick interpoliation
    return lerp(F0, F90, t);
}

float NormalDistributionGGXTR(float3 normalVec, float3 halfwayVec, float Roughness2)
{
    float a2 = Roughness2 * Roughness2; // a2 = a^2
    float NdotH = max(dot(normalVec, halfwayVec), 0.0f); // NdotH = normalVec.halfwayVec
    float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2
    float denom = (PI * pow(NdotH2 * (a2 - 1.0f) + 1.0f, 2.0f));
    
    if (denom < Epsilon)
        return 1.0f;

    return a2 / denom;
}

float IBLGeometrySchlickGGX(float NdotV, float k)  // k is a remapping of roughness based on direct lighting or IBL lighting
{
    float denom = NdotV * (1.0f - k) + k;

    return NdotV / denom;
}

float IBLGeometrySmith(float3 normalVec, float3 viewDir, float3 lightDir, float Roughness2)
{
    float k = Roughness2 / 2.0f;
    
    float NdotV = max(dot(normalVec, viewDir), 0.0f);
    float NdotL = max(dot(normalVec, lightDir), 0.0f);
    float ggx1 = IBLGeometrySchlickGGX(NdotV, k);
    float ggx2 = IBLGeometrySchlickGGX(NdotL, k);

    return ggx1 * ggx2;
}

float IBLSmithGGXCorrelated(float NdotV, float NdotL, float Roughness2)
{
    float GGXV = NdotL * sqrt(NdotV * NdotV * (1.0 - Roughness2) + Roughness2);
    float GGXL = NdotV * sqrt(NdotL * NdotL * (1.0 - Roughness2) + Roughness2);
    return 0.5f / (GGXV + GGXL);
}

float RadicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

float2 Hammersley(uint i, uint N)
{
    return float2(float(i) / float(N), RadicalInverse_VdC(i));
}

float3 ImportanceSampleGGX(float2 Xi, float Roughness2, float3 normalVec)
{
    float Phi = 2.0f * PI * Xi.x;
    float CosTheta = sqrt((1.0f - Xi.y) / (1.0f + (Roughness2 * Roughness2 - 1.0f) * Xi.y));
    float SinTheta = sqrt(1.0f - CosTheta * CosTheta);
	// from spherical coordinates to cartesian coordinates - halfway vector
    float3 halfwayVec;
    halfwayVec.x = SinTheta * cos(Phi);
    halfwayVec.y = SinTheta * sin(Phi);
    halfwayVec.z = CosTheta;
	// from tangent-space H vector to world-space sample vector
    float3 UpVector = abs(normalVec.z) < 0.999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 TangentX = normalize(cross(UpVector, normalVec));
    float3 TangentY = cross(normalVec, TangentX);
	// Tangent to world space
    return normalize((TangentX * halfwayVec.x) + (TangentY * halfwayVec.y) + (normalVec * halfwayVec.z));
}

float4 PS_IBL_Specular_Pre_Filtered(VS_OUT input)
{
    float3 normalVec = normalize(input.posL);
    
    float3 R = normalVec;
    
    float3 viewDir = R;
        
    float roughness2 = roughness * roughness;
	
    float3 PrefilteredColor = float3(0.0f, 0.0f, 0.0f);
    
    float totalWeight = 0.0f;
	    
    for (uint i = 0; i < NumSamples; i++)
    {
        float2 Xi = Hammersley(i, NumSamples);
        float3 halfwayVec = ImportanceSampleGGX(Xi, roughness2, normalVec);
        float3 lightDir = normalize(2.0f * dot(viewDir, halfwayVec) * halfwayVec - viewDir);
        float NdotL = max(dot(normalVec, lightDir), 0.0f);
		
        if (NdotL > 0)
        {
            float D = NormalDistributionGGXTR(normalVec, halfwayVec, roughness2);
            float NdotH = max(dot(normalVec, halfwayVec), 0.0f);
            float HdotV = max(dot(halfwayVec, viewDir), 0.0f);
            float pdf = (D * NdotH) / (4.0f * HdotV);

            float resolution = 1024.0f;
            
            float saTexel = 4.0f * PI / (6.0f * resolution * resolution);
            float saSample = 1.0f / max((float(NumSamples) * pdf), Epsilon);

            float fMipBias = 1.0f;
            float fMipLevel = roughness == 0.0 ? 0.0 : max(0.5 * log2(saSample / saTexel) + fMipBias, 0.0f);
			
            PrefilteredColor += TexCube_Env.SampleLevel(Sam_Clamp, lightDir, fMipLevel).rgb * NdotL;

            totalWeight += NdotL;
        }
    }

    PrefilteredColor /= max(totalWeight, Epsilon);

    return float4(PrefilteredColor, 1.0f);
}