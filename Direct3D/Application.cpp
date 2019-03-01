#include "Application.h"
#include <exception>
#include "helpers.h"
#include "InputHandler.h"
#include <sstream>
#include "Graphics.h"
#include <chrono>
#include "ColorCube.h"
#include "TextureCube.h"
#include "Skybox.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "LightTextureShader.h"
#include "PresetOne.h"
#include "TextureCube.h"
#include "Pyramid.h"


const char* Application::skyboxTextureNames[]{
	"skybox_1.png",
	"skybox_2.jpg",
	"skybox_3.png",
	"skybox_4.jpg",
};

std::default_random_engine Application::rand(std::chrono::system_clock::now().time_since_epoch().count());

Application::Application()
{
}


Application::~Application()
{
}


void Application::start()
{
	InputHandler::instance()->listen(this);

	try {
		_system = new System(1280, 720);
		_system->addUpdateTarget(this);
		_system->run();
	} catch (std::exception* e) {
		OutputDebugString(toDebugString(e->what()));
	}

	InputHandler::instance()->unlisten(this);
}

void Application::update(FLOAT deltaTime)
{
	if (_currentPreset != nullptr) {
		_currentPreset->update(deltaTime);
	}
}

void Application::notifyInput(char character)
{
	std::wstringstream ss;

	ss << character;

	// OutputDebugString(ss.str().c_str());

	Graphics* gfx = _system->getGraphics();
	Camera* camera = gfx->getCamera();
	FLOAT deltaTime = gfx->getDeltaTime();

	switch (character) {
		case 0x30:
			createSkybox();
			break;
		case 0x31:
			createColorCube();
			break;
		case 0x32:
			createMesh<TextureCube>();
			break;
		case 0x33:
			createMesh<TextureCube>(true);
			break;
		case 0x34:
			createMesh<Pyramid>();
			break;
		case 0x35:
			createMesh<Pyramid>(true);
			break;
		case 0x36:
			runPreset<PresetOne>();
			break;
		case 0x4E:
			gfx->scaleMeshes(1.0f - 1.0f * deltaTime);
			break;
		case 0x4D:
			gfx->scaleMeshes(1.0f + 1.0f * deltaTime);
			break;
		case 0x25:
			XMFLOAT3 camPosition = camera->getPosition();
			XMFLOAT3 newPosition = XMFLOAT3(camPosition.x - 5.0f * deltaTime, camPosition.y, camPosition.z);
			camera->setPosition(newPosition);
			gfx->setSkyboxPosition(newPosition);
			break;
		case 0x27:
			camPosition = camera->getPosition();
			newPosition = XMFLOAT3(camPosition.x + 5.0f * deltaTime, camPosition.y, camPosition.z);
			camera->setPosition(newPosition);
			gfx->setSkyboxPosition(newPosition);
			break;
		case 0x26:
			camPosition = camera->getPosition();
			newPosition = XMFLOAT3(camPosition.x, camPosition.y, camPosition.z + 5.0f * deltaTime);
			camera->setPosition(newPosition);
			gfx->setSkyboxPosition(newPosition);
			break;
		case 0x28:
			camPosition = camera->getPosition();
			newPosition = XMFLOAT3(camPosition.x, camPosition.y, camPosition.z - 5.0f * deltaTime);
			camera->setPosition(newPosition);
			gfx->setSkyboxPosition(newPosition);
			break;
		case 0x43:
			clear();
			break;
		case 0x41:
			gfx->rotateMeshesYCounterClockwise(5.0f * deltaTime);
			break;
		case 0x44:
			gfx->rotateMeshesYClockwise(5.0f * deltaTime);
			break;
		case 0x57:
			gfx->rotateMeshesXCounterClockwise(5.0f * deltaTime);
			break;
		case 0x53:
			gfx->rotateMeshesXClockwise(5.0f * deltaTime);
			break;
		case 0x4C:
			createLight();
			break;
		case 0x20:
			createBaseLights();
			break;
	}
}

void Application::clear()
{
	getGraphics()->clear();
}

void Application::createSkybox(int index)
{
	MeshData meshData{};
	meshData.scale = XMFLOAT3(5.0f, 5.0f, 5.0f);

	static std::uniform_int_distribution<int> randInt(0, 3);

	const char* textureName = nullptr;

	Skybox* currentSkybox = _system->getGraphics()->getSkybox();

	if (index >= 0) {
		textureName = Application::skyboxTextureNames[index];
	}

	while (index < 0) {
		index = randInt(rand);
		textureName = Application::skyboxTextureNames[index];

		if (currentSkybox != nullptr && currentSkybox->getTextureShader()->getTexture()->getFilename() == textureName) {
			index = -1;
		}
	}

	ID3D11Device* device = getDevice();

	Texture* texture = new Texture(textureName);
	texture->init(device);

	TextureShader* shader = new TextureShader(texture);
	shader->init(device);

	Skybox* mesh = new Skybox(shader, meshData);
	mesh->init(device);

	getGraphics()->setSkybox(mesh);
}

void Application::createColorCube()
{
	ID3D11Device* device = getDevice();

	MeshData meshData{};
	meshData.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	setMeshPosition(meshData);

	ColorShader* shader = new ColorShader();
	shader->init(device);

	ColorCube* mesh = new ColorCube(shader, meshData);
	mesh->init(device);

	getGraphics()->addMesh(mesh);
}

