#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler);
	~Material();
	ID3D11ShaderResourceView* GetTextureSRV();
	ID3D11ShaderResourceView* GetNormalMapSRV();
	ID3D11SamplerState* GetSamplerState();

private:
	SimpleVertexShader* _vertexShader;
	SimplePixelShader* _pixelShader;
	ID3D11ShaderResourceView* _materialSRV;
	ID3D11ShaderResourceView* _materialNormal;
	ID3D11SamplerState* _materialSampler;


};

