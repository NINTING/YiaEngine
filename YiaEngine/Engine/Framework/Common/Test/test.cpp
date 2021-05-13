
#include"MemoryManager.h"
#include"Buffer.h"
#include"AssetLoder/AssetLoder.h"
#include<memory>
#include"BmpParser.h"
//#include"AssetLoder/AssetLoder.h"

namespace YiaEngine
{
	//MemoryManager* g_pMemoryManager = new MemoryManager();

}
YiaEngine::Buffer AllocateBuffer()
{
	YiaEngine::Buffer* buf = new YiaEngine::Buffer(100);
	printf("%p\n", buf->m_pdata);
	return std::move(*buf);
}

int main()
{
	//YiaEngine::g_pMemoryManager->Initialize();
	//auto buf = AllocateBuffer(); 
	YiaEngine::AssetLoder assetLoder;
	YiaEngine::Buffer f = assetLoder.OpenAndReadBinary("bb.bmp");
	YiaEngine::BmpParser bmppar;
	auto img = bmppar.Parser(f);
	printf("%d\n", sizeof(bmppar));
	//printf("%s\n", f.m_pdata);

//	printf("%p\n", buf.m_pdata);
	return 0;
}