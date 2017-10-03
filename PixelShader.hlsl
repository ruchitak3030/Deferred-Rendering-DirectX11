struct PointLight
{
	float4 Color;
	float3 Position;
	
};
struct SpotLight
{
	float3 Direction;
	float SpotPower;
};


cbuffer lightData : register(b0)
{
	float4 DirLightColor;
	float3 DirLightDirection;
	
	//point Light sources for spotlight
	PointLight pointLight1;
	PointLight pointLight2;

	PointLight pointLight3;
	PointLight pointLight4;

	SpotLight spotLight1;
	SpotLight spotLight2;	


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

	float3 dirToPointLight3 = normalize(pointLight3.Position - input.worldPos);
	float pointLightAmount3 = saturate(dot(input.normal, dirToPointLight3));

	float3 dirToPointLight4 = normalize(pointLight4.Position - input.worldPos);
	float pointLightAmount4 = saturate(dot(input.normal, dirToPointLight4));


	//SpotLight calculation------------------------
	float angleFromCenter1 = max(dot(dirToPointLight1, spotLight1.Direction), 0.0f);
	float spotAmount1 = pow(angleFromCenter1, spotLight1.SpotPower);

	float angleFromCenter2 = max(dot(dirToPointLight2, spotLight2.Direction), 0.0f);
	float spotAmount2 = pow(angleFromCenter2, spotLight2.SpotPower);

	

	// Specular (for point light) ------------------
	float3 toCamera = normalize(CameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight3, input.normal);
	float spec = pow(max(dot(refl, toCamera), 0), 32);

	float3 toCamera1 = normalize(CameraPosition - input.worldPo*positions);
	float3 refl1 = reflect(-dirToPointLight4, input.normal);
	float spec1 = pow(max(dot(refl1, toCamera1), 0), 32);


	//Total Directional Light
	float3 totalDirLight = DirLightColor * dirLightAmount * textureColor;

	//Total point Light
	float3 totalPointLight = (pointLight3.Color*pointLightAmount3*textureColor) + (pointLight4.Color*pointLightAmount4*textureColor);

	//Total Spotlight
	float3 totalSpotLight = (spotAmount1 * pointLight1.Color * textureColor) + (spotAmount2 * pointLight2.Color * textureColor);

	

	float3 totalLight = totalPointLight + totalSpotLight + spec + spec1;

	
	return float4(totalLight,1.0f);
}