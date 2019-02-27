#include "TextureCube.h"
#include "Vertex.h"
#include <exception>

void TextureCube::createVertexBuffer(ID3D11Device* device)
{
	_vertexCount = 24;
	_stride = sizeof(VertexPNT3);

	VertexPNT3 vertices[] = {
		// Front
		VertexPNT3(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
		VertexPNT3(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f),
		VertexPNT3(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f),
		VertexPNT3(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),

		// Left
		VertexPNT3(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		VertexPNT3(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		VertexPNT3(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		VertexPNT3(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Right
		VertexPNT3(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f),
		VertexPNT3(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
		VertexPNT3(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
		VertexPNT3(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),

		// Back
		VertexPNT3(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
		VertexPNT3(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),
		VertexPNT3(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f),
		VertexPNT3(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f),

		// Top
		VertexPNT3(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
		VertexPNT3(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		VertexPNT3(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f),
		VertexPNT3(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),

		// Bottom
		VertexPNT3(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		VertexPNT3(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		VertexPNT3(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
		VertexPNT3(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
	};

	for (size_t i = 0; i < 6; ++i) {
		int base = i * 4;
		calculateTangentAndBinormal(vertices[base], vertices[base + 1], vertices[base + 2]);
		calculateTangentAndBinormal(vertices[base], vertices[base + 2], vertices[base + 3]);
	}

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _stride;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_vertexBuffer)))) {
		throw new std::exception("Could not create vertex buffer. ColorCube.cpp:createVertexBuffer()");
	}
}
