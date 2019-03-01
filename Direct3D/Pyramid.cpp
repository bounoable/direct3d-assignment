#include "Pyramid.h"
#include <random>

void Pyramid::createVertexBuffer(ID3D11Device* device)
{
	_vertexCount = 16;
	_stride = sizeof(VertexPNT3);

	VertexPNT3 vertices[] = {
		// Front
		createVertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.25f, -1.0f, 0.0f, 0.0f),
		createVertex(0.0f, 1.0f, 0.0f, 0.0f, 0.25f, -1.0f, 0.0f, 1.0f),
		createVertex(1.0f, -1.0f, -1.0f, 0.0f, 0.25f, -1.0f, 1.0f, 1.0f),

		// Left
		createVertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.25f, 0.0f, 0.0f, 0.0f),
		createVertex(0.0f, 1.0f, 0.0f, -1.0f, 0.25f, 0.0f, 0.0f, 1.0f),
		createVertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.25f, 0.0f, 1.0f, 1.0f),

		// Right
		createVertex(1.0f, -1.0f, -1.0f, 1.0f, 0.25f, 0.0f, 0.0f, 0.0f),
		createVertex(0.0f, 1.0f, 0.0f, 1.0f, 0.25f, 0.0f, 0.0f, 1.0f),
		createVertex(1.0f, -1.0f, 1.0f, 1.0f, 0.25f, 0.0f, 1.0f, 1.0f),

		// Back
		createVertex(1.0f, -1.0f, 1.0f, 0.0f, 0.25f, 1.0f, 0.0f, 0.0f),
		createVertex(0.0f, 1.0f, 0.0f, 0.0f, 0.25f, 1.0f, 0.0f, 1.0f),
		createVertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.25f, 1.0f, 1.0f, 1.0f),

		// Bottom
		createVertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		createVertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f),
		createVertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f),
		createVertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f),
	};

	calculateTangentAndBinormal(vertices[0], vertices[1], vertices[2]);
	calculateTangentAndBinormal(vertices[3], vertices[4], vertices[5]);
	calculateTangentAndBinormal(vertices[6], vertices[7], vertices[8]);
	calculateTangentAndBinormal(vertices[9], vertices[10], vertices[11]);
	calculateTangentAndBinormal(vertices[12], vertices[13], vertices[14]);
	calculateTangentAndBinormal(vertices[12], vertices[14], vertices[15]);

	D3D11_BUFFER_DESC desc {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _stride;

	D3D11_SUBRESOURCE_DATA data {};
	data.pSysMem = vertices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_vertexBuffer)))) {
		throw new std::exception("Could not create vertex buffer. Pyramid.cpp:createVertexBuffer()");
	}
}

VertexPNT3 Pyramid::createVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<FLOAT> d(0.35f, 0.85f);

	return VertexPNT3(x, y, z, xn, yn, zn, u, v, _isTransparent ? d(e) : 1.0f);
}


void Pyramid::createIndexBuffer(ID3D11Device* device)
{
	_indexCount = 18;

	USHORT indices[] = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11, 
		12, 13, 14,
		12, 14, 15,
	};

	D3D11_BUFFER_DESC desc{};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = _indexCount * sizeof(USHORT);

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = indices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_indexBuffer)))) {
		throw new std::exception("Could not create index buffer. Pyramid.cpp:createIndexBuffer()");
	}
}