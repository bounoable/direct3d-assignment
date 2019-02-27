#pragma once
#include "Cube.h"

class TextureCube :
	public Cube
{
public:
	TextureCube(AShader* shader, MeshData data = MeshData()) : Cube(shader, data) {};
	~TextureCube()
	{
		Cube::~Cube();
	}

	void makeTransparent()
	{
		_isTransparent = true;
	}

protected:
	bool _isTransparent = false;

	void createVertexBuffer(ID3D11Device* device);
	VertexPNT3 createVertex(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v);
};
