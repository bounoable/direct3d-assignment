#pragma once
#include "APreset.h"
#include <map>

class PresetOne :
	public APreset
{
public:
	using APreset::APreset;

	~PresetOne()
	{}

	void update(FLOAT deltaTime);

protected:
	std::map<AMesh*, XMFLOAT3> _rotationSpeeds;

	void createLights();
	void createMeshes();

	XMFLOAT3 getCameraPosition()
	{
		return XMFLOAT3(0.0f, 0.0f, -4.0f);
	}

	int getSkyboxIndex()
	{
		return 2;
	}
};

