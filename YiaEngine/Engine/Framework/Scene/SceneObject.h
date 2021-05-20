#pragma once

#include<memory>
#include "guid.hpp"
#include<string>

#define EnumToString(s) #s

enum SceneObjectType
{

};

class BaseSceneObject
{
protected:
	xg::Guid m_Guid;
	SceneObjectType m_Type;
protected:
	BaseSceneObject(xg::Guid&guid,SceneObjectType type):m_Type(type),m_Guid(guid){}
	BaseSceneObject(xg::Guid&& guid, SceneObjectType type) :m_Type(type), m_Guid(std::move(guid)) {}
	BaseSceneObject(BaseSceneObject&& obj) :m_Guid(std::move(obj.m_Guid)),m_Type(std::move(obj.m_Type)) {}
	BaseSceneObject& operator = (BaseSceneObject&& obj) {
		m_Guid =std::move(obj.m_Guid);
		m_Type = std::move(obj.m_Type);
	}
private:
	BaseSceneObject() = delete;
	BaseSceneObject(BaseSceneObject& obj) = delete;
	BaseSceneObject& operator = (BaseSceneObject& obj) = delete;
public:
	xg::Guid GetGuid()
	{
		return m_Guid;
	}
	std::string ToString()
	{
		std::string s;
		
		s.append("SceneObject:");
		s.append("Guid:");
		s.append(m_Guid.str());
		s.append("Type:");
		s.append(EnumToString(m_Type));
	}
};


class VertexSceneObject:BaseSceneObject
{

};