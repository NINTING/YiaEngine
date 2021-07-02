#pragma once
#ifndef FRAMEWORK_SCENE_VERTEX_ARRAY_H
#define FRAMEWORK_SCENE_VERTEX_ARRAY_H

#include "SceneEnum.h"

namespace YiaEngine
{
	namespace Scene
	{
		//代表每个顶点的属性的集合
		//采用Struct Of Array组织形式
		
		enum class VertexAttribute
		{
			kPosition,
			kNormal,
			kTangent,
			kBitangent,
			kColor,
			kTexcoord
		};


		class VertexArray
		{
		public:
			VertexArray(VertexAttribute attribute,
				DataType data_type = DataType::kFloat_3,
				void* data = nullptr, size_t size = 0) :
				attribute_(attribute), data_type_(data_type), size_(size), data_(data) {}
			VertexArray(const VertexArray& vertexarray) = default;
			VertexArray(VertexArray&& vertexarray) = default;
			VertexArray& operator = (const VertexArray&) = default;
			VertexArray& operator = (VertexArray&&) = default;
		/*	std::string Serialize()
			{
				std::string ret;
				std::stringstream ss;
				ss << "Vertex Attribute:\n";
				ss << "Attribute: " << Meta::Serialize(attribute_) << "\n";
				ss << "DataType: " << Meta::Serialize(data_type_) << "\n";
				ss << "DataSize: " << size_ << "\n";
				ss << "Data:\n";
				for (int i = 0; i < size_; i++)
				{
					switch (data_type_)
					{
					case YiaEngine::DataType::kFloat_1:
					case YiaEngine::DataType::kFloat_2:
					case YiaEngine::DataType::kFloat_3:
					case YiaEngine::DataType::kFloat_4:
						ss << *(reinterpret_cast<const float*>(data_) + i) << " ";
						break;
					case YiaEngine::DataType::kDouble_1:
					case YiaEngine::DataType::kDouble_2:
					case YiaEngine::DataType::kDouble_3:
					case YiaEngine::DataType::kDouble_4:
						ss << *(reinterpret_cast<const double*>(data_) + i) << " ";
						break;
					}
				}
				ss << "\n";
				ss >> ret;
				return ret;
			}*/
		private:
			DataType data_type_;
			VertexAttribute attribute_;
			void* data_;
			size_t size_;
		};

		
	}//Scene
	META_ENUM(Scene::VertexAttribute)
}// YiaEngine
#endif // !FRAMEWORK_SCENE_INDEX_ARRAY_H
