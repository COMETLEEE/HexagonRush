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
    // 그대로 반환합니다. 이 친구를 통해서 Stream Output Stage에서 다량의 파티클을 생성합니다.
    return input;
}