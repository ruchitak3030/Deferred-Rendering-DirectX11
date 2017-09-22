#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Vertex.h"
#include "Lights.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

public:
	void SetLights();
	void SetRenderer(GameEntity* &gameEntity, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, SimpleVertexShader* &vertexShader, SimplePixelShader* &pixelShader, ID3D11DeviceContext* context);


private:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;
	PointLight pointLight4;

	SpotLight spotLight1;
	SpotLight spotLight2;
};

