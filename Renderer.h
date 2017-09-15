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
	void SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context);


private:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
};

