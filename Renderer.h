#pragma once
const int BUFFER_COUNT = 3;

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
	//Renderer();
	~Renderer();

public:
	void SetLights();
	void SetRenderer(std::vector<GameEntity*> entities, Camera* &camera, ID3D11Buffer* vb, ID3D11Buffer* ib, ID3D11DeviceContext* context, ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, ID3D11RasterizerState* rasterizer, ID3D11BlendState* blendState, ID3D11RenderTargetView* renderTargetViewArray[BUFFER_COUNT], ID3D11ShaderResourceView* shaderResourceViewArray[BUFFER_COUNT], SimpleVertexShader* &deferredVS, SimplePixelShader* &deferredPS, SimpleVertexShader* &lightVS, SimplePixelShader* &lightPS, GameEntity* lightEntity);
	//void GetDeferredData(ID3D11RenderTargetView* renderTargetViewArray[BUFFER_COUNT], ID3D11RenderTargetView* backBufferRTV, ID3D11DepthStencilView* depthStencilView, ID3D11ShaderResourceView* shaderResourceViewArray[BUFFER_COUNT]);
	//void SetDefferedSetup(int textureWidth, int textureHeight, ID3D11Device* device);
private:
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;
	PointLight pointLight4;

	SpotLight spotLight1;
	SpotLight spotLight2;

	ID3D11Device* device;

	

	

	
};

