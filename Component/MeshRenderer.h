#pragma once

#include"Core/Mesh.h"
#include"Renderer/Material.h"
#include"Component.h"
namespace YiaEngine
{

	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(GameObject& obj) :Component(obj) {}
		std::shared_ptr <Mesh> pMesh;
		std::shared_ptr<Graphic::Material> pMaterial;

	};

}