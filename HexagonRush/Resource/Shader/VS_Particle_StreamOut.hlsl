struct VS_IN_PARTICLE
{
    float3 _initPosition : POSITION;
    
    float3 _initVelocity : VELOCITY;
    
    float2 _size : SIZE;
    
    float _age : AGE;
    
    uint _type : TYPE;
};

VS_IN_PARTICLE VS_Particle_StreamOut(VS_IN_PARTICLE input)
{
    // �״�� ��ȯ�մϴ�. �� ģ���� ���ؼ� Stream Output Stage���� �ٷ��� ��ƼŬ�� �����մϴ�.
    return input;
}