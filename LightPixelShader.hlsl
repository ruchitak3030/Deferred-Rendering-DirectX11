Texture2D Texture							: register(t0);
Texture2D NormalMap							: register(t1);
Texture2D PositionTexture					: register(t2);
SamplerState Sampler						: register(s0);

cbuffer LightData		: register(b0)
{
	float4 DirLightColor;
	float3 DirLightDirection;
	float4 pointLightPosition;
	float3 pointLightColor;
	float3 spotLightDirection;
	float spotPower;
}


float4 main(in float4 screenPosition : SV_POSITION) : SV_TARGET0
{
	float3 normal;
    float3 position;
	float3 diffuse;

	int3 sampleIndices = int3(screenPosition.xy, 0);

	normal = NormalMap.Load(screenPosition).xyz;
	position = PositionTexture.Load(screenPosition).xyz;
	diffuse = Texture.Load(screenPosition).xyz;


	//Direction Light calculation
	float3 dirLightAmount = saturate(dot(normal, -normalize(DirLightDirection)));
	
	float3 dirLight = DirLightColor * dirLightAmount * diffuse;

	//Point Light calculation
	float3 dirToPointLight = normalize(pointLightPosition - position);
	float pointLightAmount = saturate(dot(normal, dirToPointLight));

	float3 pointLight = pointLightAmount * pointLightColor * diffuse;

	//SpotLight calculation
	float angleFromCenter = max(dot(dirToPointLight, spotLightDirection), 0.0f);
	float spotAmount = pow(angleFromCenter, spotPower);

	float3 spotLight = spotAmount * pointLightColor * diffuse;


	return float4(dirLight + pointLight + spotLight);
}