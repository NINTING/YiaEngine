#include"Core/Graphic.h"
#include"Common/DataUtility.h"
#include "Mesh.h"
namespace YiaEngine
{
	namespace Graphic
	{
		VertexAttribute::VertexAttribute(VertexAttributeType attribute, DataFormat format, void* pData,size_t vertexCount)
			:Attribute(attribute), Format(format) 
		{
			Stride = DataFormatStride(format);
			if (pData)
			{
				memcpy(Data, pData,vertexCount * Stride);
			}
		}
		
	

		void Mesh::SetVertexAttributes(size_t vertexCount, size_t attributeCount, VertexAttribute* layout)
		{
			vertexCount_ = vertexCount;
			for (int i = 0; i < attributeCount; i++)
				vertexAttributes_.emplace_back(layout[i]);
		}
		void Mesh::SetVertexAttributes(size_t vertexCount, const std::vector<VertexAttribute>& layout)
		{
			vertexCount_ = vertexCount;
			vertexAttributes_.assign(layout.begin(), layout.end());
		}
		void Mesh::SetVertexData(VertexAttributeType type, void* data)
		{
			for (auto it = vertexAttributes_.begin(); it != vertexAttributes_.end(); it++)
			{
				if (it->Attribute == type)
				{
					it->DataSize = it->Stride * vertexCount;
					memcpy(it->Data, data, it->DataSize);
				}
			}
		}
		void Mesh::SetIndexData(size_t count, void* data, DataFormat format)
		{
			memcpy(indexData_, data, count * DataFormatStride(format));
			indexCount = count;
			subMeshArrays_.resize(1);
			subMeshArrays_.emplace_back({0,count})
		}
		void Mesh::SetPrimitive(MeshPrimitive primitive)
		{
			primitive_ = primitive;
		}
	}
}