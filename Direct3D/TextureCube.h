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

protected:
	void createVertexBuffer(ID3D11Device* device);
};
