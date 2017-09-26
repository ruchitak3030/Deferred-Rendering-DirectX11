Texture2D Texture							: register(t0);
Texture2D NormalMap							: register(t1);
Texture2D PositionTexture					: register(t2);
SamplerState Sampler						: register(s0);

cbuffer LightData		: register(b0)
{
	
	float4 pointLightPosition;
	float3 pointLightColor;
	
}

struct VertexToPixel
{
	float4 position		: SV_POSITION;

};

float4 main(VertexToPixel input) : SV_TARGET0
{
	float3 normal;
    float3 position;
	float3 diffuse;

	int3 sampleIndices = int3(input.position.xy, 0);

	normal = NormalMap.Load(sampleIndices).xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
	diffuse = Texture.Load(sampleIndices).xyz;


	

	//Point Light calculation
	float3 dirToPointLight = normalize(pointLightPosition - position);
	float pointLightAmount = saturate(dot(normal, dirToPointLight));

	float3 pointLight = pointLightAmount * pointLightColor * diffuse;

	

	return float4(pointLight, 1.0f);
}