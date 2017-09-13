#include "Renderer.h"

using namespace DirectX;


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context)
{



	pixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 0, 0));
	pixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.8f, 0.8f, 0.8f, 1));
	pixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 0, 0));
	pixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 0.3f, 0.3f, 1));
	pixelShader->SetFloat3("CameraPosition", camera->GetPosition());

	pixelShader->CopyAllBufferData();
	pixelShader->SetShader();


	ID3D11Buffer* vertexbuffer = gameEntity->GetMesh()->GetVertexBuffer();
	ID3D11Buffer* indexbuffer = gameEntity->GetMesh()->GetIndexBuffer();	

	context->IASetVertexBuffers(0, 1, &vertexbuffer, &stride, &offset);
	context->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);

	vertexShader->SetMatrix4x4("world", gameEntity->GetWorldMatrix());
	vertexShader->SetMatrix4x4("view", camera->GetView());
	vertexShader->SetMatrix4x4("projection", camera->GetProjection());


	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();

	context->DrawIndexed(gameEntity->GetMesh()->GetIndexCount(), 0, 0);



}



 