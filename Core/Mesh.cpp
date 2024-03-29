#include"pch.h"
//#include"Core/Graphic.h"
//#include"Common/DataFormat.h"
//
//#include "Mesh.h"
//namespace YiaEngine
//{
//	namespace Graphic
//	{
//		VertexAttributeEnum::VertexAttributeEnum(VertexAttributeType attribute, DataFormate format, void* pData,size_t vertexCount)
//			:Type(attribute), Format(format),Data(nullptr),DataSize(0)
//		{
//			Stride = DataFormatStride(format);
//		
//			if (pData)
//			{
//				DataSize = vertexCount * Stride;
//				Data = new uint8_t[DataSize];
//				memcpy(Data, pData, DataSize);
//			}
//		}
//		
//	
//
//		Mesh::Mesh():lod_(0), primitive_(MeshPrimitive::kUnkown), 
//			indexData_(nullptr), indexCount_(0), indexFormat_(DataFormate::kUnkown), vertexCount_(0)
//		{
//			subMeshArrays_.clear();
//			vertexAttributes_.clear();
//			
//		}
//
//		void Mesh::SetVertexAttributes(UINT vertexCount, size_t attributeCount, VertexAttributeEnum* layout)
//		{
//			vertexCount_ = vertexCount;
//			for (size_t i = 0; i < attributeCount; i++)
//				vertexAttributes_.emplace_back(layout[i]);
//		}
//		void Mesh::SetVertexAttributes(UINT vertexCount, const std::vector<VertexAttributeEnum>& layout)
//		{
//			vertexCount_ = vertexCount;
//			vertexAttributes_.assign(layout.begin(), layout.end());
//		}
//		void Mesh::SetVertexData(VertexAttributeType type, void* data)
//		{
//			for (auto it = vertexAttributes_.begin(); it != vertexAttributes_.end(); it++)
//			{
//				if (it->Type == type)
//				{
//					
//					it->DataSize = it->Stride * vertexCount_;
//					if (it->Data == nullptr)
//					{
//						it->Data = new UINT8[it->DataSize];
//					}
//					memcpy((UINT8*)it->Data, (UINT8*)data, it->DataSize);
//				}
//			}
//		}
//		void Mesh::SetIndexData(UINT count, void* data, DataFormate format)
//		{
//			memcpy(indexData_, data, count * DataFormatStride(format));
//			indexCount_ = count;
//			subMeshArrays_.resize(1);
//			subMeshArrays_.emplace_back(SubMesh{ 0,count });
//		}
//		void Mesh::SetPrimitive(MeshPrimitive primitive)
//		{
//			primitive_ = primitive;
//		}
//
//		const VertexAttributeEnum& Mesh::Positions()const
//		{
//			return *GetVertexAttribute(VertexAttributeType::kPosition);
//		}
//		const VertexAttributeEnum& Mesh::Uv0() const
//		{
//			return *GetVertexAttribute(VertexAttributeType::kTexcoord0);
//		}
//		const VertexAttributeEnum& Mesh::Uv1()const
//		{
//			return *GetVertexAttribute(VertexAttributeType::kTexcoord1);
//		}
//		const VertexAttributeEnum* Mesh::GetVertexAttribute(VertexAttributeType type) const
//		{
//			for (auto it = vertexAttributes_.begin(); it != vertexAttributes_.end(); it++)
//			{
//				if (it->Type == type)
//					return &(*it);
//			}
//			return nullptr;
//		}
//		void* Mesh::IndexData()const
//		{
//			return indexData_;
//		}
//		UINT Mesh::IndexCount()const
//		{
//			return indexCount_;
//		}
//	}
//}