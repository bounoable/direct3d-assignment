#include "Graphics.h"
#include "ColorCube.h"
#include "TextureCube.h"
#include "Skybox.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "LightTextureShader.h"
#include "Camera.h"

using namespace std;
using namespace DirectX;

Graphics::Graphics(HWND hWnd, UINT screenWidth, UINT screenHeight)
{
	_d3d = new Direct3D(hWnd, screenWidth, screenHeight);

	createCamera(screenWidth, screenHeight);
}

Graphics::~Graphics()
{
	if (_d3d != nullptr) {
		delete _d3d;
		_d3d = nullptr;
	}

	for (const auto& mesh : _meshes) {
		if (mesh != nullptr) {
			delete mesh;
		}
	}

	_meshes.clear();
}

void Graphics::update(FLOAT dt)
{
	_deltaTime = dt;

	rotateSkybox();
}

void Graphics::render()
{
	_d3d->beginScene(0.1f, 0.0f, 0.1f);

	if (_skybox != nullptr) {
		renderMesh(_skybox);
	}

	for (const auto& mesh : _meshes) {
		renderMesh(mesh);
	}

	_d3d->endScene();
}

void Graphics::renderMesh(AMesh* mesh)
{
	XMMATRIX worldMatrix = mesh->render(_d3d->getDeviceContext());

	XMFLOAT4X4 worldStore{};
	XMStoreFloat4x4(&worldStore, DirectX::XMMatrixTranspose(worldMatrix));

	XMFLOAT4X4 wvpStore{};
	XMStoreFloat4x4(&wvpStore, _camera->getWorldViewProjectionMatrix(&worldMatrix));

	mesh->getShader()->setWorldViewProjectionMatrix(worldStore, wvpStore, _camera->getPosition());
	mesh->getShader()->render(_d3d->getDeviceContext(), mesh->getIndexCount());
}

void Graphics::scaleMeshes(float scale)
{
	for (const auto& mesh : _meshes) {
		mesh->setScale(XMFLOAT3(mesh->getScale().x * scale, mesh->getScale().y * scale, mesh->getScale().z * scale));
	}
}

void Graphics::createCamera(UINT screenWidth, UINT screenHeight)
{
	if (_camera != nullptr) {
		delete _camera;
	}

	_camera = new Camera(
		XMFLOAT3(0.0f, 0.0f, -2.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		XMFLOAT3(0.0f, 1.0f, 0.0f),
		0.3333f,
		screenWidth,
		screenHeight
	);
}

void Graphics::rotateSkybox()
{
	if (_skybox != nullptr) {
		_skybox->rotateYClockwise(0.05f * _deltaTime);
	}
}

void Graphics::clear()
{
	setSkybox(nullptr);

	for (const auto& mesh : _meshes) {
		if (mesh != nullptr) {
			delete mesh;
		}
	}

	_meshes.clear();
	_lights.clear();
}