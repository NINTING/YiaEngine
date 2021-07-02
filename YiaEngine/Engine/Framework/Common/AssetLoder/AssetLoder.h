#pragma once
#ifndef COMMON_ASSET_LODER
#define COMMON_ASSET_LODER


/****************************
AssetLoder
资源加载模块
1.为了消除平台文件读取的差异性
2.异步读取资源


****************************/

#include<vector>
#include<string>

#include<assimp/material.h>

#include"MemoryManager.h"
#include"Buffer.h"
#include"Scene/SceneObject.h"
#include"Scene/SceneNode.h"

namespace YiaEngine
{

	//---- v1.0 
	//基本文件读取

	enum class FILE_OPEN_MODE
	{
		TEXT,
		BINARY
	};

	class AssetLoder
	{
	public:
		void Init();
		typedef void* AssetFilePtr; //不同文件有不同的格式


		~AssetLoder() {};
		
		std::string GetFullFilePath(const char* name);
		AssetFilePtr OpenFile(const char* name, FILE_OPEN_MODE mode);

		void AddSearchPath(const char* path);
		void RemoveSearchPath(const char* path);
		
		Buffer OpenAndReadText(const char* name);
		Buffer OpenAndReadBinary(const char* name);
		
		std::shared_ptr<Scene::MaterialObject> CreatePBRMaterial(aiMaterial const* aiMat);
		std::unique_ptr<Scene::GeometryNode> LoadModel(const char* name);
		//void ReadFile(AssetFilePtr fp);
		Buffer* ReadText(AssetFilePtr fp);
		
		int GetSize(AssetFilePtr);
		void CloseFile(AssetFilePtr fp);
		
	private:
		std::vector<std::string> searchPath;

	};
}
#endif // !COMMON_ASSET_LODER