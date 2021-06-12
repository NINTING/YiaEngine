

#include <iostream>
#include <fstream>


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "AssetLoder.h"

namespace YiaEngine
{
	std::string AssetLoder::GetFullFilePath(const char*name)
	{
		std::string full_path;
		FILE* fp = nullptr;
		std::string upPath;
		std::string fullPath;
		for (int i = 0; i < 10; i++)
		{
			auto it = searchPath.begin();
			bool loop = true;
			while (loop)
			{
				fullPath.assign(upPath);
				if (it != searchPath.end())
				{
					fullPath.append(*it);
					fullPath.append("/Asset/");
					it++;
				}
				else
				{
					fullPath.append("Asset/"); loop = false;
				}
				fullPath.append(name);
#ifdef DEBUG
				fprintf(stderr, "Trying to open %s\n", fullPath.c_str());
#endif
				fp = fopen(fullPath.c_str(), "rb");
				
				if (fp)
					return fullPath;
			}
			upPath.append("../");
		}
	}

	AssetLoder::AssetFilePtr AssetLoder::OpenFile(const char* name, FILE_OPEN_MODE mode)
	{
		FILE* fp = nullptr;
		std::string upPath;
		std::string fullPath;
		for (int i = 0; i < 10; i++)
		{

			auto it = searchPath.begin();
			bool loop = true;
			while (loop)
			{
				fullPath.assign(upPath);
				if (it != searchPath.end())
				{
					fullPath.append(*it);
					fullPath.append("/Asset/");
					it++;
				}
				else
				{
					fullPath.append("Asset/"); loop = false;
				}
				fullPath.append(name);
#ifdef DEBUG
				fprintf(stderr, "Trying to open %s\n", fullPath.c_str());
#endif
				switch (mode)
				{
				case FILE_OPEN_MODE::BINARY:
					fp = fopen(fullPath.c_str(), "rb");
					break;
				case FILE_OPEN_MODE::TEXT:
					fp = fopen(fullPath.c_str(), "r");
					break;
				default:
					break;
				}
				if (fp)
					return AssetFilePtr(fp);
			}

			upPath.append("../");
		}
		return nullptr;
	}

	void AssetLoder::AddSearchPath(const char* path)
	{
		for (auto it = searchPath.begin(); it != searchPath.end(); it++)
		{
			if (!(*it).compare(path))
				return;
		}
		searchPath.push_back(path);

	}

