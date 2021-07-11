
#ifndef SCENE_GEOMETRY_OBJECT_H
#define SCENE_GEOMETRY_OBJECT_H

#include<memory>
#include<vector>

#include"BaseSceneObject.h"
#include"MeshObject.h"
#include"SceneEnum.h"

namespace YiaEngine
{
	namespace Scene
	{ 
		class GeometryObject :public BaseSceneObject
		{

		public:
			GeometryObject() :BaseSceneObject(SceneObjectType::kGemometryObject) {}
			GeometryObject(const GeometryObject&) = default;
			GeometryObject(GeometryObject&&) = default;
			void AddMesh(const std::shared_ptr<MeshObject>& mesh) { meshes_.push_back(mesh); }
			GeometryObject& operator =(const GeometryObject&) = default;

			std::shared_ptr<MeshObject> GetMeshObject(int i)
			{
				return meshes_[i];
			}
			std::vector<std::shared_ptr<MeshObject>> MeshArray()
			{
				return meshes_;
			}
			int MeshCount()
			{
				return meshes_.size();
			}
			//std::string Serialize()
			//{
			//	std::string ret;
			//	std::stringstream ss;
			//	ss << "GeometryObject\n";

			//	ss << meshs_.size();
			//	for (int i = 0; i < meshs_.size(); i++)
			//	{
			//		auto m = meshs_[i];
			//		//m->Serialize();
			//		//ss << s;
			//	}
			//	ss >> ret;
			//	return ret;
			//}
		private:
			bool visible_ = true;
			bool shadow_ = false;
			bool motion_blur_ = false;

			//每个Lod 对应一个mesh
			std::vector<std::shared_ptr<MeshObject>> meshes_;
		};
	}
}
#endif // !SCENE_GEOMETRY_OBJECT_H
