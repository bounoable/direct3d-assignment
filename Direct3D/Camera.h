#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include "AMesh.h"

using namespace DirectX;

class Camera
{
public:
	Camera(
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, -2.0f),
		XMFLOAT3 viewTarget = XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f),
		float angle = 0.33333f,
		float clientWidth = 0.0f,
		float clientHeight = 0.0f,
		float nearest = 0.1f,
		float furthest = 100.0f
	);

	~Camera();

	XMMATRIX getWorldViewProjectionMatrix(XMMATRIX* worldMatrix);

	XMFLOAT3 getPosition()
	{
		return _position;
	}

	void setPosition(XMFLOAT3 position)
	{
		_viewTarget = XMFLOAT3(_viewTarget.x - _position.x + position.x, _viewTarget.y - _position.y + position.y, _viewTarget.z - _position.z + position.z);
		_position = position;
		initMatrix();
	}

protected:
	XMFLOAT3 _position;
	XMFLOAT3 _viewTarget;
	XMFLOAT3 _up;

	XMFLOAT4X4 _viewMatrix;
	XMFLOAT4X4 _projMatrix;
	XMFLOAT4X4 _orthoMatrix;

	float _angle;
	float _clientWidth;
	float _clientHeight;
	float _nearest;
	float _furthest;

	void initMatrix();
	void initViewMatrix();
	void initProjectionMatrix();
	void initOrthographicMatrix();
};


