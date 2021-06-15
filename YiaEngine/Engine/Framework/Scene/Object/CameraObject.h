#pragma once
#ifndef SCENE_CAMERA_OBJECT_H
#define SCENE_CAMERA_OBJECT_H

#include"BaseSceneObject.h"

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
	CameraObject() :BaseSceneObject(SceneObjectType::kCameraObject) {}
private:
	FovType fov_type_;
	float near_clip_;
	float far_clip_;
	float aspect_;
};


#endif