template<typename Mesh>
void Application::createMesh(bool transparent)
{
	ID3D11Device* device = getDevice();

	MeshData meshData{};
	meshData.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	setMeshPosition(meshData);

	Texture* texture = new Texture("stone.png");
	texture->init(device);

	LightTextureShader* shader = new LightTextureShader(texture);

	shader->init(device);

	Mesh* mesh = new Mesh(shader, meshData);

	std::uniform_real_distribution<FLOAT> de(0.0f, 0.05f);

	XMFLOAT4 emission(de(rand), de(rand), de(rand), 0.05f);
	mesh->setEmission(emission);

	if (transparent) {
		mesh->makeTransparent();
	}

	mesh->init(device);

	getGraphics()->addMesh(mesh);
}

void Application::setMeshPosition(MeshData& meshData)
{
	Camera* camera = getGraphics()->getCamera();

	if (camera != nullptr) {
		meshData.position = camera->getPosition();
		meshData.position.z += 2.0f;
	}
}

void Application::createLight()
{
	// rand.seed(std::chrono::system_clock::now().time_since_epoch().count());

	static std::uniform_int_distribution<int> d(2, 3);

	LightType type = static_cast<LightType>(d(rand));

	std::wstringstream ss;

	ss << (int)type;

	OutputDebugString(ss.str().c_str());

	switch (type)
	{
	case LightType::Spot:
		createSpotLight();
		break;
	case LightType::Point:
		createPointLight();
		break;
	default:
		break;
	}

	/*std::wstringstream ss;

	ss << getGraphics()->getLights().size();

	OutputDebugString(ss.str().c_str());*/
}

void Application::createPointLight()
{
	std::uniform_real_distribution<FLOAT> di(0.5f, 1.3f);
	std::uniform_real_distribution<FLOAT> dr(0.5f, 4.5f);

	getGraphics()->addLight(Light::point(di(rand), dr(rand), randomLightPosition(), randomColor()));
}

XMFLOAT3 Application::randomLightPosition()
{
	XMFLOAT3 minPos(0.0f, 0.0f, 0.0f);
	XMFLOAT3 maxPos(0.0f, 0.0f, 0.0f);

	for (const auto& mesh : getGraphics()->getMeshes()) {
		if (mesh->getPosition().x < minPos.x) {
			minPos.x = mesh->getPosition().x;
		}

		if (mesh->getPosition().y < minPos.y) {
			minPos.y = mesh->getPosition().y;
		}

		if (mesh->getPosition().z < minPos.z) {
			minPos.z = mesh->getPosition().z;
		}

		if (mesh->getPosition().x > maxPos.x) {
			maxPos.x = mesh->getPosition().x;
		}

		if (mesh->getPosition().y > maxPos.y) {
			maxPos.y = mesh->getPosition().y;
		}

		if (mesh->getPosition().z > maxPos.z) {
			maxPos.z = mesh->getPosition().z;
		}
	}

	std::uniform_real_distribution<FLOAT> dx(minPos.x - 1.5f, maxPos.x + 1.5f);
	std::uniform_real_distribution<FLOAT> dy(minPos.y - 1.5f, maxPos.y + 1.5f);
	std::uniform_real_distribution<FLOAT> dz(minPos.z - 1.5f, maxPos.z + 1.5f);

	return XMFLOAT3(dx(rand), dy(rand), dz(rand));
}

XMFLOAT4 Application::randomColor(bool noTransparency)
{
	std::uniform_real_distribution<FLOAT> d(0.0f, 1.0f);

	return XMFLOAT4(d(rand), d(rand), d(rand), noTransparency ? 1.0f : d(rand));
}

void Application::createSpotLight()
{
	std::vector<AMesh*> meshes = getGraphics()->getMeshes();

	std::uniform_real_distribution<FLOAT> di(0.25f, 1.25f);
	std::uniform_real_distribution<FLOAT> dr(1.5f, 10.0f);
	std::uniform_real_distribution<FLOAT> da(10.0f, 70.0f);
	std::uniform_int_distribution<int> dm(0, meshes.size() - 1);

	XMFLOAT3 position = randomLightPosition();
	XMFLOAT3 focus = meshes[dm(rand)]->getPosition();
	XMFLOAT3 direction = XMFLOAT3(focus.x - position.x, focus.y - position.y, focus.z - position.z);

	getGraphics()->addLight(Light::spot(di(rand), dr(rand), da(rand), position, direction, randomColor()));
}

void Application::createDirectionalLight()
{
	std::uniform_real_distribution<FLOAT> di(0.1f, 0.5f);
	std::uniform_real_distribution<FLOAT> dd(-1.0f, 1.0f);

	XMFLOAT3 direction(dd(rand), dd(rand), dd(rand));

	getGraphics()->addLight(Light::directional(di(rand), direction, randomColor()));
}

void Application::createAmbientLight()
{
	std::uniform_real_distribution<FLOAT> dc(0.05f, 0.15f);

	XMFLOAT4 color(dc(rand), dc(rand), dc(rand), 1.0f);

	getGraphics()->addLight(Light::ambient(color));
}

void Application::createBaseLights()
{
	getGraphics()->clearLights();
	createDirectionalLight();
	createAmbientLight();
}


void Application::createGround()
{
	ID3D11Device* device = getDevice();

	MeshData meshData {};
	meshData.scale = XMFLOAT3(10.0f, 0.1f, 10.0f);
	meshData.position.y = -3.0f;

	ColorShader* shader = new ColorShader();
	shader->init(device);

	ColorCube* mesh = new ColorCube(shader, meshData);
	mesh->useColor(XMFLOAT4(0.95f, 0.95f, 0.95f, 1.0f));
	mesh->init(device);

	getGraphics()->addMesh(mesh);
}