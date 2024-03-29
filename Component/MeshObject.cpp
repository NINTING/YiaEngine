#include "pch.h"
#include "MeshObject.h"
#include "Common/Utility.h"
namespace YiaEngine
{
	YiaEngine::Mesh::Mesh(const char* Name) :name_(Name) {}

	Mesh::Mesh(const char* Name,UINT count, const VertexAttributeArray* attrs)
		: name_(Name), attributes_(attrs,attrs+count)
	{

	}

	void Mesh::AddAttribute(const VertexAttributeArray& attribute)
	{
		attributes_.push_back(attribute);
		
	}

	void Mesh::AddIndices(UINT count, const UINT* indices)
	{
		indices_.clear();
		indices_.resize(count);
		indices_.assign(indices, indices + count);
	}

	void Mesh::CreateMeshGpuBuffer()
	{
		Graphic::UploadBuffer upload;
		size_t gemoSize = 0;
		for (size_t i = 0; i < attributes_.size(); i++)
		{
			gemoSize += attributes_[i].DataSize;
		}
		VertexDataSize = gemoSize;
		gemoSize += indices_.size() * sizeof(UINT);
	
		//	size_t gemoSize = positionAttr.DataSize + uvAttr.DataSize;
		std::stringstream ss;
		ss << name_;
		ss << "_UploadGemoBuffer";

		
		uint8_t* memory = new uint8_t[gemoSize];

		UINT offset = 0;
		for (size_t i = 0; i < attributes_.size(); i++)
		{
		
			if (i)
				offset += attributes_[i - 1].DataSize;
			memcpy(memory + offset, attributes_[i].Data.get(), attributes_[i].DataSize);

		}
		offset += attributes_[attributes_.size() - 1].DataSize;
		if (indices_.size())
			memcpy(memory + offset, indices_.data(), indices_.size()*sizeof(UINT));

		


		Graphic::BufferDesc meshBufferDesc = {};
		meshBufferDesc.Size = gemoSize;
		meshBufferDesc.DescriptorType =Graphic::DescriptorTypeFlags::DESCRIPTOR_TYPE_VERTEX_BUFFER | Graphic::DescriptorTypeFlags::DESCRIPTOR_TYPE_INDEX_BUFFER;
		meshBufferDesc.MemoryUsage = Graphic::ResourceUsageEnum::RESOURCE_USAGE_GPU_ONLY;
		meshBufferDesc.InitData = memory;
		gpuBuffer_.Create(meshBufferDesc);

		delete memory;
	}
	VertexAttributeArray CreateVertexAttribute(VertexAttributeEnum attribute, DataFormate formate, size_t count, void* data)
	{
		VertexAttributeArray ret;
		ret.Attribute = attribute;
		ret.Count = count;
		ret.Stride = DataFormatStride(formate);
		ret.DataSize = count * ret.Stride;
		ret.Formate = formate;
		ret.Data = std::shared_ptr<uint8_t>(new uint8_t[ret.DataSize]);
		memcpy_s(ret.Data.get(), ret.DataSize, data, ret.DataSize);

		return ret;
	}

	D3D12_VERTEX_BUFFER_VIEW Mesh::GetVertexBuffer(VertexAttributeEnum attribute)const
	{
		
		size_t offset = 0;
		for (size_t i = 0; i < attributes_.size(); i++)
		{
			if (attributes_[i].Attribute == attribute)
			{
				return gpuBuffer_.VertexBufferView(offset, attributes_[i].Stride, attributes_[i].DataSize);
			}
			offset += attributes_[i].DataSize;
		}
		YIA_ASSERT("尚不支持该属性");
	}
	D3D12_INDEX_BUFFER_VIEW Mesh::GetIndexBuffer() const
	{
		return gpuBuffer_.IndexBufferView(VertexDataSize, sizeof(UINT), indices_.size() * sizeof(UINT));
	}
}