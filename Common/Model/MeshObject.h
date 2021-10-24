#pragma once
#include"pch.h"
#include"VertexArray.h"
#include"Core/GpuBuffer.h"
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
		Mesh(const char* name);
		Mesh(const char* name, UINT count, const VertexAttributeArray* attrs);
		void SetName(const char* name) { name_ = name; }
		void AddAttribute(const VertexAttributeArray& attribute);
		void AddIndices(UINT count,const UINT* indices);
		void CreateMeshGpuBuffer();
		Graphic::GpuBuffer& MeshBuffer() { return gpuBuffer_; };
		UINT IndicesSizeInByte() { return indices_.size() * sizeof(UINT); }
		D3D12_VERTEX_BUFFER_VIEW GetVertexBuffer();
	private:
		std::vector<VertexAttributeArray> attributes_;
		std::vector <UINT>indices_;
		std::string name_;
		Graphic::GpuBuffer gpuBuffer_;
	};

}