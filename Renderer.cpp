#include "Renderer.h"

using namespace DirectX;


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::SetLights()
{
	pointLight1.SetpointLightValues(XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0), XMFLOAT3(-3, 0, 0));
	pointLight2.SetpointLightValues(XMFLOAT4(0.3f, 0.3f, 1.0f, 1.0f), XMFLOAT3(3, 0, 0));
}

void Renderer::SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context)
{
	SetLights();
	pixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 0, 0));
	pixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));
	pixelShader->SetData("pointLight1", &pointLight1, sizeof(PointLight));
	pixelShader->SetData("pointLight2", &pointLight2, sizeof(PointLight));
	pixelShader->SetFloat3("CameraPosition", camera->GetPosition());

	pixelShader->SetSamplerState("Sampler", gameEntity->GetMaterial()->GetSamplerState());
	pixelShader->SetShaderResourceView("Texture", gameEntity->GetMaterial()->GetTextureSRV());
	pixelShader->SetShaderResourceView("NormalMap", gameEntity->GetMaterial()->GetNormalMapSRV());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();

	vb = gameEntity->GetMesh()->GetVertexBuffer();
	ib = gameEntity->GetMesh()->GetIndexBuffer();

	context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetMatrix4x4("world", gameEntity->GetWorldMatrix());
	vertexShader->CopyAllBufferData();

	context->DrawIndexed(gameEntity->GetMesh()->GetIndexCount(), 0, 0);
}



 