// Physically Based Rendering
// Copyright (c) 2017-2018 Michał Siejak

// Computes diffuse irradiance cubemap convolution for image-based lighting.
// Uses quasi Monte Carlo sampling with Hammersley sequence.

static const float PI = 3.141592;
static const float TwoPI = 2 * PI;
static const float Epsilon = 0.00001;

static const uint NumSamples = 64 * 1024;
static const float InvNumSamples = 1.0 / float(NumSamples);

TextureCube inputTexture : register(t0);

RWTexture2DArray<float4> outputTexture : register(u0);

SamplerState defaultSampler : register(s0);

// Compute Van der Corput radical inverse
// See: http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
float radicalInverse_VdC(uint bits)
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

// Sample i-th point from Hammersley point set of NumSamples points total.
float2 sampleHammersley(uint i)
{
    return float2(i * InvNumSamples, radicalInverse_VdC(i));
}

// Uniformly sample point on a hemisphere.
// Cosine-weighted sampling would be a better fit for Lambertian BRDF but since this
// compute shader runs only once as a pre-processing step performance is not *that* important.
// See: "Physically Based Rendering" 2nd ed., section 13.6.1.
float3 sampleHemisphere(float u1, float u2)
{
    const float u1p = sqrt(max(0.0, 1.0 - u1 * u1));
    return float3(cos(TwoPI * u2) * u1p, sin(TwoPI * u2) * u1p, u1);
}

float2 GetSamplingUV(uint3 ThreadID)
{
    float outputWidth, outputHeight, outputDepth;
    
    // Output Texture의 Desc 얻기
    outputTexture.GetDimensions(outputWidth, outputHeight, outputDepth);

    // Screen NDC
    float2 st = ThreadID.xy / float2(outputWidth, outputHeight);
    
    // -1 ~ 1 사이로 변환 (x : 0이면 -1로 => 1이면 1 로, y : 0이면 1로 => 1이면 -1로)
    return 2.0f * float2(st.x, -st.y) + float2(-1.0f, 1.0f);
}

float3 GetSamplingVector(uint3 ThreadID)
{
    float2 uv = GetSamplingUV(ThreadID);
    
    float3 ret;
    
    switch (ThreadID.z)
    {
        // X positive
        case 0:
            ret = float3(1.0, uv.y, -uv.x);
            break;
        // X Negative
        case 1:
            ret = float3(-1.0, uv.y, uv.x);
            break;
        // Y +
        case 2:
            ret = float3(uv.x, 1.0, -uv.y);
            break;
        // Y -
        case 3:
            ret = float3(uv.x, -1.0, uv.y);
            break;
        // Z +
        case 4:
            ret = float3(uv.x, uv.y, 1.0);
            break;
        // Z -
        case 5:
            ret = float3(-uv.x, uv.y, -1.0);
            break;
    }
    
    return normalize(ret);
}

// Compute orthonormal basis for converting from tanget/shading space to world space.
void computeBasisVectors(const float3 N, out float3 S, out float3 T)
{
	// Branchless select non-degenerate T.
    T = cross(N, float3(0.0, 1.0, 0.0));
    T = lerp(cross(N, float3(1.0, 0.0, 0.0)), T, step(Epsilon, dot(T, T)));

    T = normalize(T);
    S = normalize(cross(N, T));
}

// Convert point from tangent/shading space to world space.
float3 tangentToWorld(const float3 v, const float3 N, const float3 S, const float3 T)
{
    return S * v.x + T * v.y + N * v.z;
}

[numthreads(32, 32, 1)]
void CS_IBL_Irradiance(uint3 ThreadID : SV_DispatchThreadID)
{
    float3 N = GetSamplingVector(ThreadID);
	
    float3 S, T;
    computeBasisVectors(N, S, T);

	// Monte Carlo integration of hemispherical irradiance.
	// As a small optimization this also includes Lambertian BRDF assuming perfectly white surface (albedo of 1.0)
	// so we don't need to normalize in PBR fragment shader (so technically it encodes exitant radiance rather than irradiance).
    float3 irradiance = 0.0;
    for (uint i = 0; i < NumSamples; ++i)
    {
        float2 u = sampleHammersley(i);
        
        float3 Li = tangentToWorld(sampleHemisphere(u.x, u.y), N, S, T);
        
        float cosTheta = max(0.0, dot(Li, N));

		// PIs here cancel out because of division by pdf.
        irradiance += 2.0 * inputTexture.SampleLevel(defaultSampler, Li, 0).rgb * cosTheta;
    }
    irradiance /= float(NumSamples);

    outputTexture[ThreadID] = float4(irradiance, 1.0);
}