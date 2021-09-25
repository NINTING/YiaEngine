
#include"Common/DataFormat.h"

namespace YiaEngine
{
	namespace Graphic
	{

		enum class VertexAttributeType:int
		{
			kPosition,
			kNormal,
			kTangent,
			kBitangent,
			kColor,
			kTexcoord0,
			kTexcoord1,
			kTexcoord2,
			kVertexAttributeTypeNum
		};
		enum class MeshPrimitive
		{
			kUnkown,
			kPoint,
			kLine,
			kTriangle
		};
		struct VertexAttribute
		{
			VertexAttribute(VertexAttributeType attribute, DataFormat format, void* data = nullptr, size_t vertexCount = 0);
			~VertexAttribute() { 
				if (Data!=nullptr)
				delete Data;
			}
			DataFormat Format;
			size_t DataSize;
			size_t Stride;
			void* Data;
			VertexAttributeType Type;
		};
		struct SubMesh
		{
			SubMesh() = default;
			SubMesh(size_t startIndex, size_t count) :StartIndex(startIndex), Count(count) {}
		
			size_t StartIndex;
			size_t Count;
			//网格构成存在不连续时，将会使用strip形式，
			//当存在line_strip 或者 triangle_trip时，会在不连续的地方
			//给出重置信号，restart就是这个重置信号。

			//when the mesh is discontinuous,data_type_ = xxx_trip
			//when the data_type_ = xxx_trip, the reset signal will be given at the 
			//discontinus place. restart is the reset signal
			uint64_t Restart;
		};
		class Mesh
		{
		public:
			Mesh();
			void SetVertexAttributes(UINT vertexCount, size_t attributeCount, VertexAttribute* layout);
			void SetVertexAttributes(UINT vertexCount, const std::vector<VertexAttribute>& layout);
			void SetPrimitive(MeshPrimitive primitive);
			void SetVertexData(VertexAttributeType type,void*data);
			void SetIndexData(UINT count,void* data,DataFormat format = DataFormat::kUint32_1);
			//TODO 子网格处理
			void SetSubMeshCount(size_t count);
			void SetSubMesh(size_t index, const SubMesh&);
		public:
			const VertexAttribute& Positions()const;
			const VertexAttribute& Uv0()const;
			const VertexAttribute& Uv1()const;
			void* IndexData()const;
			UINT IndexCount()const;
			UINT VertexCount()const {return vertexCount_;};

			const VertexAttribute* GetVertexAttribute(VertexAttributeType type)const;
		private:
			int lod_;
	
			MeshPrimitive primitive_;
			std::vector<SubMesh>subMeshArrays_;
			std::vector<VertexAttribute>vertexAttributes_;
			void* indexData_;
			UINT indexCount_;
			DataFormat indexFormat_;
			UINT vertexCount_;
		
		};
	}
}