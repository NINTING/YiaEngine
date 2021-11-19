#pragma once
#include"pch.h"
#include"Core/GpuBuffer.h"
#include"Common/DataFormat.h"
namespace YiaEngine
{

	enum class VertexAttributeEnum
	{
		kPosition,
		kNormal,
		kTangent,
		kBitangent,
		kColor,
		kTexcoord
	};
	struct VertexAttributeArray
	{

		DataFormate Formate;
		VertexAttributeEnum Attribute;
		std::shared_ptr<uint8_t> Data;
		size_t Stride;
		size_t DataSize;
		size_t Count;
	};


	VertexAttributeArray CreateVertexAttribute(VertexAttributeEnum attribute, DataFormate formate, size_t count, void* data);

	class Mesh
	{
	public:
		Mesh() = default;
		Mesh(const char* Name);
		Mesh(const char* Name, UINT count, const VertexAttributeArray* attrs);
		void SetName(const char* Name) { name_ = Name; }
		void AddAttribute(const VertexAttributeArray& attribute);
		void AddIndices(UINT count,const UINT* indices);
		void CreateMeshGpuBuffer();
		//int GetVertexCount();
		int GetIndexCount() const{ return indices_.size(); };
		Graphic::GpuBuffer& MeshBuffer() { return gpuBuffer_; };
		UINT IndicesSizeInByte() { return indices_.size() * sizeof(UINT); }
		D3D12_VERTEX_BUFFER_VIEW GetVertexBuffer (VertexAttributeEnum attributeEnum)const;
		D3D12_INDEX_BUFFER_VIEW GetIndexBuffer()const;
	private:
		std::vector<VertexAttributeArray> attributes_;
		std::vector <UINT>indices_;
		std::string name_;
		UINT VertexDataSize;
		Graphic::GpuBuffer gpuBuffer_;
	};

}