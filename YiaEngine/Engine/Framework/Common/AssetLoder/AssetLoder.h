#pragma once
/****************************
AssetLoder
��Դ����ģ��
1.Ϊ������ƽ̨�ļ���ȡ�Ĳ�����
2.�첽��ȡ��Դ


****************************/

#include<vector>
#include<string>
#include"MemoryManager.h"
#include"Buffer.h"
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