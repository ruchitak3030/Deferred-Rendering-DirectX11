#pragma once
const int BUFFER_COUNT = 3;
#define MAX_LIGHTS 2
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Renderer.h"
#include "Material.h"
#include "Lights.h"

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Input and mesh swapping
	bool prevTab;
	unsigned int currentEntity;

	// Keep track of "stuff" to clean up
	std::vector<Mesh*> meshes;
	std::vector<GameEntity*> entities;
	std::vector<GameEntity*> lightEntities;
	Camera* camera;	

	GameEntity* lightEntity1;
	GameEntity* lightEntity2;
	GameEntity* lightEntity3;
	GameEntity* lightEntity4;
	GameEntity* lightEntity5;
	GameEntity* lightEntity6;
	GameEntity* lightEntity7;
	GameEntity* lightEntity8;

	Mesh* sphereMesh;
	Mesh* planeMesh;


	Renderer renderer;

	//material class objects
	Material* sphereMaterial;
	Material* planeMaterial;
	

	// Initialization helper methods - feel free to customize, combine, etc.
	
	void SetDefferedSetup(int textureWidth, int textureHeight);
	void CreateMatrices();
	void LoadShaders();
	void LoadMesh();
	void LoadTextures();
	void CreateMaterials();	
	void CreateBasicGeometry();	
	void LightEntityInitialize();

	// Texture related DX stuff
	ID3D11ShaderResourceView* sphereTextureSRV;
	ID3D11ShaderResourceView* sphereNormalMapSRV;
	ID3D11ShaderResourceView* planeTextureSRV;
	ID3D11ShaderResourceView* planeNormalMapSRV;
	ID3D11SamplerState* sampler;

	//RenderTargets for Deferred rendering
	
	ID3D11RenderTargetView* renderTargetViewArray[BUFFER_COUNT];
	ID3D11ShaderResourceView* shaderResourceViewArray[BUFFER_COUNT];
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilState* depthState;

	ID3D11RasterizerState* rasterizer;
	ID3D11BlendState* blendState;

	

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* deferredVertexShader;
	SimplePixelShader* deferredPixelShader;
	SimpleVertexShader* backBufferVertexShader;
	SimplePixelShader* backBufferPixelShader;
	SimpleVertexShader* lightVertexShader;
	SimplePixelShader* lightPixelShader;
	SimpleVertexShader* dirVertexShader;
	SimplePixelShader* dirPixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;



};

