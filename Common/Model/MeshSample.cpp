#include "pch.h"
#include "MeshSample.h"

namespace YiaEngine
{
	namespace MeshSample
	{
		std::map<std::string, std::shared_ptr<Mesh>>meshMap;
		std::shared_ptr<Mesh> CreateBox()
		{
			auto it = meshMap.find("Box");

			if (it != meshMap.end())
			{
				return (*it).second;
			}
			Math::Vec3f BoxVertex[] = {
				//上
				{ -1.0f, 1.f , 1.0f },
				{ 1.0f, 1.f , 1.0f },
				{ 1.0f, 1.f , -1.0f },
				{ -1.0f, 1.f , -1.0f },
				//下
				{ -1.0f, -1.f , 1.0f },
				{ 1.0f, -1.f , 1.0f },
				{ 1.0f, -1.f , -1.0f },
				{ -1.0f, -1.f , -1.0f },
				//左
				{ -1.0f, 1.f , 1.0f },
				{ -1.0f, 1.f , -1.0f },
				{ -1.0f, -1.f , -1.0f },
				{ -1.0f, -1.f , 1.0f },
				//右
				{ 1.0f, 1.f , 1.0f },
				{ 1.0f, 1.f , -1.0f },
				{ 1.0f, -1.f , -1.0f },
				{ 1.0f, -1.f , 1.0f },
				//前
				{ -1.0f, 1.f , -1.0f },
				{ 1.0f, 1.f , -1.0f },
				{ 1.0f, -1.f , -1.0f },
				{ -1.0f, -1.f , -1.0f },
				//后
				{ -1.0f, 1.f ,	1.0f },
				{ 1.0f, 1.f ,	1.0f },
				{ 1.0f, -1.f ,	1.0f },
				{ -1.0f, -1.f , 1.0f },
			};
			Math::Vec3f Boxnormal[] =
			{
				//上
				{ 0.0f, 1.f , 0.0f },
				{ 0.0f, 1.f , 0.0f },
				{ 0.0f, 1.f , 0.0f },
				{ 0.0f, 1.f , 0.0f },
				//下
				{ 0.0f, -1.f , 0.0f },
				{ 0.0f, -1.f , 0.0f },
				{ 0.0f, -1.f , 0.0f },
				{ 0.0f, -1.f , 0.0f },
				//左
				{ -1.0f, 0.f , 0.0f },
				{ -1.0f, 0.f , 0.0f },
				{ -1.0f, 0.f , 0.0f },
				{ -1.0f, 0.f , 0.0f },
				//右
				{ 1.0f, 0.f , 0.0f },
				{ 1.0f, 0.f , 0.0f },
				{ 1.0f, 0.f , 0.0f },
				{ 1.0f, 0.f , 0.0f },
				//前
				{ 0.0f, 0.f , -1.0f },
				{ 0.0f, 0.f , -1.0f },
				{ 0.0f, 0.f , -1.0f },
				{ 0.0f, 0.f , -1.0f },
				//后
				{ 0.0f, 0.f , 1.0f },
				{ 0.0f, 0.f , 1.0f },
				{ 0.0f, 0.f , 1.0f },
				{ 0.0f, 0.f , 1.0f },
			};
			Math::Vec2f Boxuv[] =
			{
				{0,0},
				{1,0},
				{1,1},
				{0,1},

				{0,0},
				{1,0},
				{1,1},
				{0,1},

				{0,0},
				{1,0},
				{1,1},
				{0,1},

				{0,0},
				{1,0},
				{1,1},
				{0,1},

				{0,0},
				{1,0},
				{1,1},
				{0,1},

				{0,0},
				{1,0},
				{1,1},
				{0,1},

			};
			UINT boxIndex[] = {
				0,1,3,1,2,3,
				7,5,4,7,6,5,
				8,9,11,9,10,11,
				15,13,12,15,14,13,
				16,17,19,17,18,19,
				23,21,20,23,22,21 };
			std::shared_ptr<Mesh> ret = std::shared_ptr<Mesh>(new Mesh());

			meshMap["Box"] = ret;


			ret->SetName("Box");
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, 24, BoxVertex));
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, 24, Boxuv));
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kNormal, DataFormate::kFloat_3, 24, Boxnormal));
			ret->AddIndices(sizeof(boxIndex) / sizeof(int), boxIndex);
			ret->CreateMeshGpuBuffer();
			return ret;
		}

		std::shared_ptr<Mesh> CreateSphere(float radius, UINT levels, UINT slices)
		{
			auto it = meshMap.find("Sphere");

			if (it != meshMap.end())
			{
				return (*it).second;
			}
			UINT vertexCount = 2 + (levels - 1) * (slices + 1);
			UINT indexCount = 6 * (levels - 1) * slices;

			float phi = 0.0f, theta = 0.0f;
			float per_phi = Math::PI / levels;
			float per_theta = Math::PI2 / slices;
			float x, y, z;

			std::vector<Math::Vec3f> poslist;
			std::vector<Math::Vec3f> normallist;
			std::vector<Math::Vec2f> uvlist;
			std::vector<UINT> indices;

			poslist.push_back({ 0.0f, radius, 0.0f });
			normallist.push_back({ 0.0f, 1.0f, 0.0f });
			uvlist.push_back({ 0,0 });


			for (UINT i = 1; i < levels; ++i)
			{
				phi = per_phi * i;
				// 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
				
				for (UINT j = 0; j <= slices; ++j)
				{
					theta = per_theta * j;
					x = radius * sinf(phi) * cosf(theta);
					y = radius * cosf(phi);
					z = radius * sinf(phi) * sinf(theta);
					// 计算出局部坐标、法向量、Tangent向量和纹理坐标
					Math::Vec3f pos = { x, y,z };
					poslist.push_back(pos);
					normallist.push_back(pos.normalized());
					uvlist.push_back({ theta / Math::PI2, phi / Math::PI });
				
				}
			}
			poslist.push_back({ 0.0f, -radius, 0.0f });
			normallist.push_back({ 0.0f, -1.0f, 0.0f });
			uvlist.push_back({ 0,1 });

			if (levels > 1)
			{
				for (UINT j = 1; j <= slices; ++j)
				{
					indices.push_back(0);
					indices.push_back(j % (slices + 1) + 1);
					indices.push_back(j); 
				}
			}


			for (UINT i = 1; i < levels - 1; ++i)
			{
				for (UINT j = 1; j <= slices; ++j)
				{
					indices.push_back((i - 1) * (slices + 1) + j);
					indices.push_back((i - 1) * (slices + 1) + j % (slices + 1) + 1);
					indices.push_back(i * (slices + 1) + j % (slices + 1) + 1);
					
					indices.push_back(i * (slices + 1) + j % (slices + 1) + 1);
					indices.push_back(i * (slices + 1) + j);
					indices.push_back((i - 1) * (slices + 1) + j);
				}
			}

			// 逐渐放入索引
			if (levels > 1)
			{
				for (UINT j = 1; j <= slices; ++j)
				{
					indices.push_back((levels - 2) * (slices + 1) + j);
					indices.push_back((levels - 2) * (slices + 1) + j % (slices + 1) + 1);
					indices.push_back((levels - 1) * (slices + 1) + 1);

				}
			}
			std::shared_ptr<Mesh> ret = std::shared_ptr<Mesh>(new Mesh());

			meshMap["Sphere"] = ret;

			ret->SetName("Sphere");
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kPosition, DataFormate::kFloat_3, vertexCount, poslist.data()));
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kTexcoord, DataFormate::kFloat_2, vertexCount, uvlist.data()));
			ret->AddAttribute(CreateVertexAttribute(VertexAttributeEnum::kNormal, DataFormate::kFloat_3, vertexCount, normallist.data()));
			ret->AddIndices(indices.size(), indices.data());
			ret->CreateMeshGpuBuffer();
			return ret;
		}

		std::shared_ptr<Mesh> YiaEngine::MeshSample::CreateCylinder(float radius, float height, UINT slices)
		{
			return std::shared_ptr<Mesh>();
		}
	}
}