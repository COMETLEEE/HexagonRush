// Copyright (c) 2022 Comet Lee
// 등장방형도법으로 제작된 파노라마 이미지를 
// DirectX 좌표계의 큐브맵 형태로 매핑합니다.

static const float PI = 3.141592;

static const float TwoPI = 2 * PI;

Texture2D inputTexture : register(t0);

RWTexture2DArray<float4> outputTexture : register(u0);

SamplerState Sam_Clamp : register(s0);

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

[numthreads(32, 32, 1)]
void CS_EquirectToCube(uint3 ThreadID : SV_DispatchThreadID)
{
    float3 samVec = GetSamplingVector(ThreadID);
    
    float longitude = atan2(samVec.x, samVec.z) * (180.f / PI);
    
    float latitude = asin(samVec.y) * (180.f / PI);
    
    float4 color = inputTexture.SampleLevel(Sam_Clamp, float2(longitude, latitude) / float2(360.f, -180.f) + 0.5, 0);

    outputTexture[ThreadID] = color;

    // ------------------- 실패버전
    // float2 UV = GetSamplingUV(ThreadID);
    
    // float phi;
    
    // float theta;
    
    // float4 color;
    
    //// X Positive
    //if (ThreadID.z == 0)
    //{
    //    //phi = PI * 1.5f + atan(UV.x);
        
    //    //theta = PI * 0.25f + acos(UV.y) * 0.5f;
        
    //    //color = inputTexture.SampleLevel(Sam_Clamp, float2(phi / TwoPI, theta / PI), 0);

    //    //outputTexture[ThreadID] = color;
    //}
    //// X Negative
    //else if (ThreadID.z == 1)
    //{
    //    phi = PI * 0.5f + atan(UV.x);
    
    //    theta = PI * 0.25f + acos(UV.y) * 0.5f;
        
    //    color = inputTexture.SampleLevel(Sam_Clamp, float2(phi / TwoPI, theta / PI), 0);

    //    outputTexture[ThreadID] = color;
    //}
    //// Y Positive
    //else if (ThreadID.z == 2)
    //{
        
    //}
    //// Y Negative
    //else if (ThreadID.z == 3)
    //{
    //    //// 수평각 (왼쪽 부분 : -4분의 파이, 오른쪽 부분 : 4분의 파이)
    //    //phi = atan(UV.x);
                    
    //    //// 수직각
    //    //theta = acos(UV.y); // 밑에 부분 : 파이 ~ 윗 부분 : 0
       
    //    //// 좌측면
    //    //if (phi < 0.f)
    //    //{
    //    //    phi = 0.75f * PI - phi;
            
    //    //    // theta => 파이 ~ 0까지
            
    //    //    theta = PI - theta;
    //    //}
    //    //// 우측면
    //    //else
    //    //{
    //    //    phi = 1.f * PI - phi;
            
    //    //    // 0에서 파이로 상승 중
    //    //    theta = PI + theta;
    //    //}
        
    //    //color = inputTexture.SampleLevel(Sam_Clamp, float2(theta / TwoPI, phi / PI), 0);

    //    //outputTexture[ThreadID] = color;
    //}
    //// Z Positive
    //else if (ThreadID.z == 4)
    //{
    //    phi = PI + atan(UV.x);
    
    //    theta = PI * 0.25f + acos(UV.y) * 0.5f;
          
    //    color = inputTexture.SampleLevel(Sam_Clamp, float2(phi / TwoPI, theta / PI), 0);

    //    outputTexture[ThreadID] = color;
    //}
    //// Z Negative
    //else if (ThreadID.z == 5)
    //{
    //    //// 이것은 UV 좌표를 4분의 파이와 -4분의 파이 사이에 투영한 것
    //    //phi = atan(UV.x);
        
    //    //// Back 중 좌측 면
    //    //if (phi < 0.f)
    //    //    phi += 2.f * PI;
        
    //    //theta = PI * 0.25f + acos(UV.y) * 0.5f;
        
    //    //color = inputTexture.SampleLevel(Sam_Clamp, float2(phi / TwoPI, theta / PI), 0);

    //    //outputTexture[ThreadID] = color;
    //}
    //else
    //{
    //    outputTexture[ThreadID] = float4(0.f, 0.f, 0.f, 1.f);
    //}
}