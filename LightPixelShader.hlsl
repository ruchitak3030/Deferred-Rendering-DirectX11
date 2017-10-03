Texture2D Texture							: register(t0);
Texture2D NormalMap							: register(t1);
Texture2D PositionTexture					: register(t2);


struct PointLight
{
	float4 Color;
	float3 Position;
};
cbuffer LightData		: register(b0)
{
	
	PointLight pointLight1;
	PointLight pointLight2;
	
}

struct VertexToPixel
{
	float4 position		: SV_POSITION;

};

float4 main(VertexToPixel input) : SV_TARGET-
{
	float3 normal;
    float3 position;
	float3 diffuse;

	int3 sampleIndices = int3(input.position.xy, 0);

	normal = NormalMap.Load(sampleIndices).xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
	diffuse = Texture.Load(sampleIndices).xyz;

	//Point Light calculation
	float3 dirToPointLight = normalize(pointLight1.Position - position);
	float pointLightAmount = saturate(dot(normal, dirToPointLight));

	float3 dirToPointLight1 = normalize(pointLight2.Position - position);
	float pointLightAmount1 = saturate(dot(normal, dirToPointLight1));

	float3 pointLight = (pointLightAmount * pointLight1.Color * diffuse) + (pointLightAmount1 * pointLight2.Color * diffuse);
	

	return float4(pointLight, 1.0f);
}