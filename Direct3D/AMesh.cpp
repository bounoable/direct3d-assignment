#include "AMesh.h"


AMesh::~AMesh()
{
	if (_vertexBuffer != nullptr) {
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}

	if (_indexBuffer != nullptr) {
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	if (_shader != nullptr) {
		delete _shader;
		_shader = nullptr;
	}
}

void AMesh::init(ID3D11Device* device)
{
	createVertexBuffer(device);
	createIndexBuffer(device);
}

void AMesh::update(FLOAT dt)
{
}

XMMATRIX AMesh::render(ID3D11DeviceContext* deviceContext)
{
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &_stride, &offset);
	deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return
		XMMatrixScaling(_data.scale.x, _data.scale.y, _data.scale.z) *
		XMMatrixRotationRollPitchYaw(_data.rotation.x, _data.rotation.y, _data.rotation.z) *
		XMMatrixTranslation(_data.position.x, _data.position.y, _data.position.z);
}

void AMesh::rotateXClockwise(float amount)
{
	_data.rotation.x -= amount;
}

void AMesh::rotateYClockwise(float amount)
{
	_data.rotation.y -= amount;
}

void AMesh::rotateZClockwise(float amount)
{
	_data.rotation.z -= amount;
}


void AMesh::calculateTangentAndBinormal(VertexPNT3 &v1, VertexPNT3 &v2, VertexPNT3 &v3)
{
	// calculate triangle vectors
	XMFLOAT3 p1, p2;
	XMStoreFloat3(&p1, XMLoadFloat3(&v2.position) - XMLoadFloat3(&v1.position));
	XMStoreFloat3(&p2, XMLoadFloat3(&v3.position) - XMLoadFloat3(&v1.position));

	// calculate triangle vectors in texture space
	XMFLOAT2 t1, t2;
	XMStoreFloat2(&t1, XMLoadFloat2(&v2.uv) - XMLoadFloat2(&v1.uv));
	XMStoreFloat2(&t2, XMLoadFloat2(&v3.uv) - XMLoadFloat2(&v1.uv));

	// calculate denominator for tangent/binormal cross products
	float denominator = 1.0f / (t1.x * t2.y - t2.x * t1.y);

	// calculate tangent and binormal
	XMVECTOR tangent = XMVectorSet(
		t2.y * p1.x - t1.y * p2.x,
		t2.y * p1.y - t1.y * p2.y,
		t2.y * p1.z - t1.y * p2.z,
		0.0f
	) * denominator;

	XMVECTOR binormal = XMVectorSet(
		t1.x * p2.x - t2.x * p1.x,
		t1.x * p2.y - t2.x * p1.y,
		t1.x * p2.z - t2.x * p1.z,
		0.0f
	) * denominator;

	// normalize tangent and binormal
	tangent = XMVector3Normalize(tangent);
	binormal = XMVector3Normalize(binormal);

	// normal calculation
	XMVECTOR normal = XMVector3Cross(tangent, binormal);

	// store normal, tangent and binormal
	//XMStoreFloat3(&v1.normal, normal);
	//XMStoreFloat3(&v2.normal, normal);
	//XMStoreFloat3(&v3.normal, normal);

	XMStoreFloat3(&v1.tangent, tangent);
	XMStoreFloat3(&v2.tangent, tangent);
	XMStoreFloat3(&v3.tangent, tangent);

	XMStoreFloat3(&v1.binormal, binormal);
	XMStoreFloat3(&v2.binormal, binormal);
	XMStoreFloat3(&v3.binormal, binormal);
};