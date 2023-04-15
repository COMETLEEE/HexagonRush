// Copyright (c) 2022 Comet Lee
// ��������������� ���۵� �ĳ�� �̹����� 
// DirectX ��ǥ���� ť��� ���·� �����մϴ�.

static const float PI = 3.141592;

static const float TwoPI = 2 * PI;

Texture2D inputTexture : register(t0);

RWTexture2DArray<float4> outputTexture : register(u0);

SamplerState Sam_Clamp : register(s0);

float2 GetSamplingUV(uint3 ThreadID)
{
    float outputWidth, outputHeight, outputDepth;
    
    // Output Texture�� Desc ���
    outputTexture.GetDimensions(outputWidth, outputHeight, outputDepth);

    // Screen NDC
    float2 st = ThreadID.xy / float2(outputWidth, outputHeight);
    
    // -1 ~ 1 ���̷� ��ȯ (x : 0�̸� -1�� => 1�̸� 1 ��, y : 0�̸� 1�� => 1�̸� -1��)
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

    // ------------------- ���й���
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
    //    //// ���� (���� �κ� : -4���� ����, ������ �κ� : 4���� ����)
    //    //phi = atan(UV.x);
                    
    //    //// ������
    //    //theta = acos(UV.y); // �ؿ� �κ� : ���� ~ �� �κ� : 0
       
    //    //// ������
    //    //if (phi < 0.f)
    //    //{
    //    //    phi = 0.75f * PI - phi;
            
    //    //    // theta => ���� ~ 0����
            
    //    //    theta = PI - theta;
    //    //}
    //    //// ������
    //    //else
    //    //{
    //    //    phi = 1.f * PI - phi;
            
    //    //    // 0���� ���̷� ��� ��
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
    //    //// �̰��� UV ��ǥ�� 4���� ���̿� -4���� ���� ���̿� ������ ��
    //    //phi = atan(UV.x);
        
    //    //// Back �� ���� ��
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