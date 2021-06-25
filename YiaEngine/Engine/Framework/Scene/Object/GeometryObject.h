#pragma once
#ifndef SCENE_GEOMETRY_OBJECT_H
#define SCENE_GEOMETRY_OBJECT_H

#endif // !SCENE_GEOMETRY_OBJECT_H


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
	}
}
