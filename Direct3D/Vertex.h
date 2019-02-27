#pragma once
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

struct VertexPC
{
	XMFLOAT3 position;
	XMFLOAT4 color;

	VertexPC(FLOAT x, FLOAT y, FLOAT z, FLOAT r, FLOAT g, FLOAT b, FLOAT a = 1.0f) : position(x, y, z), color(r, g, b, a)
	{}
};

struct VertexPT
{
	XMFLOAT3 position;
	XMFLOAT2 uv;

	VertexPT(FLOAT x, FLOAT y, FLOAT z, FLOAT u, FLOAT v): position(x, y, z), uv(u, v)
	{}
};

struct VertexPNT
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

	VertexPNT(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v) : position(x, y, z), normal(xn, yn, zn), uv(u, v)
	{}
};

struct VertexPNT3
{
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT3 binormal;
	XMFLOAT3 tangent;
	XMFLOAT2 uv;

	VertexPNT3(FLOAT x, FLOAT y, FLOAT z, FLOAT xn, FLOAT yn, FLOAT zn, FLOAT u, FLOAT v) : position(x, y, z), normal(xn, yn, zn), uv(u, v)
	{
	}
};