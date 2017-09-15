#include "Material.h"



Material::Material(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader, ID3D11ShaderResourceView* materialSRV, ID3D11ShaderResourceView* materialNormal, ID3D11SamplerState* materialSampler)
{
	_vertexShader = vertexShader;
	_pixelShader = pixelShader;
	_materialSRV = materialSRV;
	_materialNormal = materialNormal;
	_materialSampler = materialSampler;
}


Material::~Material()
{
}

ID3D11ShaderResourceView * Material::GetTextureSRV()
{
	return _materialSRV;;
}

ID3D11ShaderResourceView * Material::GetNormalMapSRV()
{
	return _materialNormal;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return _materialSampler;
}
