cbuffer Transforms
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};


struct VertexShaderInput
{
	float4 position				: POSITION;
	float2 uv					: TEXCOORD;
	float3 normal				: NORMAL;
	float4 tangent				: TANGENT;
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

VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	//Convert positions and normals to world space
	output.positionWS = mul(input.position, WorldMatrix).xyz;
	output.normalWS = normalize(mul(input.normal, (float3x3)WorldMatrix));

	//reconstruct the tangent frame
	float3 tangentWS = normalize(mul(input.tangent.xyz, (float3x3)WorldMatrix));
	float3 bitangentWS = normalize(cross(normalize(mul(input.normal, (float3x3)WorldMatrix)), tangentWS))*input.tangent.w;

	output.TangentWS = tangentWS = tangentWS;
	output.BitangentWS = bitangentWS;

	matrix WorldViewProj = mul(mul(WorldMatrix, ViewMatrix), ProjectionMatrix);

	//calculate the clip-space position
	output.positionCS = mul(input.position, WorldViewProj);

	//Pass along the texture coordinates
	output.uv = input.uv;

	return output;
}
