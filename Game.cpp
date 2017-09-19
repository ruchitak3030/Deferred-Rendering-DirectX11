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

	
	delete vertexShader;
	delete pixelShader;

	// Clean up resources
	for(auto& e : entities) delete e;
	for(auto& m : meshes) delete m;
	delete camera;
	
}


void Game::Init()
{
	
	LoadShaders();
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

	
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	vertexShader->SetShader();
	vertexShader->SetMatrix4x4("view", camera->GetView());
	vertexShader->SetMatrix4x4("projection", camera->GetProjection());

	

	for (int i = 0; i < entities.size(); i++)
	{
		renderer.SetRenderer(entities[i], camera, vertexBuffer, indexBuffer, vertexShader, pixelShader, context);
	}
	
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