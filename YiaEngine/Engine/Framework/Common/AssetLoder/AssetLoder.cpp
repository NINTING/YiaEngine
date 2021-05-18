#include "AssetLoder.h"

#include <iostream>
#include <fstream>

using namespace YiaEngine;


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
Buffer* YiaEngine::AssetLoder::ReadText(AssetFilePtr fp)
{
	size_t len = GetSize(fp);
	Buffer* buffer = new Buffer(len);
	fread(buffer->m_pdata, len, 1,reinterpret_cast<FILE*>(fp));
	buffer->m_pdata[len] = '\0';
	CloseFile(fp);
	return buffer;
}

int YiaEngine::AssetLoder::GetSize(AssetFilePtr afp)
{
	FILE* fp = static_cast<FILE*>(afp);
	int pos =  ftell(fp);
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
