#pragma once
#ifndef FRAMEWORLK_SCENE_OBJECT_H_
#define FRAMEWORLK_SCENE_OBJECT_H_



#include<memory>
#include<string>
#include<vector>

#include "crossguid/guid.hpp"

#include "Common/MacroHelp.h"

namespace YiaEngine
{

	enum DataType
	{
		kUint16,
		kUint32,
		kFloat1,
		kFloat2,
		kFloat3,
		kFloat4,
		kDouble
	};

	enum SceneObjectType
	{
		kGemometryObject,
	};


	enum class MeshPrimitive
	{
		kPoint,
		kTriangle
	};

	enum class VertexAttribute
	{
		kPosition,
		kNormal,
		kTangent,
		kBitangent,
		kColor,
		kTexcoord
	};

	//http://www.opengex.org/opengex-spec.pdf

	class BaseSceneObject
	{
	protected:
		xg::Guid guid_;
		SceneObjectType type_;
	protected:
		BaseSceneObject(SceneObjectType type) :type_(type) { guid_ = xg::newGuid(); }
		BaseSceneObject(xg::Guid& guid, SceneObjectType type) :type_(type), guid_(guid) {}
		BaseSceneObject(xg::Guid&& guid, SceneObjectType type) :type_(type), guid_(std::move(guid)) {}
		BaseSceneObject(BaseSceneObject&& obj) :guid_(std::move(obj.guid_)), type_(std::move(obj.type_)) {}
		BaseSceneObject& operator = (BaseSceneObject&& obj) {
			guid_ = std::move(obj.guid_);
			type_ = std::move(obj.type_);
		}
	private:
		BaseSceneObject() = delete;
		BaseSceneObject(BaseSceneObject& obj) = delete;
		BaseSceneObject& operator = (const BaseSceneObject& obj) = delete;
	public:
		xg::Guid GetGuid()
		{
			return guid_;
		}
		std::string ToString()
		{
			std::string s;

			s.append("SceneObject:");
			s.append("Guid:");
			s.append(guid_.str());
			s.append("Type:");
			s.append(ENUM_TO_STRING(type_));
			return s;
		}
	};
	class GemometryObject : BaseSceneObject
	{

	public:
		GemometryObject() :BaseSceneObject(SceneObjectType::kGemometryObject) {}
		GemometryObject(const GemometryObject&) = default;
		GemometryObject(GemometryObject&&) = default;
		GemometryObject& operator =(const GemometryObject&) = default;

	private:
		bool visible_ = true;
		bool shadow_ = false;
		bool motion_blur_ = false;

		//每个Lod 对应一个mesh
		std::vector<std::shared_ptr<Mesh>> meshs_;
	};
	class Mesh
	{

	public:

		Mesh(MeshPrimitive primitive, int lod = 0) :primitive_(primitive), lod_(lod) {}
		Mesh(Mesh&& mesh) :lod_(mesh.lod_),
			primitive_(mesh.primitive_),
			index_arrays_(std::move(mesh.index_arrays_)),
			vertex_arrays_(std::move(mesh.vertex_arrays_)) {}
	private:
		int lod_;
		MeshPrimitive primitive_;
		std::vector<IndexArray>	index_arrays_;
		std::vector<VertexArray>vertex_arrays_;
	};


	class VertexArray
	{

	public:
		VertexArray(VertexAttribute attribute,
			DataType data_type = DataType::kFloat,
			void*data = nullptr,size_t size = 0):
			attribute_(attribute),data_type_(data_type),size_(size),data_(data){}
		VertexArray(const VertexArray& vertexarray) = default;
		VertexArray(VertexArray&& vertexarray) = default;
		VertexArray& operator = (const VertexArray&) = default;
		VertexArray& operator = (VertexArray&&) = default;
	
	private:
		DataType data_type_;
		VertexAttribute attribute_;
		void* data_;
		size_t size_;
	};

	class IndexArray
	{
	public:
		IndexArray(DataType data_type = DataType::kUint16,
			void* data = nullptr, size_t size = 0) :
			data_type_(data_type), size_(size),data_(data)	 {}
		IndexArray(const IndexArray& indexarray) = default;
		IndexArray(IndexArray&& indexarray) = default;
		IndexArray& operator = (const IndexArray&) = default;
		IndexArray& operator = (IndexArray&&) = default;
	private:
		DataType data_type_;
		void* data_;
		size_t size_;

		//一个模型可能会包含多个材质，每个索引列表可能引用不同的材质
		//material_id_与GemometryNode的materialRef相对应
		uint32_t material_id_;

		//网格构成存在不连续时，会使用strip形式，
		//当存在line_strip 或者 triangle_trip时，会在不连续的地方
		//给出重置信号，restart就是这个重置信号。
		uint64_t restart_;

	};

	
	class Material
	{
	private:
		std::string name;
		bool two_side_;

	};
}	//YiaEngine
#endif //FRAMEWORLK_SCENE_OBJECT_H_