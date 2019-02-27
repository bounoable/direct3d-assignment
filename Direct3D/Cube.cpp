#include "Cube.h"
#include "Vertex.h"
#include <exception>

void Cube::createIndexBuffer(ID3D11Device* device)
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

	D3D11_BUFFER_DESC desc {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = _indexCount * sizeof(USHORT);

	D3D11_SUBRESOURCE_DATA data {};
	data.pSysMem = indices;

	if (!(SUCCEEDED(device->CreateBuffer(&desc, &data, &_indexBuffer)))) {
		throw new std::exception("Could not create index buffer. Cube.cpp:createIndexBuffer()");
	}
}