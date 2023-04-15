struct CameraInfo
{
    float3 _cameraWorldPos;
    
    float _pad;
    
    float4x4 _viewMatrix;
    
    float4x4 _projMatrix;
    
    float4x4 _prevViewMatrix;
    
    float4x4 _prevProjMatrix;
};

struct SSAOInfo
{
    float4 _offsetVectors[14];
    
    float4 _viewFrustumCorners[4];
    
    float _occlusionRadius;
    
    float _occlusionFadeStart;
    
    float _occlusionFadeEnd;
    
    float _surfaceEpsilon;
    
    uint _sampleCount;
    
    float3 _pad;
};

struct Material
{
    float _metallic;
    
    float _roughness;
    
    float2 _pad;        // For 16 byte
    
    float4 _emissive;   // 자체 광원 색상
};