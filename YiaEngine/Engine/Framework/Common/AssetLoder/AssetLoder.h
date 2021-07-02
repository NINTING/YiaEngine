#pragma once
#ifndef COMMON_ASSET_LODER
#define COMMON_ASSET_LODER


/****************************
AssetLoder
��Դ����ģ��
1.Ϊ������ƽ̨�ļ���ȡ�Ĳ�����
2.�첽��ȡ��Դ


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
	//�����ļ���ȡ

	enum class FILE_OPEN_MODE
	{
		TEXT,
		BINARY
	};

	class AssetLoder
	{
	public:
		void Init();
		typedef void* AssetFilePtr; //��ͬ�ļ��в�ͬ�ĸ�ʽ


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