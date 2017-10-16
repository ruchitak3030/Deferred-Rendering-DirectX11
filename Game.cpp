#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h" // From DirectX Tool Kit

// For the DirectX Math library
using namespace DirectX;


Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;
	camera = 0;

	int i;
	for (i = 0; i < 3; i++)
	{
		renderTargetViewArray[i] = 0;
		shaderResourceViewArray[i] = 0;
	}

	depthStencilBuffer = 0;
	depthStencilView = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}


Game::~Game()
{

	delete camera;

	delete pixelShader;
	delete vertexShader;

	for (auto& m : meshes) delete m;

	delete sphereMaterial;
	delete planeMaterial;

	for (auto& e : entities) delete e;
	for (auto& le : lightEntities) delete le;

	sampler->Release();

	sphereTextureSRV->Release();
	sphereNormalMapSRV->Release();
	planeTextureSRV->Release();
	planeNormalMapSRV->Release();

	depthStencilBuffer->Release();
	depthStencilView->Release();
	rasterizer->Release();
	blendState->Release();


	for (int i = 0; i < 3; i++)
	{
		renderTargetViewArray[i]->Release();
		shaderResourceViewArray[i]->Release();
	}
		

	delete deferredPixelShader;
	delete deferredVertexShader;
	delete backBufferPixelShader;
	delete backBufferVertexShader;
	delete lightPixelShader;
	delete lightVertexShader;
	delete dirPixelShader;
	delete dirVertexShader;

	
	
}


void Game::Init()
{
	SetDefferedSetup(width, height);
	CreateMatrices();
	LoadShaders();
	LoadMesh();
	LoadTextures();	
	CreateMaterials();
	CreateBasicGeometry();	
	LightEntityInitialize();
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Game::SetDefferedSetup(int textureWidth, int textureHeight)
{
	int i;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;


	ID3D11Texture2D* renderTargetTextureArray[BUFFER_COUNT];

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);

	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc32;
	ZeroMemory(&renderTargetViewDesc32, sizeof(renderTargetViewDesc32));
	renderTargetViewDesc32.Format = textureDesc.Format;
	renderTargetViewDesc32.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc32.Texture2D.MipSlice = 0;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc8;
	ZeroMemory(&renderTargetViewDesc8, sizeof(renderTargetViewDesc8));
	renderTargetViewDesc8.Format = textureDesc.Format;
	renderTargetViewDesc8.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc8.Texture2D.MipSlice = 0;


	//Create Render target view
	device->CreateRenderTargetView(renderTargetTextureArray[0], &renderTargetViewDesc32, &renderTargetViewArray[0]);
	device->CreateRenderTargetView(renderTargetTextureArray[1], &renderTargetViewDesc32, &renderTargetViewArray[1]);
	device->CreateRenderTargetView(renderTargetTextureArray[2], &renderTargetViewDesc8, &renderTargetViewArray[2]);


	//Shader Resource View Description
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderResourceViewArray[i]);
	}

	//Release render target texture array
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		renderTargetTextureArray[i]->Release();
	}

	//Depth Buffer Description
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);

	//Depth Stencil Description
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);


	//Rasterizer setup
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.DepthClipEnable = false;

	device->CreateRasterizerState(&rasterizerDesc, &rasterizer);

	//Blend state setup
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&blendDesc, &blendState);

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	device->CreateDepthStencilState(&depthStencilDesc, &depthState);
}

void Game::CreateMatrices()
{
	camera = new Camera(0, 0, -5);
	camera->UpdateProjectionMatrix((float)width / height);
}


void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

	deferredVertexShader = new SimpleVertexShader(device, context);
	if (!deferredVertexShader->LoadShaderFile(L"Debug/DeferredVertexShader.cso"))
		deferredVertexShader->LoadShaderFile(L"DeferredVertexShader.cso");

	deferredPixelShader = new SimplePixelShader(device, context);
	if (!deferredPixelShader->LoadShaderFile(L"Debug/DeferredPixelShader.cso"))
		deferredPixelShader->LoadShaderFile(L"DeferredPixelShader.cso");

	backBufferVertexShader = new SimpleVertexShader(device, context);
	if (!backBufferVertexShader->LoadShaderFile(L"Debug/BackBufferVertexShader.cso"))
		backBufferVertexShader->LoadShaderFile(L"BackBufferVertexShader.cso");

	backBufferPixelShader = new SimplePixelShader(device, context);
	if (!backBufferPixelShader->LoadShaderFile(L"Debug/BackBufferPixelShader.cso"))
		backBufferPixelShader->LoadShaderFile(L"BackBufferPixelShader.cso");

	lightVertexShader = new SimpleVertexShader(device, context);
	if (!lightVertexShader->LoadShaderFile(L"Debug/LightVertexShader.cso"))
		lightVertexShader->LoadShaderFile(L"LightVertexShader.cso");

	lightPixelShader = new SimplePixelShader(device, context);
	if (!lightPixelShader->LoadShaderFile(L"Debug/LightPixelShader.cso"))
		lightPixelShader->LoadShaderFile(L"LightPixelShader.cso");

	dirVertexShader = new SimpleVertexShader(device, context);
	if (!dirVertexShader->LoadShaderFile(L"Debug/DirVertexShader.cso"))
		dirVertexShader->LoadShaderFile(L"DirVertexShader.cso");

	dirPixelShader = new SimplePixelShader(device, context);
	if (!dirPixelShader->LoadShaderFile(L"Debug/DirPixelShader.cso"))
		dirPixelShader->LoadShaderFile(L"DirPixelShader.cso");
	
}

