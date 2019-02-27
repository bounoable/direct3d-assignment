#include "TextureCube.h"
#include "Vertex.h"
#include <exception>
#include <random>

void TextureCube::createVertexBuffer(ID3D11Device* device)
{
	_vertexCount = 24;
	_stride = sizeof(VertexPNT3);

	VertexPNT3 vertices[] = {
		// Front
		createVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		createVertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
		createVertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		createVertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

		// Left
		createVertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		createVertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		createVertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		createVertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Right
		createVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		createVertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		createVertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		createVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Back
		createVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		createVertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		createVertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		createVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),

		// Top
		createVertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		createVertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		createVertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		createVertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),

		// Bottom
		createVertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		createVertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		createVertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
		createVertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
	};

	for (size_t i = 0; i < 6; ++i) {
		int base = i * 4;
		calculateTangentAndBinormal(vertices[base], vertices[base + 1], vertices[base + 2]);
		calculateTangentAndBinormal(vertices[base], vertices[base + 2], vertices[base + 3]);
	}

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _stride;

	D3D11_SUBRESOURCE_DATA data {};
	data.pSysMem = vertices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_vertexBuffer)))) {
		throw new std::exception("Could not create vertex buffer. ColorCube.cpp:createVertexBuffer()");
	}
}

VertexPNT3 TextureCube::createVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<FLOAT> d(0.35f, 0.85f);

	return VertexPNT3(x, y, z, xn, yn, zn, u, v, _isTransparent ? d(e) : 1.0f);
}
