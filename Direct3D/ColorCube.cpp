#include "ColorCube.h"
#include "Vertex.h"
#include <exception>
#include <random>

void ColorCube::createVertexBuffer(ID3D11Device* device)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<FLOAT> d(0.0f, 1.0f);

	_vertexCount = 24;
	_stride = sizeof(VertexPC);

	VertexPC vertices[] = {
		// Front
		createVertex(-1.0f, -1.0f, -1.0f),
		createVertex(-1.0f, 1.0f, -1.0f),
		createVertex(1.0f, 1.0f, -1.0f),
		createVertex(1.0f, -1.0f, -1.0f),

		// Left
		createVertex(-1.0f, -1.0f, 1.0f),
		createVertex(-1.0f, 1.0f, 1.0f),
		createVertex(-1.0f, 1.0f, -1.0f),
		createVertex(-1.0f, -1.0f, -1.0f),

		// Right
		createVertex(1.0f, -1.0f, -1.0f),
		createVertex(1.0f, 1.0f, -1.0f),
		createVertex(1.0f, 1.0f, 1.0f),
		createVertex(1.0f, -1.0f, 1.0f),

		// Back
		createVertex(1.0f, -1.0f, 1.0f),
		createVertex(1.0f, 1.0f, 1.0f),
		createVertex(-1.0f, 1.0f, 1.0f),
		createVertex(-1.0f, -1.0f, 1.0f),

		// Top
		createVertex(-1.0f, 1.0f, -1.0f),
		createVertex(-1.0f, 1.0f, 1.0f),
		createVertex(1.0f, 1.0f, 1.0f),
		createVertex(1.0f, 1.0f, -1.0f),

		// Bottom
		createVertex(-1.0f, -1.0f, 1.0f),
		createVertex(-1.0f, -1.0f, -1.0f),
		createVertex(1.0f, -1.0f, -1.0f),
		createVertex(1.0f, -1.0f, 1.0f),
	};

	D3D11_BUFFER_DESC desc {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _stride;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_vertexBuffer)))) {
		throw new std::exception("Could not create vertex buffer. ColorCube.cpp:createVertexBuffer()");
	}
}

VertexPC ColorCube::createVertex(FLOAT x, FLOAT y, FLOAT z)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<FLOAT> d(0.0f, 1.0f);

	FLOAT r = _useOverrideColor ? _overrideColor.x : d(e);
	FLOAT g = _useOverrideColor ? _overrideColor.y : d(e);
	FLOAT b = _useOverrideColor ? _overrideColor.z : d(e);
	FLOAT a = _useOverrideColor ? _overrideColor.w : d(e);

	return VertexPC(x, y, z, r, g, b, a);
}
