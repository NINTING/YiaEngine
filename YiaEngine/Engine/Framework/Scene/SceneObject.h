#pragma once
#ifndef FRAMEWORLK_SCENE_OBJECT_H_
#define FRAMEWORLK_SCENE_OBJECT_H_

#define WINDOWS_PLATFORM

#include<memory>
#include<string>
#include<vector>
#include<sstream>

#include<assert.h>

//#include"MagicEnum.h"
#include "crossguid/guid.hpp"

#include "Common/MacroHelp.h"
#include "Common/ImageParser/Image.h"
#include "Math/Geometry.h"
#include "Engine/Meta/Meta.h"


namespace YiaEngine
{
	class BaseSceneObject;
	class MeshObject;
	class GeometryObject;

	class VertexArray;
	class IndexArray;
	class TextureObject;
	class LightObject;
	class SpotLight;
	class PointLight;
	class CameraObject;
	class MaterialObject;
	class Transform;
	class Translation;
	class Rotation;
	class Scale;

	enum class DataType : int
	{
		kUint16_1 = 1,
		kUint32_1 = 2,
		kFloat_1  = 3,
		kDouble_1 = 4,
		kUint16_2 = 5,
		kUint32_2 = 6,
		kFloat_2  = 7,
		kDouble_2 = 8,
		kUint16_3 = 9,
		kUint32_3 = 10,
		kFloat_3  = 11,
		kDouble_3 = 12,
		kUint16_4 = 13,
		kUint32_4 = 14,
		kFloat_4  = 15,
		kDouble_4 = 16,
	};

	META_ENUM(DataType);
	
	static int getDataTypeForCount(DataType value)
	{
		return  (static_cast<int>(value)-1) / 4 + 1;
	}
	

	enum class SceneObjectType
	{
		kGemometryObject,
		kMeshObject,
		kTextureObject,
		kMaterialObject,
		kLightObject,
		kCameraObject
	};


	enum class MeshPrimitive
	{
		kPoint,
		kLine,
		kTriangle
	};
	//YiaEngine::Meta::EnumMeta<MeshPrimitive>;
	META_ENUM(MeshPrimitive);

	
		
	/*template<>
	struct YiaEngine::Meta::EnumMeta<MeshPrimitive>
	{
		typedef MeshPrimitive type;
		static constexpr std::string_view type_name{};
		static constexpr size_t count = enum_count<MeshPrimitive>();
		static constexpr std::array<MeshPrimitive, count>values{};
		static constexpr std::array<std::string_view, count> names{};
		static constexpr Core::CategoryTag tag = Core::CategoryTag::kEnumClassTag;
	};*/
	

	enum class VertexAttribute
	{
		kPosition,
		kNormal,
		kTangent,
		kBitangent,
		kColor,
		kTexcoord
	};

	META_ENUM(VertexAttribute)
	//http://www.opengex.org/opengex-spec.pdf
	
	//BaseSceneObject提供Guid，所有SceneObject类型都表示一个持久化的Asset
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
	class GeometryObject :public BaseSceneObject
	{

	public:
		GeometryObject() :BaseSceneObject(SceneObjectType::kGemometryObject) {}
		GeometryObject(const GeometryObject&) = default;
		GeometryObject(GeometryObject&&) = default;
		void AddMesh(const std::shared_ptr<MeshObject>& mesh) { meshs_.push_back(mesh); }
		GeometryObject& operator =(const GeometryObject&) = default;
		std::string Serialize()
		{
			std::string ret;
			std::stringstream ss;
			ss << "GeometryObject\n";
			
			ss << meshs_.size();
			for (int i = 0; i < meshs_.size(); i++)
			{
				auto m = meshs_[i];
				//m->Serialize();
				//ss << s;
			}
			ss >> ret;
			return ret;
		}
	private:
		bool visible_ = true;
		bool shadow_ = false;
		bool motion_blur_ = false;

		//每个Lod 对应一个mesh
		std::vector<std::shared_ptr<MeshObject>> meshs_;
	};
	class MeshObject :public BaseSceneObject
	{

	public:
	
		MeshObject(MeshPrimitive primitive = MeshPrimitive::kTriangle, int lod = 0):
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
		
