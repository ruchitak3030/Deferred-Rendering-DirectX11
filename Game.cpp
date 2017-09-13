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
	textureSRV->Release();
	normalMapSRV->Release();

	
	delete vertexShader;
	delete pixelShader;

	// Clean up resources
	for(auto& e : entities) delete e;
	for(auto& m : meshes) delete m;
	delete camera;
	delete renderer;
}


void Game::Init()
{
	
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Load texture stuff
	HRESULT result = CreateWICTextureFromFile(
		device,
		context, // If I provide the context, it will auto-generate Mipmaps
		L"Debug/Textures/rock.jpg",
		0, // We don't actually need the texture reference
		&textureSRV);


	result = CreateWICTextureFromFile(
		device,
		context, // If I provide the context, it will auto-generate Mipmaps
		L"Debug/Textures/rockNormals.jpg",
		0, // We don't actually need the texture reference
		&normalMapSRV);

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	device->CreateSamplerState(&samplerDesc, &sampler);


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

	meshes.push_back(sphereMesh);
	meshes.push_back(sphereMesh1);
	meshes.push_back(sphereMesh2);
	meshes.push_back(sphereMesh3);

		
	sphere = new GameEntity(sphereMesh);
	sphere1 = new GameEntity(sphereMesh1);
	sphere2 = new GameEntity(sphereMesh2);
	sphere3 = new GameEntity(sphereMesh3);

	entities.push_back(sphere);
	entities.push_back(sphere1);
	entities.push_back(sphere2);
	entities.push_back(sphere3);
	

	sphere->SetScale(1.0f,1.0f, 1.0f);
	sphere->SetPosition(1.0f, 0.0f, 0.0f);


	sphere1->SetScale(1.0f, 1.0f, 1.0f);
	sphere1->SetPosition(1.0f, 0.0f, 2.0f);

	sphere2->SetScale(1.0f, 1.0f, 1.0f);
	sphere2->SetPosition(-1.0f, 0.0f, 1.0f);

	sphere3->SetScale(1.0f, 1.0f, 1.0f);
	sphere3->SetPosition(-1.0f, 0.0f, 4.0f);

	currentEntity = 0;
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

	/*bool currentTab = (GetAsyncKeyState(VK_TAB) & 0x8000) != 0;
	if (currentTab && !prevTab)
		currentEntity = (currentEntity + 1) % entities.size();
	prevTab = currentTab;*/


	/*entities[currentEntity]->Rotate(0, deltaTime * 0.2f, 0);
	
	entities[currentEntity]->UpdateWorldMatrix();*/

	sphere->UpdateWorldMatrix();
	sphere1->UpdateWorldMatrix();
	sphere2->UpdateWorldMatrix();
	sphere3->UpdateWorldMatrix();

	
}

void Game::Draw(float deltaTime, float totalTime)
{
	const float color[4] = {0,0,0,0};

	
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);



	renderer->SetRenderer(sphere, camera, vertexBuffer, indexBuffer, vertexShader, pixelShader, context);
	//vertexShader->SetShader();
	//pixelShader->SetShader();

	//pixelShader->SetFloat3("DirLightDirection", XMFLOAT3(1, 0, 0));
	//pixelShader->SetFloat4("DirLightColor", XMFLOAT4(0.8f, 0.8f, 0.8f, 1));
	//pixelShader->SetFloat3("PointLightPosition", XMFLOAT3(3, 0, 0));
	//pixelShader->SetFloat4("PointLightColor", XMFLOAT4(1, 0.3f, 0.3f, 1));
	//pixelShader->SetFloat3("CameraPosition", camera->GetPosition());

	pixelShader->SetSamplerState("Sampler", sampler);
	pixelShader->SetShaderResourceView("Texture", textureSRV);
	pixelShader->SetShaderResourceView("NormalMap", normalMapSRV);

	//pixelShader->CopyAllBufferData();
	//pixelShader->SetShader();

	//ID3D11Buffer* vb = sphere->GetMesh()->GetVertexBuffer();
	//ID3D11Buffer* ib = sphere->GetMesh()->GetIndexBuffer();

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	//context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);


	//vertexShader->SetMatrix4x4("world", sphere->GetWorldMatrix());
	//vertexShader->SetMatrix4x4("view", camera->GetView());
	//vertexShader->SetMatrix4x4("projection", camera->GetProjection());

	//vertexShader->CopyAllBufferData();

	//context->DrawIndexed(sphere->GetMesh()->GetIndexCount(), 0, 0);

	//ID3D11Buffer* vb1 = sphere1->GetMesh()->GetVertexBuffer();
	//ID3D11Buffer* ib1 = sphere1->GetMesh()->GetIndexBuffer();

	//context->IASetVertexBuffers(0, 1, &vb1, &stride, &offset);
	//context->IASetIndexBuffer(ib1, DXGI_FORMAT_R32_UINT, 0);


	//vertexShader->SetMatrix4x4("world", sphere1->GetWorldMatrix());
	//vertexShader->SetMatrix4x4("view", camera->GetView());
	//vertexShader->SetMatrix4x4("projection", camera->GetProjection());

	//vertexShader->CopyAllBufferData();

	//context->DrawIndexed(sphere1->GetMesh()->GetIndexCount(), 0, 0);


	////Entity 3
	//ID3D11Buffer* vb2 = sphere2->GetMesh()->GetVertexBuffer();
	//ID3D11Buffer* ib2 = sphere2->GetMesh()->GetIndexBuffer();

	//context->IASetVertexBuffers(0, 1, &vb2, &stride, &offset);
	//context->IASetIndexBuffer(ib2, DXGI_FORMAT_R32_UINT, 0);


	//vertexShader->SetMatrix4x4("world", sphere2->GetWorldMatrix());
	//vertexShader->SetMatrix4x4("view", camera->GetView());
	//vertexShader->SetMatrix4x4("projection", camera->GetProjection());

	//vertexShader->CopyAllBufferData();

	//context->DrawIndexed(sphere2->GetMesh()->GetIndexCount(), 0, 0);


	////Entity4
	//ID3D11Buffer* vb3 = sphere3->GetMesh()->GetVertexBuffer();
	//ID3D11Buffer* ib3 = sphere3->GetMesh()->GetIndexBuffer();

	//context->IASetVertexBuffers(0, 1, &vb3, &stride, &offset);
	//context->IASetIndexBuffer(ib3, DXGI_FORMAT_R32_UINT, 0);


	//vertexShader->SetMatrix4x4("world", sphere3->GetWorldMatrix());
	//vertexShader->SetMatrix4x4("view", camera->GetView());
	//vertexShader->SetMatrix4x4("projection", camera->GetProjection());

	//vertexShader->CopyAllBufferData();

	//context->DrawIndexed(sphere3->GetMesh()->GetIndexCount(), 0, 0);

	

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