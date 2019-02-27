#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include "AShader.h"

using namespace DirectX;

struct MeshData
{
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
};

class AMesh
{
public:
	AMesh(AShader* shader, MeshData data = MeshData())
	{
		_shader = shader;
		_data = data;
	}

	virtual ~AMesh();

	void init(ID3D11Device* device);
	void update(FLOAT dt);
	XMMATRIX render(ID3D11DeviceContext* deviceContext);

	void rotateYClockwise(float amount);
	void rotateYCounterClockwise(float amount)
	{
		rotateYClockwise(-amount);
	}

	void rotateXClockwise(float amount);
	void rotateXCounterClockwise(float amount)
	{
		rotateXClockwise(-amount);
	}

	void rotateZClockwise(float amount);
	void rotateZCounterClockwise(float amount)
	{
		rotateZClockwise(-amount);
	}

	USHORT getIndexCount()
	{
		return _indexCount;
	}

	AShader* getShader()
	{
		return _shader;
	}

	void setShader(AShader* shader)
	{
		_shader = shader;
	}

	XMFLOAT3 getPosition()
	{
		return _data.position;
	}

	void setPosition(XMFLOAT3 position)
	{
		_data.position = position;
	}

	XMFLOAT3 getRotation()
	{
		return _data.rotation;
	}

	XMFLOAT3 getScale()
	{
		return _data.scale;
	}

	void setScale(XMFLOAT3 scale)
	{
		_data.scale = scale;
	}

protected:
	virtual void createVertexBuffer(ID3D11Device* device) = 0;
	virtual void createIndexBuffer(ID3D11Device* device) = 0;
	void calculateTangentAndBinormal(VertexPNT3 &v1, VertexPNT3 &v2, VertexPNT3 &v3);
	
	ID3D11Buffer* _vertexBuffer = nullptr;
	ID3D11Buffer* _indexBuffer = nullptr;
	UINT _vertexCount = 0;
	USHORT _indexCount = 0;
	UINT _stride = 0;

	AShader* _shader;
	MeshData _data;
};

