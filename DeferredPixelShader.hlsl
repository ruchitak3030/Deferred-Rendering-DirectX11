// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
SamplerState Sampler	: register(s0);


struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMALWS;
	float3 tangent		: TANGENTWS;
	float3 worldPos		: POSITIONWS;
	float2 uv			: TEXCOORD;
};


struct PixelShaderOutput
{
	float4 Normal				: SV_Target0;
	float4 DiffuseAlbedo		: SV_Target1;
	float4 Position				: SV_Target2;
};

PixelShaderOutput main(VertexToPixel input)
{
	PixelShaderOutput output;

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

	output.Normal = float4(input.normal, 1.0f);
	output.DiffuseAlbedo = textureColor;
	output.Position = float4( input.worldPos, 1.0f);

	////Sample the diffuse map
	//float3 diffuseAlbedo = DiffuseMap.Sample(AnisoSampler, input.uv).rgb;

	////Normalize the tangent frame after interpolation
	//float3x3 tangentFrameWS = float3x3(normalize(input.TangentWS), normalize(input.BitangentWS), normalize(input.normalWS));

	////Sample the tangent-space normal map and decompress
	//float3 normalTS = NormalMap.Sample(AnisoSampler, input.uv).rgb;
	//normalTS = normalize(normalTS*2.0f - 1.0f);

	////Convert to world space
	//float3 normalWS = mul(normalTS, tangentFrameWS);

	////Output G-Buffer values
	//output.Normal = float4(normalWS, 1.0f);
	//output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	//output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	//output.Position = float4(input.positionWS, 1.0f);

	return output;
}