		std::string Serialize()
		{
			std::stringstream ss;
			std::string ret;
			ss << "MeshObject\n";
			ss << "primitive:";
			ss << Meta::Serialize(primitive_);
			ss << "\n";
			ss << "Vertex List\n";
			for (int i = 0; i < vertex_arrays_.size(); i++)
			{
				ss << "#" << i << "\n";
			//	ss << vertex_arrays_[i].Serialize();
			}
			ss >> ret;
			return ret;
		}
	private:
		int lod_;
		MeshPrimitive primitive_;
		std::vector<IndexArray>	index_arrays_;
		std::vector<VertexArray>vertex_arrays_;
	};

	//代表每个顶点的属性的集合
	//采用Struct Of Array组织形式

	class VertexArray
	{

	public:
		VertexArray(VertexAttribute attribute,
			DataType data_type = DataType::kFloat_3,
			void*data = nullptr,size_t size = 0):
			attribute_(attribute),data_type_(data_type),size_(size),data_(data){}
		VertexArray(const VertexArray& vertexarray) = default;
		VertexArray(VertexArray&& vertexarray) = default;
		VertexArray& operator = (const VertexArray&) = default;
		VertexArray& operator = (VertexArray&&) = default;
		std::string Serialize()
		{
			std::string ret;
		/*	std::stringstream ss;
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
			ss >> ret;*/
			return ret;
		}
	private:
		DataType data_type_;
		VertexAttribute attribute_;
		void* data_;
		size_t size_;
	};

	class IndexArray
	{
	public:
		IndexArray(void* data , size_t size,
			DataType data_type = DataType::kUint16_1,uint32_t material_id = 0) :
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

		//网格构成存在不连续时，将会使用strip形式，
		//当存在line_strip 或者 triangle_trip时，会在不连续的地方
		//给出重置信号，restart就是这个重置信号。

		//when the mesh is discontinuous,data_type_ = xxx_trip
		//when the data_type_ = xxx_trip, the reset signal will be given at the 
		//discontinus place. restart is the reset signal
		uint64_t restart_;

	};


	enum class AddressingMode
	{
		kRepeat,
		kClamp,
		kBorder,
		kMirror_Reapt,
		kMirror_Clamp,
		kMirror_Border
	};

	class TextureObject:public BaseSceneObject {
	public:
	private:
		std::string name_;
		std::shared_ptr<Image>image_;
		//指定使用网格哪一个纹理坐标集
		uint32_t texcoord_ = 0;

		//应用于纹理坐标的转换
		//包含以下几种矩阵类型
		//Transform,Translation,Rotation,Scale
		std::vector<Mat4x4f> transform_;
		
		AddressingMode x_address_;
		AddressingMode y_address_;
		AddressingMode z_address_;
		//当时Border模式下，border的颜色
		Color border_;

		TextureObject() :
			BaseSceneObject(SceneObjectType::kTextureObject),
			image_(nullptr),
			x_address_(AddressingMode::kRepeat),
			y_address_(AddressingMode::kRepeat),
			z_address_(AddressingMode::kRepeat),
			border_(Color::Zero()) {}
		TextureObject(TextureObject&) = default;
		TextureObject(TextureObject&&) = default;
		TextureObject(std::string name):BaseSceneObject(SceneObjectType::kTextureObject), name_(name){}
		TextureObject(uint32_t texcoord,const std::shared_ptr<Image>&image):
			BaseSceneObject(SceneObjectType::kTextureObject),
			texcoord_(texcoord),
			image_(image){}
		TextureObject(uint32_t texcoord, const std::shared_ptr<Image>&& image):
			BaseSceneObject(SceneObjectType::kTextureObject),
			texcoord_(texcoord),
			image_(std::move(image)) {}

	};

	//一个混合参数(value,texture)
	//对于一个材质属性diffuse
	//既可以是color或者texture
	//the VariatParam have (value,texture)
	//for a material attribute diffuse
	//it can be color or(and) texture
	template<typename T>
	class VariantParam
	{

	public:
		VariantParam() = default;
		VariantParam(const T& value) :value_(value) {}
		VariantParam(const std::shared_ptr<TextureObject>& texture) :texture_(texture) {}
		VariantParam(const VariantParam&) = default;
		VariantParam(VariantParam&&) = default;

		VariantParam(std::shared_ptr<TextureObject>&& texture):texture_(std::move(texture)) {}
	
		/*const T value() { return value_; }
		void set_value(const T& value) { value_ = value; }

		const std::shared_ptr<TextureObject> texture() { return texture_; }
		void set_texture(std::shared_ptr<TextureObject>& texture) { texture_ = texture; }*/


