#include "pch.h"
#include "MeshObject.h"
#include "Common/Utility.h"
namespace YiaEngine
{
	YiaEngine::Mesh::Mesh(const char* name) :name_(name) {}

	Mesh::Mesh(const char* name,UINT count, const VertexAttributeArray* attrs)
		: name_(name), attributes_(attrs,attrs+count)
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
		gemoSize += indices_.size() * sizeof(UINT);
		//	size_t gemoSize = positionAttr.DataSize + uvAttr.DataSize;
		std::stringstream ss;
		ss << name_;
		ss << "_UploadGemoBuffer";

		wchar_t* wname = new wchar_t[ss.str().length()+1];
		Char2Wchar(ss.str().c_str(), wname);

		upload.Create(wname, gemoSize);
		delete wname;
		uint8_t* memory = (uint8_t*)upload.Map();
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

		upload.UnMap();

		wname = new wchar_t[name_.length() + 1];
		Char2Wchar(name_.c_str(), wname);

		gpuBuffer_.Create(wname, gemoSize, 1, upload);
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
}