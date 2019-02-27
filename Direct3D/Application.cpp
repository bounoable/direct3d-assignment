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
		_system->run();
	} catch (std::exception* e) {
		OutputDebugString(toDebugString(e->what()));
	}

	InputHandler::instance()->unlisten(this);
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
			createTextureCube();
			break;
		case 0x33:
			createLightTextureCube();
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
	}
}

void Application::clear()
{
	getGraphics()->clear();
}

void Application::createSkybox()
{
	MeshData meshData{};
	meshData.scale = XMFLOAT3(5.0f, 5.0f, 5.0f);

	static std::uniform_int_distribution<int> randInt(0, 3);

	int index = -1;
	const char* textureName = nullptr;

	Skybox* currentSkybox = _system->getGraphics()->getSkybox();

	while (index == -1) {
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

void Application::createTextureCube()
{
	ID3D11Device* device = getDevice();

	MeshData meshData {};
	meshData.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	setMeshPosition(meshData);

	Texture* texture = new Texture("stone.png");
	texture->init(device);

	TextureShader* shader = new TextureShader(texture);
	shader->init(device);

	TextureCube* mesh = new TextureCube(shader, meshData);
	mesh->init(device);

	getGraphics()->addMesh(mesh);
}


void Application::createLightTextureCube()
{
	ID3D11Device* device = getDevice();

	MeshData meshData{};
	meshData.scale = XMFLOAT3(0.5f, 0.5f, 0.5f);

	setMeshPosition(meshData);

	Texture* texture = new Texture("stone.png");
	texture->init(device);

	LightTextureShader* shader = new LightTextureShader(texture);

	shader->init(device);

	TextureCube* mesh = new TextureCube(shader, meshData);
	mesh->makeTransparent();
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
	static std::default_random_engine e;
	static std::uniform_int_distribution<int> d(0, 1);

	Light light {};

	switch (d(e))
	{
	case 0:
		light = createPointLight();
		break;
	case 1:
		light = createPointLight();
		break;
	default:
		break;
	}

	getGraphics()->addLight(light);

	std::wstringstream ss;

	ss << getGraphics()->getLights().size();

	OutputDebugString(ss.str().c_str());
}

Light Application::createPointLight()
{
	static std::default_random_engine e;
	e.seed(std::chrono::system_clock::now().time_since_epoch().count());

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

	std::uniform_real_distribution<FLOAT> dx(minPos.x - 1.0f, maxPos.x + 1.0f);
	std::uniform_real_distribution<FLOAT> dy(minPos.y - 1.0f, maxPos.y + 1.0f);
	std::uniform_real_distribution<FLOAT> dz(minPos.z - 1.0f, maxPos.z + 1.0f);

	std::uniform_real_distribution<FLOAT> di(0.2f, 1.2f);
	std::uniform_real_distribution<FLOAT> dr(1.0f, 5.0f);

	std::uniform_real_distribution<FLOAT> dc(0.0f, 1.0f);

	Light light = Light::point(di(e), dr(e), XMFLOAT3(dx(e), dy(e), dz(e)), XMFLOAT4(dc(e), dc(e), dc(e), dc(e)));

	return light;
}

Light Application::createSpotLight()
{
	return Light();
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