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

struct SpotLight
{
	XMFLOAT4 Color;
	XMFLOAT3 Position;
	XMFLOAT3 Direction;
	XMFLOAT2 Angles;
	XMFLOAT4 Range;

	void SetSpotLightValues(XMFLOAT4 _color, XMFLOAT3 _position, XMFLOAT3 _direction, XMFLOAT2 _angles, XMFLOAT4 _range)
	{
		Color = _color;
		Position = _position;
		Direction = _direction;
		Angles = _angles;
		Range = _range;
	}
};
