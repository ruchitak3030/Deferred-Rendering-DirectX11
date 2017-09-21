Texture2D DiffuseMap			: register(t0);
Texture2D NormalMap				: register(t1);
SamplerState AnisoSampler		: register(s0);

cbuffer MatProperties
{
	float3 SpecularAlbedo;
	float SpecularPower;
};

struct VertexToPixel
{
	float4 positionCS			: SV_Position;
	float2 uv					: TEXCOORD;
	float3 normalWS				: NORMALWS;
	float3 positionWS			: POSITIONWS;
	float3 TangentWS			: TANGENTWS;
	float3 BitangentWS			: BITANGENTWS;
};

struct PixelShaderOutput
{
	float4 Normal				: SV_Target0;
	float4 DiffuseAlbedo		: SV_Target1;
	float4 SpecularAlbedo		: SV_Target2;
	float4 Position				: SV_Target3;
};

PixelShaderOutput main(VertexToPixel input)
{
	PixelShaderOutput output;

	//Sample the diffuse map
	float3 diffuseAlbedo = DiffuseMap.Sample(AnisoSampler, input.uv).rgb;

	//Normalize the tangent frame after interpolation
	float3x3 tangentFrameWS = float3x3(normalize(input.TangentWS), normalize(input.BitangentWS), normalize(input.normalWS));

	//Sample the tangent-space normal map and decompress
	float3 normalTS = NormalMap.Sample(AnisoSampler, input.uv).rgb;
	normalTS = normalize(normalTS*2.0f - 1.0f);

	//Convert to world space
	float3 normalWS = mul(normalTS, tangentFrameWS);

	//Output G-Buffer values
	output.Normal = float4(normalWS, 1.0f);
	output.DiffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.SpecularAlbedo = float4(SpecularAlbedo, SpecularPower);
	output.Position = float4(input.positionWS, 1.0f);

	return output;
}