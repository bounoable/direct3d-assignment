#pragma once
#include "AMesh.h"

class Cube :
	public AMesh
{
public:
	Cube(AShader* shader, MeshData data = MeshData()) : AMesh(shader, data) {};
	~Cube()
	{
		AMesh::~AMesh();
	}

protected:
	virtual void createVertexBuffer(ID3D11Device* device) = 0;
	void createIndexBuffer(ID3D11Device* device);
};
