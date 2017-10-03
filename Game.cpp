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

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}


Game::~Game()
{
	
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }
	sampler->Release();
	
	sphereTextureSRV->Release();
	sphereNormalMapSRV->Release();
	planeTextureSRV->Release();
	planeNormalMapSRV->Release();

	//Clean up render target stuff
	int i;

	if (depthStencilBuffer)
	{
		depthStencilBuffer->Release();
		depthStencilBuffer = 0;
	}

	
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		if (shaderResourceViewArray[i])
		{
			shaderResourceViewArray[i]->Release();
			shaderResourceViewArray[i] = 0;
		}

		if (renderTargetViewArray[i])
		{
			renderTargetViewArray[i]->Release();
			renderTargetViewArray[i] = 0;
		}

		if (renderTargetTextureArray[i])
		{
			renderTargetTextureArray[i]->Release();
			renderTargetTextureArray[i] = 0;
		}
	}

	rasterizer->Release();
	blendState->Release();

	delete sphereMaterial;
	delete planeMaterial;

	
	delete vertexShader;
	delete pixelShader;
	
	delete backBufferPixelShader;
	delete backBufferVertexShader;

	delete deferredPixelShader;
	delete deferredVertexShader;

	delete lightVertexShader;
	delete lightPixelShader;
	

	// Clean up resources
	delete lightEntity;
	
	for(auto& e : entities) delete e;
	for(auto& m : meshes) delete m;
	delete camera;
	
}


void Game::Init()
{
	
	LoadShaders();
	SetDefferedSetup(width, height);
	CreateMaterials();
	CreateMatrices();
	CreateBasicGeometry();
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Quality = 0;

	for ( i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateTexture2D(&textureDesc, NULL, &renderTargetTextureArray[i]);

	}

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	//Create Render target view
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateRenderTargetView(renderTargetTextureArray[i], &renderTargetViewDesc, &renderTargetViewArray[i]);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	//Shader Resource View Description
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	for (i = 0; i < BUFFER_COUNT; i++)
	{
		device->CreateShaderResourceView(renderTargetTextureArray[i], &shaderResourceViewDesc, &shaderResourceViewArray[i]);
	}


	//Depth Buffer Description
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = textureWidth;
	depthBufferDesc.Height = textureHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
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
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
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
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	device->CreateBlendState(&blendDesc, &blendState);
}


void Game::CreateMatrices()
{
	camera = new Camera(0, 0, -5);
	camera->UpdateProjectionMatrix((float)width / height);
}



void Game::CreateBasicGeometry()
{
	Mesh* sphereMesh = new Mesh("Models/sphere.obj", device);
	Mesh* sphereMesh1 = new Mesh("Models/sphere.obj", device);
	Mesh* sphereMesh2 = new Mesh("Models/sphere.obj", device);
	Mesh* sphereMesh3 = new Mesh("Models/sphere.obj", device);
	Mesh* sphereMesh4 = new Mesh("Models/sphere.obj", device);
	Mesh* sphereMesh5 = new Mesh("Models/sphere.obj", device);
	Mesh* planeMesh1 = new Mesh("Models/cube.obj", device);
	Mesh* planeMesh2 = new Mesh("Models/cube.obj", device);
	Mesh* planeMesh3 = new Mesh("Models/cube.obj", device);
	Mesh* planeMesh4 = new Mesh("Models/cube.obj", device);

	Mesh* lightMesh = new Mesh("Models/sphere.obj", device);

	meshes.push_back(sphereMesh);
	meshes.push_back(sphereMesh1);
	meshes.push_back(sphereMesh2);
	meshes.push_back(sphereMesh3);
	meshes.push_back(sphereMesh4);
	meshes.push_back(sphereMesh5);
	meshes.push_back(planeMesh1);
	meshes.push_back(planeMesh2);
	meshes.push_back(planeMesh3);
	meshes.push_back(planeMesh4);


		
	sphere = new GameEntity(sphereMesh, sphereMaterial);
	sphere1 = new GameEntity(sphereMesh1,sphereMaterial);
	sphere2 = new GameEntity(sphereMesh2, sphereMaterial);
	sphere3 = new GameEntity(sphereMesh3, sphereMaterial);
	sphere4 = new GameEntity(sphereMesh4, sphereMaterial);
	sphere5 = new GameEntity(sphereMesh5, sphereMaterial);
	plane1 = new GameEntity(planeMesh1, planeMaterial);
	plane2 = new GameEntity(planeMesh2, planeMaterial);
	plane3 = new GameEntity(planeMesh3, planeMaterial);
	plane4 = new GameEntity(planeMesh4, planeMaterial);

	lightEntity = new GameEntity(lightMesh, NULL);

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

	lightEntity->SetPosition(0, 2, 0);
	lightEntity->SetScale(1, 1, 1);

	currentEntity = 0;

	//Assign light values
	//spotlight 1
	light[0].Color = XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0);
	light[0].Position = XMFLOAT3(-3, 0, 0);
	light[0].Direction = XMFLOAT3(2, 2, -2);
	light[0].SpotPower = 0.5;

	//Spotlight 2
	light[1].Direction = XMFLOAT3(-2, 2, -5);
	light[1].Position = XMFLOAT3(3, 0, 0);
	light[1].Color = XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f);
	light[1].SpotPower = 0.5;


	//pointlight1
	light[2].Position = XMFLOAT3(-1, 1, 0);
	light[2].Color = XMFLOAT4(0.3f, 0.0f, 1.0f, 1.0);
	light[2].Direction = XMFLOAT3(0, 0, 0);
	light[2].SpotPower = 0;

	//pointlight2
	light[3].Position = XMFLOAT3(1, 1, 0);
	light[3].Color = XMFLOAT4(0.3f, 0.0f, 0.7f, 1.0f);
	light[3].Direction = XMFLOAT3(0, 0, 0);
	light[3].SpotPower = 0;


		
}

