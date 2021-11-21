#pragma once
#include"pch.h"


#include"Transform.h"
#include"MeshRenderer.h"
#include"RigidBody.h"
namespace YiaEngine
{
	class GameObject
	{
	public:
		GameObject():rigidBody(*this){}
		MeshRenderer meshRenderer;
		RigidBody rigidBody;
		Transform transform;
		void Update();
	private:

	};
}

