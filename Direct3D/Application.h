#pragma once
#include "InputListener.h"
#include "System.h"
#include <random>
#include "Direct3D.h"
#include "Graphics.h"
#include "UpdateTarget.h"

class APreset;

class Application : InputListener, UpdateTarget
{
public:
	Application();
	~Application();

	void start();
	void update(FLOAT deltaTime);
	void notifyInput(char character);

	ID3D11Device* getDevice()
	{
		return getDirect3D()->getDevice();
	}

	Direct3D* getDirect3D()
	{
		return getGraphics()->getDirect3D();
	}

	Graphics* getGraphics()
	{
		return _system->getGraphics();
	}

	void createSkybox(int index = -1);
	void createColorCube();

	template<typename Mesh>
	void createMesh(bool transparent = false);

	void createLight();
	void createPointLight();
	void createSpotLight();
	void createDirectionalLight();
	void createAmbientLight();
	void createBaseLights();
	void createGround();
	void clear();

	XMFLOAT3 randomLightPosition();
	XMFLOAT4 randomColor(bool noTransparency = false);

	template<typename Preset>
	void runPreset()
	{
		if (_currentPreset != nullptr) {
			delete _currentPreset;
			_currentPreset = nullptr;
		}

		Preset* preset = new Preset(this);

		preset->apply();
		_currentPreset = preset;
	}

private:
	static const char* skyboxTextureNames[];
	static std::default_random_engine rand;

	System* _system = nullptr;
	APreset* _currentPreset = nullptr;

	void setMeshPosition(MeshData& meshData);
};