void Game::CreateMaterials()
{
	// Load texture stuff
	CreateWICTextureFromFile(device,context, L"Debug/Textures/rock.jpg", 0, &sphereTextureSRV);
	CreateWICTextureFromFile(device,context, L"Debug/Textures/rockNormals.jpg",0, &sphereNormalMapSRV);

	CreateWICTextureFromFile(device, context, L"Debug/Textures/plane.tiff", 0, &planeTextureSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Textures/planeNormals.tiff", 0, &planeNormalMapSRV);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &sampler);


	sphereMaterial = new Material(vertexShader, pixelShader, sphereTextureSRV, sphereNormalMapSRV, sampler);
	planeMaterial = new Material(vertexShader, pixelShader, planeTextureSRV, planeNormalMapSRV, sampler);
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
		
}

void Game::Draw(float deltaTime, float totalTime)
{
	const float color[4] = {0,0,0,0};
	int i;
	
	
	/*context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	



	
	vertexShader->SetShader();
	vertexShader->SetMatrix4x4("view", camera->GetView());
	vertexShader->SetMatrix4x4("projection", camera->GetProjection());*/

	//Bind render target view array and depth stencil buffer to output render pipeline
	context->OMSetRenderTargets(BUFFER_COUNT, renderTargetViewArray, depthStencilView);

	//Clear the render target buffers
	for (i = 0; i < BUFFER_COUNT; i++)
	{
		context->ClearRenderTargetView(renderTargetViewArray[i], color);
	}

	//Clear the depth buffer
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	
	
	for (int i = 0; i < entities.size(); i++)
	{
		ID3D11Buffer* vb = entities[i]->GetMesh()->GetVertexBuffer();
		ID3D11Buffer* ib = entities[i]->GetMesh()->GetIndexBuffer();

		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

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

		context->DrawIndexed(entities[i]->GetMesh()->GetIndexCount(), 0, 0);
	}
	//ID3D11RenderTargetView* targets[8] = {};
	//targets[0] = backBufferRTV;
	

	context->OMSetRenderTargets(1,&backBufferRTV, depthStencilView);
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f,0);

	context->RSSetState(rasterizer);
	float blend[4] = { 1,1,1, 1};
	context->OMSetBlendState(blendState, blend, 0xFFFFFFFF);

	for (int i = 0; i < MAX_LIGHTS; i++)
	{
		ID3D11Buffer* vb1 = lightEntity->GetMesh()->GetVertexBuffer();
		ID3D11Buffer* ib1 = lightEntity->GetMesh()->GetIndexBuffer();

		context->IASetVertexBuffers(0, 1, &vb1, &stride, &offset);
		context->IASetIndexBuffer(ib1, DXGI_FORMAT_R32_UINT, 0);

		lightVertexShader->SetMatrix4x4("world", lightEntity->GetWorldMatrix());
		lightVertexShader->SetMatrix4x4("view", camera->GetView());
		lightVertexShader->SetMatrix4x4("projection", camera->GetProjection());

		lightVertexShader->CopyAllBufferData();
		lightVertexShader->SetShader();



		lightPixelShader->SetSamplerState("Sampler", sampler);
		lightPixelShader->SetShaderResourceView("NormalMap", shaderResourceViewArray[0]);
		lightPixelShader->SetShaderResourceView("Texture", shaderResourceViewArray[1]);
		lightPixelShader->SetShaderResourceView("PositionTexture", shaderResourceViewArray[2]);

		lightPixelShader->SetFloat3("CameraPosition", camera->GetPosition());
		pointLight1.SetpointLightValues(XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0), XMFLOAT3(-3, 0, 0));
		pointLight2.SetpointLightValues(XMFLOAT4(0.5f, 1.0f, 0.5f, 1.0f), XMFLOAT3(3, 0, 0));
		/*lightPixelShader->SetFloat3("pointLightPosition", XMFLOAT3(-3, 0, 0));
		lightPixelShader->SetFloat4("pointLightColor", XMFLOAT4(1.0f, 0.3f, 0.3f, 1.0));*/
		/*lightPixelShader->SetData("pointLight1", &pointLight1, sizeof(PointLight));
		lightPixelShader->SetData("pointLight2", &pointLight2, sizeof(PointLight));*/

		lightPixelShader->CopyAllBufferData();
		lightPixelShader->SetShader();

		context->DrawIndexed(lightEntity->GetMesh()->GetIndexCount(), 0, 0);

		context->OMSetBlendState(NULL, blend, 0xFFFFFFFF);
	}
	

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