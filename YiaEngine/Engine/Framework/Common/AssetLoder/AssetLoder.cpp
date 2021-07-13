

#include <iostream>
#include <fstream>


#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "freeImage/FreeImage.h"

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
	std::shared_ptr<Image> YiaEngine::AssetLoder::LoadImageFile(const char* filename)
	{
	 	std::string full_name = GetFullFilePath(filename);
		
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP* dib(0);
		//pointer to the image data
		BYTE* bits(0);

		fif = FreeImage_GetFileType(full_name.c_str(), 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(full_name.c_str());
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return nullptr;
		
		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, full_name.c_str());
		//if the image failed to load, return failure
	
	
		if (!dib)
			return nullptr;

		const char* format = FreeImage_GetFIFDescription(fif);

		printf("%s\n", format);
	
		//get the image width and height
		uint32_t width = FreeImage_GetWidth(dib);
		uint32_t height = FreeImage_GetHeight(dib);
		uint32_t pitch = FreeImage_GetPitch(dib);
		uint32_t mem_size = FreeImage_GetMemorySize(dib);
		uint32_t bpp_size = FreeImage_GetBPP(dib);
		auto info = FreeImage_GetInfo(dib);
		bits = FreeImage_GetBits(dib);
		uint32_t data_size = pitch * height;
#ifdef DEBUG
		printf("width0:%d\n", info->bmiHeader.biWidth);
		printf("width:%d\n", width);
		printf("height:%d\n", height);
		printf("pitch:%d\n", pitch);
		printf("mem_size:%d\n", mem_size);
		printf("data_size:%d\n", data_size);
		printf("size:%lld\n", info->bmiHeader.biSize);
		printf("image size:%d\n", info->bmiHeader.biSizeImage);

		printf("bpp_size:%d\n", bpp_size);
		
		printf("image start%p\n", info->bmiColors);
		printf("image start1%p\n", bits);
		printf("head size %p\n", sizeof(info->bmiHeader));
#endif // DEBUG
		auto type = FreeImage_GetImageType(dib);
		auto color_type = FreeImage_GetColorType(dib);
	
		auto dibsize =  FreeImage_GetDIBSize(dib);

		if ((bits == 0) || (width == 0) || (height == 0))
			return nullptr;
		
		std::shared_ptr<Image> ptr_img = std::shared_ptr<Image>(new Image);

		
		
		
		ptr_img->width = width;
		ptr_img->height = height;
		ptr_img->bitCount = info->bmiHeader.biBitCount;
		ptr_img->data_size = data_size;
		ptr_img->pitch = pitch;
		ptr_img->pData = nullptr;
		uint32_t image_size = 0;
		R8G8B8A8U* pdata = nullptr;
		switch (color_type)
		{
		case FIC_MINISWHITE:
			break;
		case FIC_MINISBLACK:
			break;
		case FIC_RGB:
			pitch = (((32 >> 3) * width) + 3) & ~3;
			image_size = pitch  * height;
			ptr_img->pitch = pitch;
			ptr_img->data_size = image_size;
			ptr_img->pData = reinterpret_cast<R8G8B8A8U*>(MemoryManager::Instance()->Allocate(image_size));
			pdata = reinterpret_cast<R8G8B8A8U*>(ptr_img->pData);
			for (int y = height - 1; y>=0; y--)
			{
				for (int x = 0; x < width; x++)
				{
					RGBQUAD quad;
					if (FreeImage_GetPixelColor(dib, x, y, &quad))
					{
						(pdata + x + (height - y - 1)  * width)->bgr = *(reinterpret_cast<R8G8B8U*>(&quad));
					}
					/*(pdata + x + y * width)->rbg =
						*(reinterpret_cast<R8G8B8U*>(bits + y *pitch + x * (info->bmiHeader.biBitCount >> 3)));*/
				}
			}
			break;
		case FIC_PALETTE:
			break;
		case FIC_RGBALPHA:
		break;
		case FIC_CMYK:
			break;
		default:
			break;
		}

		return ptr_img;
		
		

	}
	//std::shared_ptr<Image> AssetLoder::LoadImage()
	//{

	//}

	//Scene::VariantParam GetMaterialParameter()
	//{

	//}

	std::shared_ptr<Scene::MaterialObject> CreatePBRMaterial( aiMaterial const* aiMat)
	{
		std::shared_ptr<Scene::PBRMaterial> mat_obj = std::make_shared<Scene::PBRMaterial>();
		//aiString mat_name;
		//aiMat->Get(AI_MATKEY_NAME, mat_name);
		//mat_obj->SetName(mat_name.C_Str());
		//
		//Color color;
		//aiString texture_path;
		////mat_obj
		//aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		//
		//aiMat->GetTextureCount(aiTextureType_DIFFUSE);
		//auto result = aiMat->GetTexture(aiTextureType_AMBIENT,0, &texture_path);
		//if (result == aiReturn_SUCCESS)
		//{
		//	
		//}
		//mat_obj->SetParam("Diffuse", color);

		//result = aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
		return mat_obj;
	}
	
	std::unique_ptr<Scene::GeometryNode> AssetLoder::LoadModel(const char* name)
	{
		using namespace Scene;
		Assimp::Importer importer;

		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll
		// probably to request more postprocessing than we do in this example.
		auto full_name = GetFullFilePath(name);
		const aiScene* scene = importer.ReadFile(full_name,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_FlipUVs);
		
		// If the import failed, report it
		if (!scene) {
			printf("%s", (importer.GetErrorString()));
			return nullptr;
		}
		
		
	
		std::unique_ptr<GeometryNode>geo_node = std::unique_ptr<GeometryNode>(new GeometryNode());
		std::shared_ptr<Scene::GeometryObject>geo_obj = std::shared_ptr<Scene::GeometryObject>(new GeometryObject());
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			std::shared_ptr<Scene::MeshObject> mesh_object = std::shared_ptr<Scene::MeshObject>(new Scene::MeshObject);
			auto mesh = scene->mMeshes[i];
			switch (mesh->mPrimitiveTypes)
			{
			case aiPrimitiveType::aiPrimitiveType_POINT:
				mesh_object->set_primitive(Scene::MeshPrimitive::kPoint);
				break;
			case aiPrimitiveType::aiPrimitiveType_LINE:
				mesh_object->set_primitive(Scene::MeshPrimitive::kLine);
				break;
			case aiPrimitiveType::aiPrimitiveType_TRIANGLE:
				mesh_object->set_primitive(Scene::MeshPrimitive::kTriangle);
				break;
			default:
				break;
			}
			auto material  = scene->mMaterials[mesh->mMaterialIndex];
			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
			aiUVTransform ut,vt;
			material->Get(AI_MATKEY_MAPPINGMODE_V_DIFFUSE(0),vt);
			material->Get(AI_MATKEY_MAPPINGMODE_U_DIFFUSE(0),ut);

			aiUVTransform mappingt,axit,flagt;
			material->Get(AI_MATKEY_MAPPING_DIFFUSE(0), mappingt);
			material->Get(AI_MATKEY_TEXMAP_AXIS_DIFFUSE(0), axit);
			material->Get(AI_MATKEY_TEXFLAGS_DIFFUSE(0), flagt);
			
			geo_node->AddMatrial(CreatePBRMaterial(material));
			//顶点属性
			if (scene->mMeshes[i]->HasPositions())
			{
				//顶点坐标
				uint32_t vertices_num = scene->mMeshes[i]->mNumVertices;
				Vec3f* vertices = new Vec3f[vertices_num];
				memcpy(vertices, scene->mMeshes[i]->mVertices, vertices_num*sizeof(float)*3);
				VertexArray positions(VertexAttribute::kPosition, DataType::kFloat_3,
					vertices, vertices_num);
				mesh_object->add_vertex_array(positions);
				//法线
				if (scene->mMeshes[i]->HasNormals())
				{
					VertexArray normals(VertexAttribute::kNormal, DataType::kFloat_3,
						scene->mMeshes[i]->mNormals, vertices_num);
					mesh_object->add_vertex_array(normals);

				}
				//切线
				if (scene->mMeshes[i]->HasTangentsAndBitangents())
				{
					VertexArray tangent(VertexAttribute::kTangent, DataType::kFloat_3,
						scene->mMeshes[i]->mTangents, vertices_num);
					VertexArray bit_tangent(VertexAttribute::kBitangent, DataType::kFloat_3,
						scene->mMeshes[i]->mBitangents, vertices_num);
					mesh_object->add_vertex_array(tangent);

					mesh_object->add_vertex_array(bit_tangent);
					
				}
				//纹理坐标
				for (int j = 0; j < scene->mMeshes[i]->GetNumUVChannels(); j++)
				{
					if (scene->mMeshes[i]->HasTextureCoords(j))
					{
						Vec3f* uvs = new Vec3f[vertices_num];
						memcpy(uvs, scene->mMeshes[i]->mTextureCoords[i], vertices_num * sizeof(float) * 3);

						VertexArray texcoord(VertexAttribute::kTexcoord, DataType::kFloat_3,
							uvs, vertices_num);
						mesh_object->add_vertex_array(texcoord);
					}
				}
				//顶点色
				for (int j = 0; j < scene->mMeshes[i]->GetNumColorChannels(); j++)
				{
					if (scene->mMeshes[i]->HasTextureCoords(j))
					{
						VertexArray color(VertexAttribute::kColor, DataType::kFloat_4,
							scene->mMeshes[i]->mColors[i], vertices_num);
						mesh_object->add_vertex_array(color);

					}
				}
			}
			//索引
			if(scene->mMeshes[i]->HasFaces())
			{
				uint32_t* indeces = new uint32_t[scene->mMeshes[i]->mNumFaces * 3];
				for (uint32_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					*(indeces + j * 3 + 0) = scene->mMeshes[i]->mFaces[j].mIndices[0];
					*(indeces + j * 3 + 1) = scene->mMeshes[i]->mFaces[j].mIndices[1];
					*(indeces + j * 3 + 2) = scene->mMeshes[i]->mFaces[j].mIndices[2];
				}
				IndexArray index_array(indeces,scene->mMeshes[i]->mNumFaces * 3, DataType::kUint32_1);
				mesh_object->add_index_array(index_array);
			}
//			mesh_object->Serialize();
			geo_obj->AddMesh(mesh_object);
		}
		//delete scene;
		geo_node->set_object_ref(geo_obj);
		
		return geo_node;
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
