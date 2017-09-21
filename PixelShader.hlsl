struct PointLight
{
	float4 Color;
	float3 Position;
	
};

struct SpotLight
{
	float4 Color;
	float3 Position;
	float3 Direction;
	
};

cbuffer lightData : register(b0)
{
	float4 DirLightColor;
	float3 DirLightDirection;
	
	PointLight pointLight1;
	PointLight pointLight2;

	SpotLight spotLight1;
	

	float3 CameraPosition;
};

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
SamplerState Sampler	: register(s0);


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};



float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Read and unpack normal from map
	float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).xyz * 2 - 1;

	// Transform from tangent to world space
	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);

	float3x3 TBN = float3x3(T, B, N);
	input.normal = normalize(mul(normalFromMap, TBN));

	// Sample the texture
	float4 textureColor = Texture.Sample(Sampler, input.uv);

	// Directional light calculation ---------------

	float dirLightAmount = saturate(dot(input.normal, -normalize(DirLightDirection)));

	// Point light calculation ---------------------
	float3 dirToPointLight1 = normalize(pointLight1.Position - input.worldPos);
	float pointLightAmount1 = saturate(dot(input.normal, dirToPointLight1));

	float3 dirToPointLight2 = normalize(pointLight2.Position - input.worldPos);
	float pointLightAmount2 = saturate(dot(input.normal, dirToPointLight2));

	

	//SpotLight calculation------------------------
	float3 dirToSpotLight1 = normalize(spotLight1.Position - input.worldPos);
	float spotLightAmount1 = saturate(dot(input.normal, dirToSpotLight1));

	float minCos = cos(5.0);
	float maxCos = (minCos + 1.0f) / 2.0f;
	float cosAngle = dot(spotLight1.Direction.xyz, -dirToSpotLight1);
	float spot = smoothstep(minCos, maxCos, cosAngle);

	/*float3 refl1 = reflect(-dirToSpotLight1, input.normal);
	float spotSpecular = pow(max(dot(refl1, normalize(CameraPosition - input.worldPos)), 0), 32);

	float spot = pow(max(dot(-dirToSpotLight1, spotLight1.Direction), 0.0f), 20);*/

	float4 finalLitColor = spotLight1.Color * spotLightAmount1 * textureColor * spot;

	// Specular (for point light) ------------------

	float3 toCamera = normalize(CameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight1, input.normal);
	float spec = pow(max(dot(refl, toCamera), 0), 32);

	

	//return (DirLightColor * dirLightAmount * textureColor) +	// Directional light
	//	(pointLight1.Color * pointLightAmount1 * textureColor) + (pointLight2.Color * pointLightAmount2 * textureColor) + spec+	// Point light
	//	finalLitColor;													// Specular
	
	return finalLitColor;
}