void Game::LoadMesh()
{
	sphereMesh = new Mesh("Models/sphere.obj", device);
	planeMesh = new Mesh("Models/cube.obj", device);
}

void Game::LoadTextures()
{
	// Load texture stuff
	CreateWICTextureFromFile(device, context, L"Debug/Textures/rock.jpg", 0, &sphereTextureSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Textures/rockNormals.jpg", 0, &sphereNormalMapSRV);

	CreateWICTextureFromFile(device, context, L"Debug/Textures/plane.tiff", 0, &planeTextureSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Textures/planeNormals.tiff", 0, &planeNormalMapSRV);
}




void Game::CreateMaterials()
{
	

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);


	sphereMaterial = new Material(vertexShader, pixelShader, sphereTextureSRV, sphereNormalMapSRV, sampler);
	planeMaterial = new Material(vertexShader, pixelShader, planeTextureSRV, planeNormalMapSRV, sampler);
}



void Game::CreateBasicGeometry()
{
	GameEntity* sphere = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* sphere1 = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* sphere2 = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* sphere3 = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* sphere4 = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* sphere5 = new GameEntity(sphereMesh, sphereMaterial);
	GameEntity* plane1 = new GameEntity(planeMesh, planeMaterial);
	GameEntity* plane2 = new GameEntity(planeMesh, planeMaterial);
	GameEntity* plane3 = new GameEntity(planeMesh, planeMaterial);
	GameEntity* plane4 = new GameEntity(planeMesh, planeMaterial);

	entities.push_back(sphere);
	entities.push_back(sphere1);
	entities.push_back(sphere2);
	entities.push_back(sphere3);
	entities.push_back(sphere4);
	entities.push_back(sphere5);
	entities.push_back(plane1);
	entities.push_back(plane2);
	entities.push_back(plane3);
	entities.push_back(plane4);	


	sphere->SetScale(1.0f, 1.0f, 1.0f);
	sphere->SetPosition(2.0f, -2.0f, 0.0f);

	sphere1->SetScale(1.0f, 1.0f, 1.0f);
	sphere1->SetPosition(2.0f, -2.0f, 2.0f);

	sphere2->SetScale(1.0f, 1.0f, 1.0f);
	sphere2->SetPosition(0.0f, -2.0f, 0.0f);

	sphere3->SetScale(1.0f, 1.0f, 1.0f);
	sphere3->SetPosition(0.0f, -2.0f, 2.0f);

	sphere4->SetScale(1.0f, 1.0f, 1.0f);
	sphere4->SetPosition(-2.0f, -2.0f, 0.0f);

	sphere5->SetScale(1.0f, 1.0f, 1.0f);
	sphere5->SetPosition(-2.0f, -2.0f, 2.0f);

	//Bottom
	plane1->SetScale(5.0f, 0.01f, 5.5f);
	plane1->SetPosition(0.0f, -3.5f, 0.0f);

	//Right
	plane2->SetScale(0.01f, 3.5f, 5.5f);
	plane2->SetPosition(2.5f, -2.0f, 0.5f);

	//Left
	plane3->SetScale(0.01f, 3.5f, 5.5f);
	plane3->SetPosition(-2.5f, -2.0f, 0.5f);

	//Back
	plane4->SetScale(5.0f, 3.5f, 0.01f);
	plane4->SetPosition(0.0f, -2.0f, 3.0f);	

	currentEntity = 0;

	
	
}



void Game::LightEntityInitialize()
{

	lightEntity1 = new GameEntity(sphereMesh, XMFLOAT3(1.0f, 0.3f, 0.3f));
	lightEntity1->SetPosition(-3, 0, 0);
	lightEntity1->SetScale(5, 5, 5);

	lightEntity2 = new GameEntity(sphereMesh, XMFLOAT3(0.5f, 1.0f, 0.5f));
	lightEntity2->SetPosition(3, 0, 0);
	lightEntity2->SetScale(5, 5, 5);

	lightEntity3 = new GameEntity(sphereMesh, XMFLOAT3(0.3f, 0.0f, 1.0f));
	lightEntity3->SetPosition(-1, 1, 0);
	lightEntity3->SetScale(5, 5, 5);

	lightEntity4 = new GameEntity(sphereMesh, XMFLOAT3(0.3f, 0.0f, 0.7f));
	lightEntity4->SetPosition(1, 1, 0);
	lightEntity4->SetScale(5, 5, 5);

	lightEntity5 = new GameEntity(sphereMesh, XMFLOAT3(0.2, 0.3f, 0.8f));
	lightEntity5->SetPosition(-1, -1, -0);
	lightEntity5->SetScale(5, 5, 5);

	lightEntity6 = new GameEntity(sphereMesh, XMFLOAT3(0.8f, 0.0f, 0.5f));
	lightEntity6->SetPosition(1, -1, 0);
	lightEntity6->SetScale(5, 5, 5);

	lightEntity7 = new GameEntity(sphereMesh, XMFLOAT3(0.0f, 0.6f, 0.4f));
	lightEntity7->SetPosition(-2, 0, 1);
	lightEntity7->SetScale(5, 5, 5);

	lightEntity8 = new GameEntity(sphereMesh, XMFLOAT3(0.3f, 0.0f, 0.7f));
	lightEntity8->SetPosition(2, 0, 1);
	lightEntity8->SetScale(5, 5, 5);


	lightEntities.push_back(lightEntity1);
	lightEntities.push_back(lightEntity2);
	lightEntities.push_back(lightEntity3);
	lightEntities.push_back(lightEntity4);
	lightEntities.push_back(lightEntity5);
	lightEntities.push_back(lightEntity6);
	lightEntities.push_back(lightEntity7);
	lightEntities.push_back(lightEntity8);
}



void Game::OnResize()
{
	
	DXCore::OnResize();

	
	if( camera ) 
		camera->UpdateProjectionMatrix((float)width / height);
}


void Game::Update(float deltaTime, float totalTime)
{
	
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	
	camera->Update(deltaTime);

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->UpdateWorldMatrix();
	}

	lightEntity1->UpdateWorldMatrix();
	lightEntity2->UpdateWorldMatrix();
	lightEntity3->UpdateWorldMatrix();
	lightEntity4->UpdateWorldMatrix();
	lightEntity5->UpdateWorldMatrix();
	lightEntity6->UpdateWorldMatrix();
	lightEntity7->UpdateWorldMatrix();
	lightEntity8->UpdateWorldMatrix();
		
}

