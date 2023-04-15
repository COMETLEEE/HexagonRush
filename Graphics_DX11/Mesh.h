#pragma once
#include "ResourceBase.h"

#include "Renderer.h"

namespace GraphicsEngine
{
	class Mesh : public ResourceBase<RESOURCE_TYPE::MESH>
	{
	public:
		template <typename TVertex = StaticMeshVertex>
		void Initialize(const tstring& meshName, const std::vector<TVertex>& vertexBuffer, const std::vector<uint32>& indexBuffer,
			bool isDynamicMesh = false, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		template <typename TVertex = StaticMeshVertex>
		void ResetVertexBuffer(const std::vector<TVertex>& vertexBuffer, D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		void ResetIndexBuffer(const std::vector<uint32>& indexBuffer);

		void Update();

		void Release();

		ComPtr<ID3D11Buffer> _vertexBuffer;

		ComPtr<ID3D11Buffer> _indexBuffer;

		D3D11_PRIMITIVE_TOPOLOGY _topology;

		uint32 _stride;

		// 이것은 어떤 경우에 사용할 수 있을까 ..? 고민 중
		uint32 _offset;

		uint32 _indexBufferSize;

		bool _isDynamicMesh;
	};

	template <typename TVertex>
	void Mesh::Initialize(const tstring& meshName, const std::vector<TVertex>& vertexBuffer, const std::vector<uint32>& indexBuffer, bool isDynamicMesh /*= false*/, D3D11_PRIMITIVE_TOPOLOGY topology /*= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
	{
		_resourceName = meshName;

		_topology = topology;

		_isDynamicMesh = isDynamicMesh;

		/// <summary>	
		/// Vertex Buffer를 만드는 과정
		/// </summary>
		D3D11_BUFFER_DESC vertexBufferDesc;
		vertexBufferDesc.ByteWidth = sizeof(TVertex) * static_cast<unsigned int>(vertexBuffer.size());

		if (!isDynamicMesh)
		{
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.CPUAccessFlags = 0;

		}
		else
		{
			vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA vertexInitData;
		vertexInitData.pSysMem = vertexBuffer.data();

		Renderer::Get()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexInitData, &_vertexBuffer);

		_stride = sizeof(TVertex);
		_offset = 0;

		/// <summary>
		/// Index Buffer를 만드는 과정
		/// </summary>
		D3D11_BUFFER_DESC indexBufferDesc;

		indexBufferDesc.ByteWidth = sizeof(uint32) * static_cast<uint32>(indexBuffer.size());

		if (!isDynamicMesh)
		{
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.CPUAccessFlags = 0;
		}
		else
		{
			indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}

		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA indexInitData;
		indexInitData.pSysMem = indexBuffer.data();

		Renderer::Get()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexInitData, &_indexBuffer);

		_indexBufferSize = static_cast<uint32>(indexBuffer.size());
	}

	template <typename TVertex /*= StaticMeshVertex*/>
	void Mesh::ResetVertexBuffer(const std::vector<TVertex>& vertexBuffer, D3D11_PRIMITIVE_TOPOLOGY topology /*= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST*/)
	{
		if (!_isDynamicMesh)
			return;

		ComPtr<ID3D11DeviceContext> deviceContext = Renderer::Get()->GetDeviceContext();

		// Vertex Buffer Mapping
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		deviceContext->Map(_vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

		TVertex* mappedData = (TVertex*)mappedResource.pData;

		memcpy(mappedData, (void*)vertexBuffer.data(), sizeof(TVertex) * vertexBuffer.size());

		deviceContext->Unmap(_vertexBuffer.Get(), 0);
	}
}