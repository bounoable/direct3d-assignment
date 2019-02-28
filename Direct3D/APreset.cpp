#include "APreset.h"

std::default_random_engine APreset::rand(std::chrono::system_clock::now().time_since_epoch().count());

APreset::APreset(Application* app)
{
	_app = app;
}


APreset::~APreset()
{
}

void APreset::apply()
{
	_app->clear();
	_app->createBaseLights();
	_app->getGraphics()->getCamera()->setPosition(getCameraPosition());

	createLights();
	createMeshes();

	_app->createSkybox(getSkyboxIndex());
}

Texture* APreset::createTexture(const char* filename)
{
	Texture* texture = new Texture(filename);
	texture->init(_app->getDevice());

	return texture;
}