void Game::Draw(float deltaTime, float totalTime)
{
	const float color[4] = {0,0,0,0};
	int i;
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	

	//Bind render target view array and depth stencil buffer to output render pipeline
	context->OMSetRenderTargets(BUFFER_COUNT, renderTargetViewArray, depthStencilView);

	//Clear the render target buffers
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		context->ClearRenderTargetView(renderTargetViewArray[i], color);
	}

	//Clear the depth buffer
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);	
	
	
	for (int i = 0; i < entities.size(); i++)
	{
		ID3D11Buffer* vb = entities[i]->GetMesh()->GetVertexBuffer();
		ID3D11Buffer* ib = entities[i]->GetMesh()->GetIndexBuffer();


		deferredVertexShader->SetMatrix4x4("world", entities[i]->GetWorldMatrix());
		deferredVertexShader->SetMatrix4x4("view", camera->GetView());
		deferredVertexShader->SetMatrix4x4("projection", camera->GetProjection());


		deferredVertexShader->CopyAllBufferData();
		deferredVertexShader->SetShader();

		deferredPixelShader->SetSamplerState("Sampler", entities[i]->GetMaterial()->GetSamplerState());
		deferredPixelShader->SetShaderResourceView("Texture", entities[i]->GetMaterial()->GetTextureSRV());
		deferredPixelShader->SetShaderResourceView("NormalMap", entities[i]->GetMaterial()->GetNormalMapSRV());

		deferredPixelShader->CopyAllBufferData();
		deferredPixelShader->SetShader();

		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
		

		context->DrawIndexed(entities[i]->GetMesh()->GetIndexCount(), 0, 0);
	}
	
	context->OMSetRenderTargets(1,&backBufferRTV, depthStencilView);
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f,0);

	context->RSSetState(rasterizer);
	float blend[4] = { 1,1,1, 1 };
	context->OMSetBlendState(blendState, blend, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthState, 0);

	dirVertexShader->SetShader();

	dirPixelShader->SetShaderResourceView("Position", shaderResourceViewArray[0]);
	dirPixelShader->SetShaderResourceView("NormalMap", shaderResourceViewArray[1]);
	dirPixelShader->SetShaderResourceView("Texture", shaderResourceViewArray[2]);
	backBufferPixelShader->SetSamplerState("Sampler", sampler);

	dirPixelShader->SetFloat3("lightColor", XMFLOAT3(0.5f, 0.5f, 0.5f));
	dirPixelShader->SetFloat3("lightDir", XMFLOAT3(10.0f, -10.0f, 0.0f));

	dirPixelShader->CopyAllBufferData();
	dirPixelShader->SetShader();

	ID3D11Buffer* nothing = 0;
	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	context->Draw(3, 0);

	for (int i = 0; i < 8; i++)
	{

		ID3D11Buffer* vb1 = lightEntities[i]->GetMesh()->GetVertexBuffer();
		ID3D11Buffer* ib1 = lightEntities[i]->GetMesh()->GetIndexBuffer();

		context->IASetVertexBuffers(0, 1, &vb1, &stride, &offset);
		context->IASetIndexBuffer(ib1, DXGI_FORMAT_R32_UINT, 0);

		lightVertexShader->SetMatrix4x4("world", lightEntities[i]->GetWorldMatrix());
		lightVertexShader->SetMatrix4x4("view", camera->GetView());
		lightVertexShader->SetMatrix4x4("projection", camera->GetProjection());

		lightVertexShader->CopyAllBufferData();
		lightVertexShader->SetShader();

		lightPixelShader->SetShaderResourceView("NormalMap", shaderResourceViewArray[0]);
		lightPixelShader->SetShaderResourceView("Texture", shaderResourceViewArray[1]);
		lightPixelShader->SetShaderResourceView("PositionTexture", shaderResourceViewArray[2]);

		lightPixelShader->SetSamplerState("basicSampler", sampler);

		lightPixelShader->SetFloat3("cameraPosition", camera->GetPosition());
		
		lightPixelShader->SetFloat3("lightColor", lightEntities[i]->GetLightColor());
		lightPixelShader->SetFloat3("lightPosition", lightEntities[i]->GetPosition());


		lightPixelShader->CopyAllBufferData();
		lightPixelShader->SetShader();

		context->DrawIndexed(lightEntities[i]->GetMesh()->GetIndexCount(), 0, 0);

		
	}
	
	context->RSSetState(NULL);
	context->OMSetBlendState(NULL, blend, 0xFFFFFFFF);
	context->OMSetDepthStencilState(NULL, 0);
