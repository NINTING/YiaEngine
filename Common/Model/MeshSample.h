#pragma once
#include"pch.h"
#include<Common/DataFormat.h>
#include"Component/MeshObject.h"
#include"Common/YiaMath.h"
namespace YiaEngine
{
	namespace MeshSample
	{
		
		std::shared_ptr<Mesh> CreateBox();


		std::shared_ptr<Mesh> CreateSphere(float radius = 1, UINT levels= 20, UINT slices = 20);

		// 创建圆柱体网格数据，slices越大，精度越高。

		std::shared_ptr<Mesh> CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT slices = 20);

		// 创建只有圆柱体侧面的网格数据，slices越大，精度越高
		std::shared_ptr<Mesh> CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20);

		// 创建圆锥体网格数据，slices越大，精度越高。
	
		std::shared_ptr<Mesh>  CreateCone(float radius = 1.0f, float height = 2.0f, UINT slices = 20);

		// 创建只有圆锥体侧面网格数据，slices越大，精度越高。
		
		std::shared_ptr<Mesh> CreateConeNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20);
	}
}