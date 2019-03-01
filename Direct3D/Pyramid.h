#pragma once
#include "AMesh.h"

class Pyramid :
	public AMesh
{
public:
	Pyramid(AShader* shader, MeshData data = MeshData()): AMesh(shader, data)
	{}

	~Pyramid()
	{}

	void makeTransparent()
	{
		_isTransparent = true;
	}

protected:
	bool _isTransparent = false;

	void createVertexBuffer(ID3D11Device* device);
	void createIndexBuffer(ID3D11Device* device);

	VertexPNT3 createVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v);
};

