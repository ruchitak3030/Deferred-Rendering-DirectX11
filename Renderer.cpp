#include "Renderer.h"

using namespace DirectX;




Renderer::~Renderer()
{
}

void Renderer::SetLights()
{
	pointLight1.SetpointLightValues(XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0), XMFLOAT3(-3, 0, 0));
	pointLight2.SetpointLightValues(XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f	), XMFLOAT3(3, 0, 0));
	pointLight3.SetpointLightValues(XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0), XMFLOAT3(-1, 1, 0));
	pointLight4.SetpointLightValues(XMFLOAT4(0.3f, 0.0f, 0.7f, 1.0f), XMFLOAT3(1, 1, 0));

	spotLight1.SetSpotLightValues(XMFLOAT3(2,2,-2),0.5);
	spotLight2.SetSpotLightValues(XMFLOAT3(-2, 2, -5), 0.5);
}


//void Renderer::SetRenderer(std::vector<GameEntity*> entities, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, ID3D11DeviceContext* context, ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, ID3D11RasterizerState* rasterizer, ID3D11BlendState* blendState, ID3D11RenderTargetView* renderTargetViewArray[BUFFER_COUNT], ID3D11ShaderResourceView* shaderResourceViewArray[BUFFER_COUNT], SimpleVertexShader* &deferredVS, SimplePixelShader* &deferredPS, SimpleVertexShader* &lightVS, SimplePixelShader* &lightPS, GameEntity* lightEntity)
//{
//
//
//	const float color[4] = { 0,0,0,0 };
//	int i;
//
//	//Bind render target view array and depth stencil buffer to output render pipeline
//	context->OMSetRenderTargets(BUFFER_COUNT, renderTargetViewArray, depthStencilView);
//
//	//Clear the render target buffers
//	for (i = 0; i < BUFFER_COUNT; i++)
//	{
//		context->ClearRenderTargetView(renderTargetViewArray[i], color);
//	}
//
//	//Clear the depth buffer
//	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
//
//	for (int i = 0; i < entities.size(); i++)
//	{
//		ID3D11Buffer* vb = entities[i]->GetMesh()->GetVertexBuffer();
//		ID3D11Buffer* ib = entities[i]->GetMesh()->GetIndexBuffer();
//
//		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
//		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
//
//		deferredVS->SetMatrix4x4("world", entities[i]->GetWorldMatrix());
//		deferredVS->SetMatrix4x4("view", camera->GetView());
//		deferredVS->SetMatrix4x4("projection", camera->GetProjection());
//
//		deferredVS->CopyAllBufferData();
//		deferredVS->SetShader();
//
//		deferredPS->SetSamplerState("Sampler", entities[i]->GetMaterial()->GetSamplerState());
//		deferredPS->SetShaderResourceView("Texture", entities[i]->GetMaterial()->GetTextureSRV());
//		deferredPS->SetShaderResourceView("NormalMap", entities[i]->GetMaterial()->GetNormalMapSRV());
//
//		deferredPS->CopyAllBufferData();
//		deferredPS->SetShader();
//
//		context->DrawIndexed(entities[i]->GetMesh()->GetIndexCount(), 0, 0);
//	}
//	
//	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
//	context->ClearRenderTargetView(backBufferRTV, color);
//	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	context->RSSetState(rasterizer);
//	float blend[4] = { 1,1,1, 1 };
//	context->OMSetBlendState(blendState, blend, 0xFFFFFFFF);
//
//	ID3D11Buffer* vb1 = lightEntity->GetMesh()->GetVertexBuffer();
//	ID3D11Buffer* ib1 = lightEntity->GetMesh()->GetIndexBuffer();
//
//	context->IASetVertexBuffers(0, 1, &vb1, &stride, &offset);
//	context->IASetIndexBuffer(ib1, DXGI_FORMAT_R32_UINT, 0);
//
//	lightVS->SetMatrix4x4("world", lightEntity->GetWorldMatrix());
//	lightVS->SetMatrix4x4("view", camera->GetView());
//	lightVS->SetMatrix4x4("projection", camera->GetProjection());
//
//	lightVS->CopyAllBufferData();
//	lightVS->SetShader();
//
//
//
//	//lightPS->SetSamplerState("Sampler", sampler);
//	lightPS->SetShaderResourceView("NormalMap", shaderResourceViewArray[0]);
//	lightPS->SetShaderResourceView("Texture", shaderResourceViewArray[1]);
//	lightPS->SetShaderResourceView("PositionTexture", shaderResourceViewArray[2]);
//
//	lightPS->SetFloat3("CameraPosition", camera->GetPosition());
//	pointLight1.SetpointLightValues(XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0), XMFLOAT3(-3, 0, 0));
//	pointLight2.SetpointLightValues(XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT3(3, 0, 0));
//	/*lightPixelShader->SetFloat3("pointLightPosition", XMFLOAT3(-3, 0, 0));
//	lightPixelShader->SetFloat4("pointLightColor", XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0));*/
//	lightPS->SetData("pointLight1", &pointLight1, sizeof(PointLight));
//	lightPS->SetData("pointLight2", &pointLight2, sizeof(PointLight));
//
//	lightPS->CopyAllBufferData();
//	lightPS->SetShader();
//
//	context->DrawIndexed(lightEntity->GetMesh()->GetIndexCount(), 0, 0);
//
//	context->OMSetBlendState(NULL, blend, 0xFFFFFFFF);
//
	









	/*SetLights();
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

	context->DrawIndexed(gameEntity->GetMesh()->GetIndexCount(), 0, 0);*/
//}





 