#include "Graphics.h"
#include "ColorCube.h"
#include "TextureCube.h"
#include "Skybox.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "LightTextureShader.h"
#include "Camera.h"
#include <algorithm>

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

	_d3d->beginScene(0.0f, 0.0f, 0.0f);

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

void Graphics::applyLights(AMesh* mesh)
{
	LightTextureShader* shader = dynamic_cast<LightTextureShader*>(mesh->getShader());

	if (shader == nullptr) {
		return;
	}

	if (shader->getLightCount() >= 12) {
		return;
	}

	std::vector<Light> lights;

	for (auto& light : _lights) {
		lights.push_back(light);
	}

	auto cmp = LightDistanceComparer(mesh->getPosition());
	std::sort(lights.begin(), lights.end(), cmp);

	for (const auto& light : lights) {
		shader->addLight(light);
	}
}

void Graphics::cleanupLights()
{
	// Clean up when more than 30 lights in the scene,
	// but resize the lights to 20 so it doesn't run with every new light

	if (_lights.size() <= 30) {
		return;
	}

	// Remove the lights that are the furthest away from the objects
	XMFLOAT3 avg(0.0f, 0.0f, 0.0f);

	for (const auto& mesh : _meshes) {
		avg.x += mesh->getPosition().x;
		avg.y += mesh->getPosition().y;
		avg.z += mesh->getPosition().z;
	}

	avg.x /= _meshes.size();
	avg.y /= _meshes.size();
	avg.z /= _meshes.size();

	auto cmp = LightDistanceComparer(avg);

	std::sort(_lights.begin(), _lights.end(), cmp);
	_lights.resize(20);
}