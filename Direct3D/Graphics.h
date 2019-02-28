#pragma once
#include <Windows.h>
#include "Direct3D.h"
#include "AShader.h"
#include "Camera.h"
#include <string>
#include "Skybox.h"
#include "UpdateTarget.h"


struct LightDistanceComparer
{
	XMFLOAT3 position {};

	LightDistanceComparer(XMFLOAT3 pos)
	{
		position = pos;
	}

	bool operator()(Light& lhs, Light& rhs)
	{
		if (
			(lhs.type == LightType::Ambient || lhs.type == LightType::Directional) &&
			(rhs.type == LightType::Ambient || rhs.type == LightType::Directional)
		) {
			if (lhs.type == LightType::Ambient) {
				return true;
			}

			if (rhs.type == LightType::Ambient) {
				return false;
			}

			if (lhs.type == LightType::Directional) {
				return true;
			}

			return false;
		}

		XMFLOAT3 distanceVecA = XMFLOAT3(
			position.x - lhs.position.x,
			position.y - lhs.position.y,
			position.z - lhs.position.z
		);

		XMFLOAT3 distanceVecB = XMFLOAT3(
			position.x - rhs.position.x,
			position.y - rhs.position.y,
			position.z - rhs.position.z
		);

		float distanceA = sqrtf(
			distanceVecA.x * distanceVecA.x +
			distanceVecA.y + distanceVecA.y +
			distanceVecA.z * distanceVecA.z
		);

		float distanceB = sqrtf(
			distanceVecB.x * distanceVecB.x +
			distanceVecB.y + distanceVecB.y +
			distanceVecB.z * distanceVecB.z
		);

		return distanceA <= distanceB;
	}
};

class Graphics: public UpdateTarget
{
public:
	Graphics(HWND hWnd, UINT screenWidth, UINT screenHeight);
	~Graphics();

	void update(FLOAT dt);
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

		if (_camera != nullptr) {
			setSkyboxPosition(_camera->getPosition());
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
		_meshes.push_back(mesh);

		LightTextureShader* shader = dynamic_cast<LightTextureShader*>(mesh->getShader());

		if (shader != nullptr) {
			applyLights(mesh);
		}
	}

	std::vector<AMesh*> getMeshes()
	{
		return _meshes;
	}

	void addLight(Light light)
	{
		cleanupLights();

		_lights.push_back(light);

		for (const auto& mesh : getMeshes()) {
			LightTextureShader* shader = dynamic_cast<LightTextureShader*>(mesh->getShader());

			if (shader != nullptr) {
				shader->clearLights();
				applyLights(mesh);
			}
		}
	}

	std::vector<Light> getLights()
	{
		return _lights;
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

	void clearLights()
	{
		_lights.clear();

		for (const auto& mesh : _meshes) {
			LightTextureShader* shader = dynamic_cast<LightTextureShader*>(mesh->getShader());

			if (shader != nullptr) {
				shader->clearLights();
			}
		}
	}

protected:
	void rotateSkybox();
	void renderMesh(AMesh* mesh);

	void applyLights(AMesh* mesh);
	void cleanupLights();

private:
	std::vector<AMesh*> _meshes;
	Skybox* _skybox;

	std::vector<Light> _lights;

	Direct3D* _d3d = nullptr;
	Camera* _camera = nullptr;

	FLOAT _deltaTime = 0.0f;
};

