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
	pointLight2.SetpointLightValues(XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT3(3, 0, 0));
	pointLight3.SetpointLightValues(XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0), XMFLOAT3(-1, 1, 0));
	pointLight4.SetpointLightValues(XMFLOAT4(0.3f, 0.0f, 0.7f, 1.0f), XMFLOAT3(1, 1, 0));

	spotLight1.SetSpotLightValues(XMFLOAT3(2,2,-2),0.5);
	spotLight2.SetSpotLightValues(XMFLOAT3(-2, 2, -5), 0.5);
}

void Renderer::SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context)
{
	SetLights();
	pixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 0, 0));
	pixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.3f, 0.3f, 0.3f, 1));

	pixelShader->SetData("pointLight1", &pointLight1, sizeof(PointLight));
	pixelShader->SetData("pointLight2", &pointLight2, sizeof(PointLight));
	pixelShader->SetData("pointLight3", &pointLight3, sizeof(PointLight));
	pixelShader->SetData("pointLight4", &pointLight4, sizeof(PointLight));

	pixelShader->SetData("spotLight1", &spotLight1, sizeof(SpotLight));
	pixelShader->SetData("spotLight2", &spotLight2, sizeof(SpotLight));

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



 