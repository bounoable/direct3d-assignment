#pragma once
#include <Windows.h>
#include "Direct3D.h"
#include "AMesh.h"
#include "AShader.h"
#include "Camera.h"
#include <map>
#include <random>
#include <string>
#include <unordered_set>
#include "Skybox.h"
#include <chrono>

class Graphics
{
public:
	Graphics(HWND hWnd, UINT screenWidth, UINT screenHeight);
	~Graphics();

	void update(FLOAT dt);
	void render();
	void clear();

	Direct3D* getDirect3D()
	{
		return _d3d;
	}

	FLOAT getDeltaTime()
	{
		return _deltaTime;
	}

	Camera* getCamera()
	{
		return _camera;
	}

	Skybox* getSkybox()
	{
		return _skybox;
	}

	void setSkybox(Skybox* skybox)
	{
		if (_skybox != nullptr) {
			delete _skybox;
		}

		_skybox = skybox;
	}

	void setSkyboxPosition(XMFLOAT3 position)
	{
		if (_skybox == nullptr) {
			return;
		}

		_skybox->setPosition(position);
	}

	void addMesh(AMesh* mesh)
	{
		_meshes.insert(mesh);
	}

	void rotateMeshesYClockwise(float amount)
	{
		for (const auto& mesh : _meshes) {
			mesh->rotateYCounterClockwise(amount);
		}
	}

	void rotateMeshesYCounterClockwise(float amount)
	{
		rotateMeshesYClockwise(-amount);
	}

	void rotateMeshesXClockwise(float amount)
	{
		for (const auto& mesh : _meshes) {
			mesh->rotateXCounterClockwise(amount);
		}
	}

	void rotateMeshesXCounterClockwise(float amount)
	{
		rotateMeshesXClockwise(-amount);
	}

	void createCamera(UINT screenWidth, UINT screenHeight);

	void scaleMeshes(float scale);

protected:
	void rotateSkybox();
	void renderMesh(AMesh* mesh);

private:
	std::unordered_set<AMesh*> _meshes;
	Skybox* _skybox;

	Direct3D* _d3d = nullptr;
	Camera* _camera = nullptr;

	FLOAT _deltaTime = 0.0f;
};
