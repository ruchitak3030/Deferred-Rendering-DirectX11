
cbuffer lightData : register(b0)
{
	float4 DirLightColor;
	float3 DirLightDirection;
	
	float4 PointLightColor;
	float3 PointLightPosition;

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


	// Directional light calculation ---------------

	float dirLightAmount = saturate(dot(input.normal, -normalize(DirLightDirection)));

	// Point light calculation ---------------------
	
	float3 dirToPointLight = normalize(PointLightPosition - input.worldPos);
	float pointLightAmount = saturate(dot(input.normal, dirToPointLight));

	// Specular (for point light) ------------------

	float3 toCamera = normalize(CameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float spec = pow(max(dot(refl, toCamera), 0), 32);

	// Sample the texture
	float4 textureColor = Texture.Sample(Sampler, input.uv);

	return (DirLightColor * dirLightAmount * textureColor) +	// Directional light
		(PointLightColor * pointLightAmount * textureColor) + 	// Point light
		spec;													// Specular
}