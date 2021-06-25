#pragma once

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

			//std::string Serialize()
			//{
			//	std::stringstream ss;
			//	std::string ret;
			//	ss << "MeshObject\n";
			//	ss << "primitive:";
			//	ss << Meta::Serialize(primitive_);
			//	ss << "\n";
			//	ss << "Vertex List\n";
			//	for (int i = 0; i < vertex_arrays_.size(); i++)
			//	{
			//		ss << "#" << i << "\n";
			//		//	ss << vertex_arrays_[i].Serialize();
			//	}
			//	ss >> ret;
			//	return ret;
			//}
		private:
			int lod_;
			MeshPrimitive primitive_;
			std::vector<IndexArray>	index_arrays_;
			std::vector<VertexArray>vertex_arrays_;
		};
	}
	META_ENUM(Scene::MeshPrimitive);
}

