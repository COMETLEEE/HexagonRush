// Light Informations
struct DirectionalLightInfo
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;
    
    float3 _direction;
    float _pad;
};

struct PointLightInfo
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;
    
    float3 _position;
    float _range;
    
    float3 _attenuation;
    float _pad;
};

struct SpotLightInfo
{
    float4 _ambient;
    float4 _diffuse;
    float4 _specular;
    
    float3 _position;
    float _range;
    
    float3 _direction;
    float _spot;
    
    float3 _attenuation;
    float _pad;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflect,
							 DirectionalLightInfo L,
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L._direction;

	// Add ambient term.
    ambient = Ambient * L._ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), Specular.w);
					
        diffuse = diffuseFactor * Diffuse * L._diffuse;
        spec = specFactor * Specular * L._specular;
    }
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflect,
					PointLightInfo L, float3 pos, float3 normal, float3 toEye,
					out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = L._position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if (d > L._range)
        return;
	
	// Ambient term.
    ambient = Ambient * L._ambient;
		
	// Normalize the light vector.
    lightVec /= d;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
    float diffusefactor = dot(lightVec, normal);

	//// flatten to avoid dynamic branching.
	[flatten]
    if (diffusefactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specfactor = pow(max(dot(v, toEye), 0.0f), Specular.w);
					
        diffuse = diffusefactor * diffuse * L._diffuse;
        spec = specfactor * Specular * L._specular;
    }

	// Attenuate
    float att = 1.0f / dot(L._attenuation, float3(1.0f, d, d * d));

    ambient *= att;
    diffuse *= att;
    spec *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(float4 Ambient, float4 Diffuse, float4 Specular, float4 Reflect,
					SpotLightInfo L, float3 pos, float3 normal, float3 toEye,
					out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = L._position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if (d > L._range)
        return;
		
	// Normalize the light vector.
    lightVec /= d;
	
	// Ambient term.
    ambient = Ambient * L._ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), Specular.w);
					
        diffuse = diffuseFactor * Diffuse * L._diffuse;
        spec = specFactor * Specular * L._specular;
    }
	
	// Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, L._direction), 0.0f), L._spot);

	// Scale by spotlight factor and attenuate.
    float att = spot / dot(L._attenuation, float3(1.0f, d, d * d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}