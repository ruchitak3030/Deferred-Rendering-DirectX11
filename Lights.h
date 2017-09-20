#pragma once
#include "DXCore.h"
#include <DirectXMath.h>


using namespace DirectX;

struct PointLight
{
	XMFLOAT4 Color;
	XMFLOAT3 Position;

	void SetpointLightValues(XMFLOAT4 _color, XMFLOAT3 _position)
	{
		Color = _color;
		Position = _position;
	}
};
