#define MAX_LIGHTS 8

//Light types
#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
SamplerState Sampler	: register(s0);

struct _Material
{
	float4 Emissive;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float SpecularPower;
	bool UseTexture;
};

cbuffer MaterialProperties : register(b0)
{
	_Material material;
}

struct Light
{
	float4 Position;
	float4 Direction;
	float4 Color;
	float SpotAngle;
	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;
	int LightType;
	bool Enabled;
};

cbuffer LightProperties : register(b1)
{
	/*float4 DirLightColor;
	float3 DirLightDirection;
	
	float4 PointLightColor;
	float3 PointLightPosition;

	float3 CameraPosition;*/
	float4 CameraPosition;
	float4 GlobalAmbient;
	Light Lights[MAX_LIGHTS];

};

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};


float4 CalculateDiffuse(Light light, float3 L, float3 N)
{
	float NdotL = max(0, dot(N, L));
	return light.Color * NdotL;
}

float4 CalculateSpecular(Light light, float3 V, float3 L, float3 N)
{
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(R, V));

	float3 H = normalize(L + V);
	float NdotH = max(, dot(N, H));

	return light.Color * pow(RdotV, Material.SpecularPower);
}

float CalculateAttenuation(Light light, float d)
{
	return 1.0f / (light.ConstantAttenuation + light.LinearAttenuation*d + light.QuadraticAttenuation*d*d);
}

LightingResult CalculatePointLight(Light light, float3 V, float4 P, float3 N)
{
	LighttingResult result;

	float3 L = (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = CalculateAttenuation(light, distance);

	result.Diffuse = CalculateDiffuse(light, L, N) * attenuation;
	result.Specular = CalculateSpecular(light, V, L, N) * attenuation;

	return result;
}

LightingResult CalculateDirectionalLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;
	float3 L = -light.Direction.xyz;
	result.Diffuse = CalculateDiffuse(light, L, N);
	result.Specular = CalculateSpecular(light, V, L, N);

	return result;
}

float CalculateSpotCone(Light light, float3 L)
{
	float minCos = cos(light.SpotAngle);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(light.Direction.xyz, -L);

	return smoothstep(minCos, maxCos, cosAngle);

}

LightingResult CalculateSpotLight(Light light, float3 V, float4 P, float3 N)
{
	LightingResult result;

	float3 (light.Position - P).xyz;
	float distance = length(L);
	L = L / distance;

	float attenuation = CalculateAttenuation(light, distance);
	float spotIntensity = CalculateSpotCone(light, L);

	result.Diffuse = CalculateDiffuse(light, L, N)*attnuation* spotIntensity;
	result.Specular = CalculateSpecular(light, V, L, N)* attenuation*spotIntensity;

	return result;
}

LightingResult ComputeLighting(float4 P, float3 N)
{
	float3 V = normalize(CameraPosition - P).xyz;

	LightingResult totalResult = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

	[unroll]
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		LightingResult result = { { 0, 0, 0, 0 },{ 0, 0, 0, 0 } };

		if (!Lights[i].Enabled) continue;

		switch (Lights[i].LightType)
		{
		case DIRECTIONAL_LIGHT:
		{
			result = CalculateDirectionalLight(Lights[i], V, P, N);
		}
		break;
		case POINT_LIGHT:
		{
			result = CalculatePointLight(Lights[i], V, P, N);
		}
		break;
		case SPOT_LIGHT:
		{
			result = CalculateSpotLight(Lights[i], V, P, N);
		}
		break;
		}
		totalResult.Diffuse += result.Diffuse;
		totalResult.Specular += result.Specular;
	}

	totalResult.Diffuse = saturate(totalResult.Diffuse);
	totalResult.Specular = saturate(totalResult.Specular);

	return totalResult;
}

float4 main(VertexToPixel input) : SV_TARGET
{
	LightingResult lit = ComputeLighting(input.position, normalize(input.normal));

	float4 emissive = material.Emissive;
	float4 ambient = material.Ambient * GlobalAmbient ;
	float4 diffuse = material.Diffuse * lit.Diffuse;
	float4 specular = material.Specular * lit.Specular;

	float4 texColor = { 1,1,1,1 };
	if (material.UseTexture)
	{
		texColor = Texture.Sample(Sampler, input.uv);
	 }

	float4 finalColor = (emissive + ambient + diffuse + specular) * texColor;

	return finalColor;

	//input.normal = normalize(input.normal);
	//input.tangent = normalize(input.tangent);

	//// Read and unpack normal from map
	//float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).xyz * 2 - 1;

	//// Transform from tangent to world space
	//float3 N = input.normal;
	//float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	//float3 B = cross(T, N);

	//float3x3 TBN = float3x3(T, B, N);
	//input.normal = normalize(mul(normalFromMap, TBN));


	//// Directional light calculation ---------------

	//float dirLightAmount = saturate(dot(input.normal, -normalize(DirLightDirection)));

	//// Point light calculation ---------------------
	//
	//float3 dirToPointLight = normalize(PointLightPosition - input.worldPos);
	//float pointLightAmount = saturate(dot(input.normal, dirToPointLight));

	//// Specular (for point light) ------------------

	//float3 toCamera = normalize(CameraPosition - input.worldPos);
	//float3 refl = reflect(-dirToPointLight, input.normal);
	//float spec = pow(max(dot(refl, toCamera), 0), 32);

	//// Sample the texture
	//float4 textureColor = Texture.Sample(Sampler, input.uv);

	//return (DirLightColor * dirLightAmount * textureColor) +	// Directional light
	//	(PointLightColor * pointLightAmount * textureColor) + 	// Point light
	//	spec;													// Specular
}