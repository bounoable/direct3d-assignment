#pragma once
#include "AMesh.h"
#include "TextureShader.h"

class Skybox :
	public AMesh
{
public:
	Skybox(TextureShader* shader, MeshData data = MeshData()) : AMesh(shader, data) {};
	~Skybox()
	{
		AMesh::~AMesh();
	}

	TextureShader* getTextureShader()
	{
		return static_cast<TextureShader*>(_shader);
	}

protected:
	void createVertexBuffer(ID3D11Device* device);
	void createIndexBuffer(ID3D11Device* device);
};

