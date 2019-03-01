#include "PresetOne.h"
#include "TextureCube.h"
#include "AMesh.h"
#include "Pyramid.h"


void PresetOne::createLights()
{
	Graphics* gfx = _app->getGraphics();

	gfx->addLight(Light::spot(5.0f, 10.0f, 100.0f, XMFLOAT3(2.5f, 1.5f, -3.25f), XMFLOAT3(-2.5f, -1.5f, 3.25f), _app->randomColor()));
	gfx->addLight(Light::spot(8.0f, 10.0f, 80.0f, XMFLOAT3(-1.5f, -0.5f, -2.25f), XMFLOAT3(1.5f, 0.5f, 2.25f), _app->randomColor()));
	
	gfx->addLight(Light::spot(15.0f, 20.0f, 400.0f, XMFLOAT3(4.5f, -2.5f, -6.25f), XMFLOAT3(-5.5f, 2.5f, 6.25f), _app->randomColor()));
	gfx->addLight(Light::spot(15.0f, 20.0f, 450.0f, XMFLOAT3(-5.5f, 0.25f, -6.25f), XMFLOAT3(6.0f, -0.25f, 6.25f), _app->randomColor()));

	gfx->addLight(Light::point(0.2f, 4.0f, XMFLOAT3(-0.3f, 3.25f, -1.5f), _app->randomColor()));
}

void PresetOne::createMeshes()
{
	MeshData data {};
	data.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	data.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	data.rotation = XMFLOAT3(0.15f, 0.75f, 0.25f);

	addTextureMesh<Pyramid>(data, "stone.png");

	data.position = XMFLOAT3(1.5f, -0.25f, -0.15f);
	data.scale = XMFLOAT3(0.45f, 0.575f, 0.5f);
	data.rotation = XMFLOAT3(0.5f, 0.25f, 0.15f);

	addTextureMesh<TextureCube>(data, "stone.png");

	data.position = XMFLOAT3(-1.5f, +0.125f, -0.05f);
	data.scale = XMFLOAT3(0.5f, 0.45f, 0.47f);
	data.rotation = XMFLOAT3(0.8f, 0.75f, 0.35f);

	addTextureMesh<TextureCube>(data, "stone.png");

	std::uniform_real_distribution<FLOAT> d(0.1f, 1.0f);

	for (const auto mesh : _meshes) {
		_rotationSpeeds[mesh] = XMFLOAT3(d(rand), d(rand), d(rand));
	}
}

void PresetOne::update(FLOAT deltaTime)
{
	std::uniform_real_distribution<FLOAT> d(0.1f, 1.0f);

	for (const auto mesh : _meshes) {
		XMFLOAT3 speed = _rotationSpeeds[mesh];

		mesh->rotateXClockwise(speed.x * deltaTime);
		mesh->rotateYClockwise(speed.y * deltaTime);
		mesh->rotateZClockwise(speed.z * deltaTime);
	}
}