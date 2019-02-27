#include "Skybox.h"
#include "Vertex.h"
#include <exception>
#include <random>

void Skybox::createVertexBuffer(ID3D11Device* device)
{
	_vertexCount = 24;
	_stride = sizeof(VertexPNT3);

	VertexPNT3 vertices[] = {
		// Back
		VertexPNT3(3.0f, -3.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.75f, 1.0f / 1.5f),
		VertexPNT3(3.0f, 3.0f, -3.0f, 0.0f, 0.0f, 1.0f, 0.75f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, 3.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, -3.0f, -3.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f / 1.5f),

		// Left
		VertexPNT3(-3.0f, -3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f / 1.5f),
		VertexPNT3(-3.0f, 3.0f, -3.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, 3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.25f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, -3.0f, 3.0f, 1.0f, 0.0f, 0.0f, 0.25f, 1.0f / 1.5f),

		// Right
		VertexPNT3(3.0f, -3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f / 1.5f),
		VertexPNT3(3.0f, 3.0f, 3.0f, -1.0f, 0.0f, 0.0f, 0.5f, 1.0f / 3.0f),
		VertexPNT3(3.0f, 3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.75f, 1.0f / 3.0f),
		VertexPNT3(3.0f, -3.0f, -3.0f, -1.0f, 0.0f, 0.0f, 0.75f, 1.0f / 1.5f),

		// Front
		VertexPNT3(-3.0f, -3.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.25f, 1.0f / 1.5f),
		VertexPNT3(-3.0f, 3.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.25f, 1.0f / 3.0f),
		VertexPNT3(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f / 3.0f),
		VertexPNT3(3.0f, -3.0f, 3.0f, 0.0f, 0.0f, -1.0f, 0.5f, 1.0f / 1.5f),

		// Top
		VertexPNT3(3.0f, 3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.0f),
		VertexPNT3(3.0f, 3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.5f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, 3.0f, 3.0f, 0.0f, -1.0f, 0.0f, 0.25f, 1.0f / 3.0f),
		VertexPNT3(-3.0f, 3.0f, -3.0f, 0.0f, -1.0f, 0.0f, 0.25f, 0.0f),

		// Bottom
		VertexPNT3(3.0f, -3.0f, 3.0f, 0.0f, 11.0f, 0.0f, 0.5f, 1.0f / 1.5f),
		VertexPNT3(3.0f, -3.0f, -3.0f, 0.0f, 11.0f, 0.0f, 0.5f, 1.0f),
		VertexPNT3(-3.0f, -3.0f, -3.0f, 0.0f, 11.0f, 0.0f, 0.25f, 1.0f),
		VertexPNT3(-3.0f, -3.0f, 3.0f, 0.0f, 11.0f, 0.0f, 0.25f, 1.0f / 1.5f),
	};

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _stride;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_vertexBuffer)))) {
		throw new std::exception("Could not create vertex buffer. Skybox.cpp:createVertexBuffer()");
	}
}

void Skybox::createIndexBuffer(ID3D11Device* device)
{
	_indexCount = 36;

	USHORT indices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 5, 6,
		4, 6, 7,
		8, 9, 10,
		8, 10, 11,
		12, 13, 14,
		12, 14, 15,
		16, 17, 18,
		16, 18, 19,
		20, 21, 22,
		20, 22, 23,
	};

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = _indexCount * sizeof(USHORT);

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = indices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_indexBuffer)))) {
		throw new std::exception("Could not create index buffer. Skybox.cpp:createIndexBuffer()");
	}
}