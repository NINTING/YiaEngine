#pragma once

#include"Core/Mesh.h"
#include"Renderer/Material.h"
namespace YiaEngine
{

	class MeshRenderer
	{
	public:
		std::shared_ptr <Mesh> pMesh;
		std::shared_ptr<Graphic::Material> pMaterial;

	};

}