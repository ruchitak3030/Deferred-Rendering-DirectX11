struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv           : TEXCOORD0;
};

Texture2D Texture		: register(t0);
SmaplerState Sampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
	float4 totalColor = Texture.Sample(Sampler, input.uv);
	return totalColor;
}