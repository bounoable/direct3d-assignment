#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

enum LightType
{
	None = 0,
	Directional = 1,
	Spot = 2,
	Point = 3,
};

struct Light
{
	LightType type = LightType::Directional;
	FLOAT intensity = 0.0f;
	FLOAT radius = 0.0f;
	FLOAT angle = 0.0f;
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	FLOAT padding {};
	XMFLOAT3 direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	FLOAT padding2 {};
	XMFLOAT4 color = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	static Light directional(
		FLOAT intensity,
		XMFLOAT3 direction,
		XMFLOAT4 color
	) {
		Light light {};
		light.type = LightType::Directional;
		light.intensity = intensity;
		light.direction = direction;
		light.color = color;

		return light;
	}

	static Light point(
		FLOAT intensity,
		FLOAT radius,
		XMFLOAT3 position,
		XMFLOAT4 color
	) {
		Light light {};
		light.type = LightType::Point;
		light.intensity = intensity;
		light.radius = radius;
		light.position = position;
		light.color = color;

		return light;
	}

	static Light spot(
		FLOAT intensity,
		FLOAT radius,
		FLOAT angle,
		XMFLOAT3 position,
		XMFLOAT3 direction,
		XMFLOAT4 color
	) {
		Light light {};
		light.type = LightType::Spot;
		light.intensity = intensity;
		light.radius = radius;
		light.angle = angle;
		light.position = position;
		light.direction = direction;
		light.color = color;

		return light;
	}
};