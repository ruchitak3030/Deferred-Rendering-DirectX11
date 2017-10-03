#include "Renderer.h"

using namespace DirectX;




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

//void Renderer::SetDefferedSetup(int textureWidth, int textureHeight, ID3D11Device* device)
//{
//	int i;
//
//	D3D11_TEXTURE2D_DESC textureDesc;
//	ZeroMemory(&textureDesc, sizeof(textureDesc));
//
//	textureDesc.Width = textureWidth;
//	textureDesc.Height = textureHeight;
//	textureDesc.MipLevels = 1;
//	textureDesc.ArraySize = 1;
//	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	textureDesc.SampleDesc.Count = 1;
//	textureDesc.Usage = D3D11_USAGE_DEFAULT;
//	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
//	textureDesc.CPUAccessFlags = 0;
//	textureDesc.MiscFlags = 0;
//	textureDesc.SampleDesc.Quality = 0;
//
//	for (i = 0; i < BUFFER_COUNT; i++)
//	{
//		device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);
//
//	}
//
//	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
//	renderTargetViewDesc.Format = textureDesc.Format;
//	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	renderTargetViewDesc.Texture2D.MipSlice = 0;
//
//	//Create Render target view
//	for (i = 0; i < BUFFER_COUNT; i++)
//	{
//		device->CreateRenderTargetView(renderTargetTextureArray[i], &renderTargetViewDesc, &renderTargetViewArray[i]);
//	}
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//	//Shader Resource View Description
//	shaderResourceViewDesc.Format = textureDesc.Format;
//	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//	shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//	for (i = 0; i < BUFFER_COUNT; i++)
//	{
//		device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderResourceViewArray[i]);
//	}
//
//
//	//Depth Buffer Description
//	D3D11_TEXTURE2D_DESC depthBufferDesc;
//	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
//	depthBufferDesc.Width = textureWidth;
//	depthBufferDesc.Height = textureHeight;
//	depthBufferDesc.MipLevels = 1;
//	depthBufferDesc.ArraySize = 1;
//	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	depthBufferDesc.SampleDesc.Count = 1;
//	depthBufferDesc.SampleDesc.Quality = 0;
//	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
//	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
//	depthBufferDesc.CPUAccessFlags = 0;
//	depthBufferDesc.MiscFlags = 0;
//
//	device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
//
//	//Depth Stencil Description
//	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
//	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
//	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//	depthStencilViewDesc.Texture2D.MipSlice = 0;
//
//	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &dx->depthStencilView);
//
//
//	//Rasterizer setup
//	D3D11_RASTERIZER_DESC rasterizerDesc;
//	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
//	rasterizerDesc.CullMode = D3D11_CULL_NONE;
//	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
//	rasterizerDesc.DepthClipEnable = false;
//
//	device->CreateRasterizerState(&rasterizerDesc, &rasterizer);
//
//	//Blend state setup
//	D3D11_BLEND_DESC blendDesc;
//	ZeroMemory(&blendDesc, sizeof(blendDesc));
//	blendDesc.AlphaToCoverageEnable = false;
//	blendDesc.IndependentBlendEnable = false;
//	blendDesc.RenderTarget[0].BlendEnable = true;
//	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
//	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
//	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
//	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
//	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
//
//	device->CreateBlendState(&blendDesc, &blendState);
//
//
//
//
//}

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





 