	private:
		T value_;
		std::shared_ptr<TextureObject>texture_;
	//	T value_;
	//	std::shared_ptr<TextureObject>texture_;
	};
	using ColorParam = VariantParam<Color>;
	using ValueParam = VariantParam<float>;
	using Value = float;

	class MaterialObject:public BaseSceneObject
	{

	protected:
		MaterialObject() :BaseSceneObject(SceneObjectType::kMaterialObject) {};
		std::string name_;
		bool two_side_;
	};
	class PBRMaterial:public MaterialObject
	{
	public:
		PBRMaterial() = default;
		PBRMaterial(const PBRMaterial&) = default;
		//PBRMaterial(const PBRMaterial&&) = default;
		PBRMaterial& operator=(const PBRMaterial&) = default;
		PBRMaterial& operator=(PBRMaterial&&) = default;
	private:
		ColorParam diffuse_;
		ColorParam specular_;
		ValueParam specular_power_;
		ColorParam emission_;
		ValueParam opacity_;
		ColorParam transparency_;
		TextureObject norma_map_;
		TextureObject height_map_;
		ValueParam height_scale_;
		TextureObject occlution_;
		ValueParam roughness_;
		ValueParam metalness_;
		//清漆材质
		ColorParam clearcoat_;
		ValueParam clearcoat_roughness_;
		ColorParam sheen_;
		ValueParam sheen_roughness_;
		Value ior_;
	};

	enum class LightType
	{
		kPointLight,
		kSpotLight,
		kDirectionLight,
	};
	enum class AttenType
	{
		kNone,
		kLinearAtten,
	};
	class AttenCurve {
		static float Atten(AttenType type, float instance, float distance)
		{
			if (type == AttenType::kLinearAtten)
				return LinearAtten(instance, distance);
			return instance;
		}

		static float LinearAtten(float instance, float distance)
		{
			return instance * 1.f / distance;
		}
	};
	class LightObject :public BaseSceneObject
	{
	protected:
		LightObject():BaseSceneObject(SceneObjectType::kLightObject){}
		
		bool shadow_;
		float instansity_;
		//LightType type_;
		Color color_;
		AttenType atten_type_;
		float near_clip_;
		float far_clip_;
	};
	class SpotLight:public LightObject
	{
	public:
		SpotLight():LightObject(){}
	private:
		float inner_angle_;
		float out_angle_;
	};

	class PointLight :public LightObject
	{
	public:
		PointLight():LightObject() {}
	};

	class DirectionLight :public LightObject
	{
		DirectionLight():LightObject() {}
	};


	enum class FovType
	{
		kHorizontal,
		kVertical
	};
	enum class CameraType
	{
		Perspective,
		Orthogonal
	};
	class CameraObject :public BaseSceneObject
	{
	public:
		CameraObject():BaseSceneObject(SceneObjectType::kCameraObject){}
	private:
		FovType fov_type_;
		float near_clip_;
		float far_clip_;
		float aspect_;
	};

	enum class AxisType
	{
		kXAxis,
		kYAxis,
		kZAxis
	};

	//基本的空间变换
	class Transform
	{
	public:
		Transform(Mat4x4f mat,bool object_only = false):matrix_(mat),object_only_(object_only){}
		Transform() :matrix_(Mat4x4f::Identity()), object_only_(false){}
	protected:
		Mat4x4f matrix_;
		bool object_only_;
	};

	class Translation :public Transform
	{
	public:
		Translation() = default;
		Translation(float dis_x, float dis_y, float dis_z):Transform()
		{
			matrix_[3][0] = dis_x;
			matrix_[3][1] = dis_y;
			matrix_[3][2] = dis_z;
		}
		Translation(AxisType axis,float value):Transform()
		{
			switch (axis)
			{
			case YiaEngine::AxisType::kXAxis:
				matrix_[3][0] = value;
				break;
			case YiaEngine::AxisType::kYAxis:
				matrix_[3][1] = value;
				break;
			case YiaEngine::AxisType::kZAxis:
				matrix_[3][2] = value;
				break;
			default:
				assert(0);
			}
			
		}
	};

	class Rotation :public Transform
	{

	};

	class Scale :public Transform
	{

	};

}	//YiaEngine
#endif //FRAMEWORLK_SCENE_OBJECT_H_