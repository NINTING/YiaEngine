#pragma once

#ifndef SCENE_MESH_OBJECT_H
#define SCENE_MESH_OBJECT_H


#include<sstream>
#include<string>





#include"BaseSceneObject.h"
#include"VertexArray.h"
#include"IndexArray.h"
#include"SceneEnum.h"
#include"Engine/Meta/Meta.h"

namespace YiaEngine
{
	
	namespace Scene {
		enum class MeshPrimitive
		{
			kPoint,
			kLine,
			kTriangle
		};
		//YiaEngine::Meta::EnumMeta<MeshPrimitive>;
		//META_ENUM(MeshPrimitive);
	
		class MeshObject :public BaseSceneObject
		{

		public:

			MeshObject(MeshPrimitive primitive = MeshPrimitive::kTriangle, int lod = 0) :
				BaseSceneObject(SceneObjectType::kMeshObject),
				primitive_(primitive),
				lod_(lod) {}

			MeshObject(MeshObject&& mesh) :
				BaseSceneObject(SceneObjectType::kMeshObject),
				lod_(mesh.lod_),
				primitive_(mesh.primitive_),
				index_arrays_(std::move(mesh.index_arrays_)),
				vertex_arrays_(std::move(mesh.vertex_arrays_)) {}

			void set_vertex_arrays(const std::vector<VertexArray>& vertex_arrays)
			{
				vertex_arrays_ = vertex_arrays;
			}
			void set_index_arrays(const std::vector<IndexArray>& index_arrays)
			{
				index_arrays_ = index_arrays;
			}
			void add_vertex_array(const VertexArray& vertex_array)
			{
				vertex_arrays_.push_back(vertex_array);
			}
			void add_index_array(const IndexArray& index_array)
			{
				index_arrays_.push_back(index_array);
			}
			void set_primitive(MeshPrimitive primitive)
			{
				primitive_ = primitive;
			}
			std::vector<VertexArray> vertex_arrays()
			{
				return vertex_arrays_;
			}
			
			const VertexArray GetAttribute(VertexAttribute attribute)const
			{
				for (auto it = vertex_arrays_.begin(); it != vertex_arrays_.end(); it++)
				{
					if (it->isAttribute(attribute))
						return *it;
				}
				return VertexArray{};
			}

			const VertexArray GetPosition() const 
			{
				return GetAttribute(VertexAttribute::kPosition);
			}
			const VertexArray GetUVs() const
			{
				return GetAttribute(VertexAttribute::kTexcoord);
			}
			const VertexArray GetNormal() const
			{
				return GetAttribute(VertexAttribute::kNormal);
			}
			const IndexArray GetIndexArray(int i)const
			{
				assert(i < index_arrays_.size());
				return index_arrays_[i];
			}
			/*void TMP_SetMeshData(const Graphic::GpuBuffer& meshdata)
			{
				meshData_ = meshdata;
			}
			void TMP_SetVertexBufferView(int index,const D3D12_VERTEX_BUFFER_VIEW& vbo)
			{
				vbo_[index] = vbo;
			}
			void TMP_SetIndexBufferView(const D3D12_INDEX_BUFFER_VIEW& vbo)
			{
				ibo_ = vbo;
			}
			*/
	/*		D3D12_VERTEX_BUFFER_VIEW* VertexView(size_t index) { return &vbo_[index]; };
			D3D12_INDEX_BUFFER_VIEW* IndexView() { return &ibo_; };*/
		private:
			void GenerateMeshData();
		private:
			int lod_;
			MeshPrimitive primitive_;
			std::vector<IndexArray>	index_arrays_;
			std::vector<VertexArray>vertex_arrays_;
		//	Graphic::GpuBuffer meshData_;
			/*D3D12_VERTEX_BUFFER_VIEW vbo_[10];
			D3D12_INDEX_BUFFER_VIEW ibo_;*/
		};
	}
///	META_ENUM(Scene::MeshPrimitive);
}

#endif //SCENE_MESH_OBJECT_H