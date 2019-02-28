#pragma once
#include "Application.h"
#include "Texture.h"
#include "LightTextureShader.h"
#include <vector>
#include <random>

class APreset
{
public:
	APreset(Application* app);
	~APreset();

	void apply();
	virtual void update(FLOAT deltaTime) {};

protected:
	static std::default_random_engine rand;

	Application* _app = nullptr;
	std::vector<AMesh*> _meshes;

	virtual void createLights() = 0;
	virtual void createMeshes() = 0;

	virtual XMFLOAT3 getCameraPosition()
	{
		return XMFLOAT3(0.0f, 0.0f, -2.0f);
	}

	virtual int getSkyboxIndex()
	{
		return -1;
	}

	Texture* createTexture(const char* filename);
	
	template<typename Mesh>
	Mesh* addTextureMesh(MeshData data, const char* texture)
	{
		LightTextureShader* shader = new LightTextureShader(createTexture(texture));
		shader->init(_app->getDevice());

		Mesh* mesh = new Mesh(shader, data);
		mesh->init(_app->getDevice());
		_meshes.push_back(mesh);

		_app->getGraphics()->addMesh(mesh);

		return mesh;
	}
};

