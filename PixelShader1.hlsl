#define POINT_LIGHT 2;
#define SPOT_LIGHT 2;
#define DIRECTIONAL_LIGHT 1;
#define NUM_LIGHT 5;

struct Light
{
	float3 Position;
	float4 Direction;
	float4 Color;
	float spotPower;
	bool Enabled;
	uint Type;
};


struct LightResult
{
	float3 diffuse;
};

Texture2D Texture			: register(t0);
Texture2D NormalMap			: register(t1);
Texture2D PositionTexture	: register(t2);
StructuredBuffer<Light> lights	:	register(b0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;

};

LightResult ComputeDirectionalLight(float4 _direction, float3 _normal, float4 _color, float3 _diffuse, float3 position)
{
	LightResult result;
	float dirLightAmount = saturate(dot(_normal, -normalize(_direction)));

	result.diffuse = _color*dirLightAmount*_diffuse;
}

LightResult ComputePointLight(float3 _position, float3 _normal, float4 _color, float3 _diffuse, float3 position )
{
	LightResult result;

	float3 dirToPointLight = normalize(_position - position);
	float pointLightAmount = saturate(dot(_normal, dirToPointLight));

	result.diffuse = _color * pointLightAmount * _diffuse;
}

LightResult ComputeSpotLight(float4 _direction, float3 _position, float3 _normal, float4 _color, float3 _diffuse, float3 position)
{
	LightResult result;

	float3 dirToSpotLight = normalize(_position - position);
	float spotLightAmount = saturate(dot(_normal, dirToSpotLight));

	float angleFromCenter = max(dot(dirToSpotLight, _direction), 0.0f);
	float spotAmount = pow(angleFromCenter, spotPower);

	result.diffuse = spotLightAmount * spotAmount * _color;
}


float4 main(VertexToPixel input) : SV_TARGET0
{
	float3 normal;
	float3 position;
	float3 diffuse;
	LightResult totalResult;

	int3 sampleIndices = int3(input.position.xy, 0);

	normal = NormalMap.Load(sampleIndices).xyz;
	position = PositionTexture.Load(sampleIndices).xyz;
	diffuse = Texture.Load(sampleIndices).xyz;

	for (int i = 0; i < NUM_LIGHT; i++)
	{
		LightResult result;
		switch (lights[i].Type)
		{
		case DIRECTIONAL_LIGHT:
		{
			result.diffuse = ComputeDirectionalLight(lights[i].Direction, normal, lights[i].Color, diffuse, position);
		}
		break;

		case POINT_LIGHT:
		{
			result.diffuse = ComputePointLight(lights[i].Position, normal, lights[i].Color, diffuse, position);
		}
		break;

		case SPOT_LIGHT:
		{
			result.diffuse = ComputeSpotLight(lights[i].Direction, lights[i].Position, normal, lights[i].Color, diffuse, position);
		}
		break;
		}
		totalResult.diffuse += result.diffuse;
	}
	
	return float4(totalResult.diffuse, 1.0f);
}