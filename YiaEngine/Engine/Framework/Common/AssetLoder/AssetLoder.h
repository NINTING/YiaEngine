#pragma once
/****************************
AssetLoder
资源加载模块
1.为了消除平台文件读取的差异性
2.异步读取资源


****************************/

#include<vector>
#include<string>
#include"MemoryManager.h"
#include"Buffer.h"
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
		AssetFilePtr OpenFile(const char* name, FILE_OPEN_MODE mode);

		void AddSearchPath(const char* path);
		void RemoveSearchPath(const char* path);
		
		Buffer OpenAndReadText(const char* name);
		Buffer OpenAndReadBinary(const char* name);
		//void ReadFile(AssetFilePtr fp);
		Buffer* ReadText(AssetFilePtr fp);
		int GetSize(AssetFilePtr);

		void CloseFile(AssetFilePtr fp);

	private:
		std::vector<std::string> searchPath;

	};
}