	void AssetLoder::RemoveSearchPath(const char* path)
	{
		auto it = searchPath.begin();
		for (; it != searchPath.end(); it++)
		{
			if (!(*it).compare(path))
			{
				break;
			}
		}
		searchPath.erase(it);

	}
	Buffer YiaEngine::AssetLoder::OpenAndReadText(const char* name)
	{
		AssetFilePtr fp = OpenFile(name, FILE_OPEN_MODE::TEXT);
		Buffer* pbuffer = nullptr;
		if (fp) {
			size_t len = GetSize(fp);
			pbuffer = new Buffer(len);
			fread(pbuffer->m_pdata, len, 1, reinterpret_cast<FILE*>(fp));
			pbuffer->m_pdata[len] = '\0';
			CloseFile(fp);

		}
		else
			pbuffer = new Buffer();

		return std::move(*pbuffer);

	}
	Buffer YiaEngine::AssetLoder::OpenAndReadBinary(const char* name)
	{
		AssetFilePtr fp = OpenFile(name, FILE_OPEN_MODE::BINARY);
		Buffer* pbuffer = nullptr;
		if (fp) {
			size_t len = GetSize(fp);
			pbuffer = new Buffer(len);
			size_t s = fread(pbuffer->m_pdata, len, 1, reinterpret_cast<FILE*>(fp));

			CloseFile(fp);

		}
		else
			pbuffer = new Buffer();

		return std::move(*pbuffer);

	}
	std::shared_ptr<MeshObject> AssetLoder::LoadMesh(const char* name)
	{
		Assimp::Importer importer;

		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll
		// probably to request more postprocessing than we do in this example.
		auto full_name = GetFullFilePath(name);
		const aiScene* scene = importer.ReadFile(full_name,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		
		// If the import failed, report it
		if (!scene) {
			printf("%s", (importer.GetErrorString()));
			return nullptr;
		}
		
		std::shared_ptr<MeshObject> mesh_object = std::make_shared<MeshObject>();
		for (int i = 0; i < scene->mNumMaterials; i++)
		{
			scene->mMaterials[i]->Get();
		}
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			switch (scene->mMeshes[i]->mPrimitiveTypes)
			{
			case aiPrimitiveType::aiPrimitiveType_POINT:
				mesh_object->set_primitive(MeshPrimitive::kPoint);
				break;
			case aiPrimitiveType::aiPrimitiveType_TRIANGLE:
				mesh_object->set_primitive(MeshPrimitive::kTriangle);
				break;
			default:
				break;
			}
			
			//顶点属性
			
			if (scene->mMeshes[i]->HasPositions())
			{
				//顶点坐标
				uint32_t vertices_num = scene->mMeshes[i]->mNumVertices;
				VertexArray positions(VertexAttribute::kPosition, DataType::kFloat3,
					scene->mMeshes[i]->mVertices, vertices_num);
				mesh_object->add_vertex_array(positions);
				//法线
				if (scene->mMeshes[i]->HasNormals())
				{
					VertexArray normals(VertexAttribute::kNormal, DataType::kFloat3,
						scene->mMeshes[i]->mNormals, vertices_num);
					mesh_object->add_vertex_array(normals);

				}
				//切线
				if (scene->mMeshes[i]->HasTangentsAndBitangents())
				{
					VertexArray tangent(VertexAttribute::kTangent, DataType::kFloat3,
						scene->mMeshes[i]->mTangents, vertices_num);
					VertexArray bit_tangent(VertexAttribute::kBitangent, DataType::kFloat3,
						scene->mMeshes[i]->mBitangents, vertices_num);
					mesh_object->add_vertex_array(tangent);

					mesh_object->add_vertex_array(bit_tangent);
					
				}
				//纹理坐标
				for (int j = 0; j < scene->mMeshes[i]->GetNumUVChannels(); j++)
				{
					if (scene->mMeshes[i]->HasTextureCoords(j))
					{
						VertexArray texcoord(VertexAttribute::kTexcoord, DataType::kFloat3,
							scene->mMeshes[i]->mTextureCoords[i], vertices_num);
						mesh_object->add_vertex_array(texcoord);

					}
				}
				//顶点色
				for (int j = 0; j < scene->mMeshes[i]->GetNumColorChannels(); j++)
				{
					if (scene->mMeshes[i]->HasTextureCoords(j))
					{
						VertexArray color(VertexAttribute::kColor, DataType::kFloat4,
							scene->mMeshes[i]->mColors[i], vertices_num);
						mesh_object->add_vertex_array(color);

					}
				}
			}
			//索引
			if(scene->mMeshes[i]->HasFaces())
			{
				for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					IndexArray index_array(DataType::kUint32,
						scene->mMeshes[i]->mFaces[j].mIndices,
						scene->mMeshes[i]->mFaces[j].mNumIndices);
					mesh_object->add_index_array(index_array);
				}
			}
		}
		return mesh_object;
	}
	Buffer* YiaEngine::AssetLoder::ReadText(AssetFilePtr fp)
	{
		size_t len = GetSize(fp);
		Buffer* buffer = new Buffer(len);
		fread(buffer->m_pdata, len, 1, reinterpret_cast<FILE*>(fp));
		buffer->m_pdata[len] = '\0';
		CloseFile(fp);
		return buffer;
	}

	int YiaEngine::AssetLoder::GetSize(AssetFilePtr afp)
	{
		FILE* fp = static_cast<FILE*>(afp);
		int pos = ftell(fp);
		fseek(fp, 0, SEEK_END);
		size_t length = ftell(fp);
		fseek(fp, pos, SEEK_SET);
		return length;
	}

	void YiaEngine::AssetLoder::CloseFile(AssetFilePtr fp)
	{
		fclose((FILE*)fp);
		fp = nullptr;
	}
}
