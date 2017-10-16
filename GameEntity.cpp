#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	this->mesh = mesh;
	_material = material;

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
}

GameEntity::GameEntity(Mesh* mesh, DirectX::XMFLOAT3 lightColor)
{
	this->mesh = mesh;
	_lightColor = lightColor;

	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
	position = XMFLOAT3(0, 0, 0);
	rotation = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
}

GameEntity::~GameEntity(void)	
{
}

void GameEntity::UpdateWorldMatrix()
{
	XMMATRIX trans = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX rotX = XMMatrixRotationX(rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);
	XMMATRIX sc = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMMATRIX total = sc * rotZ * rotY * rotX * trans;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(total));
}

DirectX::XMFLOAT3 GameEntity::GetLightColor()
{
	return _lightColor;
}

DirectX::XMFLOAT3 GameEntity::GetPosition()
{
	return position;
}
