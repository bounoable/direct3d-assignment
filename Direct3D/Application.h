#pragma once
#include "InputListener.h"
#include "System.h"
#include <random>
#include "Direct3D.h"
#include "Graphics.h"

class Application : InputListener
{
public:
	Application();
	~Application();

	void start();
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

private:
	static const char* skyboxTextureNames[];
	static std::default_random_engine rand;

	System* _system = nullptr;

	void createSkybox();
	void createColorCube();
	void createTextureCube();
	void createLightTextureCube();
	void createLight();
	Light createPointLight();
	Light createSpotLight();
	void createGround();
	void clear();

	void setMeshPosition(MeshData& meshData);
};