//	backBufferVertexShader->SetShader();
//	backBufferPixelShader->SetShaderResourceView("Texture", shaderResourceViewArray[0]);
//	backBufferPixelShader->SetSamplerState("Sampler", sampler);
//	backBufferPixelShader->CopyAllBufferData();
//	backBufferPixelShader->SetShader();
//
//	ID3D11Buffer* nothing = 0;
//	context->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
//	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
//
//	 Actually draw exactly 3 vertices
//	context->Draw(3, 0);
//
//	 Unbind the post process texture from input
//	backBufferPixelShader->SetShaderResourceView("Texture", 0);

	//for (int i = 0; i < entities.size(); i++)
	//{
	//	renderer.SetRenderer(entities[i], camera, vertexBuffer, indexBuffer, vertexShader, pixelShader, context);
	//}
	//*/

	//renderer.SetRenderer(entities, camera, vertexBuffer, indexBuffer,  context,backBufferRTV,depthStencilView,rasterizer, blendState, &renderTargetViewArray[BUFFER_COUNT], &shaderResourceViewArray[BUFFER_COUNT], deferredVertexShader, deferredPixelShader, lightVertexShader, lightPixelShader, lightEntity);

	swapChain->Present(0, 0);
}

#pragma region Mouse Input

void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	
	prevMousePos.x = x;
	prevMousePos.y = y;

	SetCapture(hWnd);
}


void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	
	ReleaseCapture();
}


void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	
	if (buttonState & 0x0001)
	{
		float xDiff = (x - prevMousePos.x) * 0.005f;
		float yDiff = (y - prevMousePos.y) * 0.005f;
		camera->Rotate(yDiff, xDiff);
	}

	prevMousePos.x = x;
	prevMousePos.y = y;
}


void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion