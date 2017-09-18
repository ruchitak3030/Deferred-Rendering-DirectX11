#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>

#include "Camera.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Renderer.h"
#include "Material.h"

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
	Camera* camera;

	GameEntity* sphere;
	GameEntity* sphere1;
	GameEntity* sphere2;
	GameEntity* sphere3;
	GameEntity* sphere4;
	GameEntity* sphere5;

	GameEntity* plane1;
	GameEntity* plane2;
	GameEntity* plane3;
	GameEntity* plane4;


	Renderer renderer;

	//material class objects
	Material* sphereMaterial;
	Material* planeMaterial;
	

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();
	void CreateMaterials();

	// Texture related DX stuff
	ID3D11ShaderResourceView* sphereTextureSRV;
	ID3D11ShaderResourceView* sphereNormalMapSRV;
	ID3D11ShaderResourceView* planeTextureSRV;
	ID3D11ShaderResourceView* planeNormalMapSRV;
	ID3D11SamplerState* sampler;

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//Per-vertex Data
	struct VertexPosNormTex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 Tex;
	};

	//Per-instance data
	_declspec(align(16)) struct PlaneInstanceData
	{
		XMMATRIX WorldMatrix;
		XMMATRIX InverseTransposeWorldMatrix;
	};

	struct PerFrameConstantBufferData
	{
		XMMATRIX ViewProjectionMatrix;
	};

	// This structure is used in the simple vertex shader.
	struct PerObjectConstantBufferData
	{
		XMMATRIX WorldMatrix;
		XMMATRIX InverseTransposeWorldMatrix;
		XMMATRIX WorldViewProjectionMatrix;
	};

	struct _Material
	{
		_Material()
			: Emissive(0.0f, 0.0f, 0.0f, 1.0f)
			, Ambient(0.1f, 0.1f, 0.1f, 1.0f)
			, Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
			, Specular(1.0f, 1.0f, 1.0f, 1.0f)
			, SpecularPower(128.0f)
			, UseTexture(false)
		{}

		DirectX::XMFLOAT4   Emissive;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Ambient;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Diffuse;
		//----------------------------------- (16 byte boundary)
		DirectX::XMFLOAT4   Specular;
		//----------------------------------- (16 byte boundary)
		float               SpecularPower;
		// Add some padding complete the 16 byte boundary.
		int                 UseTexture;
		// Add some padding to complete the 16 byte boundary.
		float                 Padding[2];
		//----------------------------------- (16 byte boundary)
	}; // Total:                                80 bytes (5 * 16)

	struct MaterialProperties
	{
		_Material   Material;
	};

	std::vector<MaterialProperties> materialProperties;
	int NumInstances;
};

