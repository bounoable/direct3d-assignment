#include "Camera.h"
#include "InputHandler.h"


Camera::Camera(
	XMFLOAT3 position,
	XMFLOAT3 viewTarget,
	XMFLOAT3 up,
	float angle,
	float clientWidth,
	float clientHeight,
	float nearest,
	float furthest
) {
	_position = position;
	_viewTarget = viewTarget;
	_up = up;
	_angle = angle;
	_clientWidth = clientWidth;
	_clientHeight = clientHeight;
	_nearest = nearest;
	_furthest = furthest;

	XMStoreFloat4x4(&_viewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&_projMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&_orthoMatrix, XMMatrixIdentity());

	this->initMatrix();
}


Camera::~Camera()
{
}

void Camera::initMatrix()
{
	this->initViewMatrix();
	this->initProjectionMatrix();
	//this->initOrthographicMatrix();
}

void Camera::initViewMatrix()
{
	XMStoreFloat4x4(&_viewMatrix, XMMatrixLookAtLH(XMLoadFloat3(&_position), XMLoadFloat3(&_viewTarget), XMLoadFloat3(&_up)));
}

void Camera::initProjectionMatrix()
{
	XMStoreFloat4x4(&_projMatrix, XMMatrixPerspectiveFovLH(XM_PI * _angle, _clientWidth / _clientHeight, 0.3f, 100.0f));
}

void Camera::initOrthographicMatrix()
{
	XMStoreFloat4x4(&_orthoMatrix, XMMatrixOrthographicLH(_clientWidth, _clientHeight, 1.0f, _furthest));
}

XMMATRIX Camera::getWorldViewProjectionMatrix(XMMATRIX* worldMatrix)
{
	XMMATRIX wvp {};

	// wvp = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	wvp = *worldMatrix * XMMatrixLookAtLH(XMVectorSet(_position.x, _position.y, _position.z, 1.0f), XMVectorSet(_viewTarget.x, _viewTarget.y, _viewTarget.z, 0.0f), XMVectorSet(_up.x, _up.y, _up.z, 0.0f));
	wvp *= XMLoadFloat4x4(&_projMatrix);
	wvp = XMMatrixTranspose(wvp);

	return wvp;
}
