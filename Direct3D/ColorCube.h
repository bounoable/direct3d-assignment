#pragma once
#include "Cube.h"
#include "Vertex.h"

class ColorCube :
	public Cube
{
public:
	ColorCube(AShader* shader, MeshData data = MeshData()) : Cube(shader, data) {};
	~ColorCube()
	{
		Cube::~Cube();
	}

	void useColor(XMFLOAT4 color)
	{
		_overrideColor = color;
		_useOverrideColor = true;
	}

protected:
	bool _useOverrideColor = false;
	XMFLOAT4 _overrideColor { 0.0f, 0.0f, 0.0f, 1.0f };

	void createVertexBuffer(ID3D11Device* device);
	VertexPC createVertex(FLOAT x, FLOAT y, FLOAT z);
};
