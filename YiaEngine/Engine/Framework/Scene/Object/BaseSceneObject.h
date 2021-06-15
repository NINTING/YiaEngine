#pragma once
#ifndef SCENE_OBJECT_BASESCENE_OBJECT_H
#define SCENE_OBJECT_BASESCENE_OBJECT_H


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

#endif // SCENE_OBJECT_BASESCENE_OBJECT_H
