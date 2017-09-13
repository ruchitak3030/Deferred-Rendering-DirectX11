#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Vertex.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* &vertexBuffer, ID3D11Buffer* &indexBuffer, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context);


private:
	GameEntity* gameEntityRenderer;
	Camera* cameraRenderer;
	ID3D11Buffer* vertexBufferRenderer;
	ID3D11Buffer* indexBufferRenderer;
	SimpleVertexShader* vertexShaderRenderer;
	SimplePixelShader* pixelShaderRenderer;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
};

