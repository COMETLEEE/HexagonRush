#include "pch.h"
#include "Mesh.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	void Mesh::ResetIndexBuffer(const std::vector<uint32>& indexBuffer)
	{
		if (!_isDynamicMesh)
			return;

		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		// Vertex Buffer Mapping
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		deviceContext->Map(_indexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		uint32* mappedData = (uint32*)mappedResource.pData;

		memcpy(mappedData, (void*)indexBuffer.data(), sizeof(uint32) * indexBuffer.size());

		deviceContext->Unmap(_indexBuffer.Get(), 0);
	}

	void Mesh::Update()
	{
		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &_stride, &_offset);

		deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		deviceContext->IASetPrimitiveTopology(_topology);

		deviceContext->DrawIndexed(_indexBufferSize, 0, 0);
	}

	void Mesh::Release()
	{
		_vertexBuffer.ReleaseAndGetAddressOf();

		_indexBuffer.ReleaseAndGetAddressOf();
	}
}