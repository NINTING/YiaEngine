

#include"pch.h"

#include "GameObject.h"
namespace YiaEngine
{

	void GameObject::Update()
	{
		rigidBody.Update();
	}
}