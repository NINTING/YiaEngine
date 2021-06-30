#pragma once


#include"Image.h"
#include"ImageParser.h"
#include"Buffer.h"
namespace YiaEngine
{
#pragma pack(push,1)
	static struct ThumbnailFormatUnit {
		uint8_t     data;
	}
	static struct Thumbnail_RGB
	{
		uint8_t     Xthumbnail;
		uint8_t     Ythumbnail;
		ThumbnailFormatUnit* ThumbnailDatas[3];
	};
	static struct Thumbnail_PALETTE
	{
		uint8_t     Xthumbnail;
		uint8_t     Ythumbnail;
		uint8_t		palette[768];
		ThumbnailFormatUnit* ThumbnailDatas;
	};
	
	static struct JPEG_Start
	{
		uint16_t SOI;//文件头到位图数据的偏移
			
	};
	static struct JFIF_Segment
	{
		uint16_t marker;
		uint16_t length;
		char     Identifier[5];
		uint8_t     MajorVersion;
		uint8_t     MinorVersion;
		uint8_t     DensityUnits;
		uint16_t    Xdensity;
		uint16_t    Ydensity;
		Thumbnail_RGB datas;
	};
	
	
	static struct JFIF_EX_Segment
	{
		uint16_t marker;
		uint16_t length;
		char     Identifier[5];
		char	 Thumbnail format;
	};


#define BITMAP_FILEHEAD_SIZE 14
	struct  BitMap_DataHead
	{
		uint32_t HeaderSize;
		uint32_t Width;
		uint32_t Height;
		uint16_t Plane;
		uint16_t BitCount;
		uint32_t Compression;
		uint32_t ImageSize;
		int32_t PelsPerMeterX;
		int32_t PelsPerMeterY;
		uint32_t ClrUsed;
		uint32_t ClrImportant;
	};
#pragma pack(pop)
	//
	//BMP解析 https://blog.csdn.net/u012877472/article/details/50272771
	class BmpParser:public ImageParser
	{
	public:
		virtual Image Parser(const Buffer& buffer)
		{
			BitMap_FileHead* pFileHead = reinterpret_cast<BitMap_FileHead*>(buffer.m_pdata);
			BitMap_DataHead* pDataHead = reinterpret_cast<BitMap_DataHead*>(buffer.m_pdata + BITMAP_FILEHEAD_SIZE);
			Image imageData;
			if (pFileHead->Sinature == 0x4D42)
			{
				printf("bmp图片长宽(w:%d,h:%d)\n", pDataHead->Width, pDataHead->Height);
				printf("bmp颜色位数%d\n", pDataHead->BitCount);
				printf("bmp图片大小位数%d\n", pDataHead->ImageSize);
				imageData.bitCount = 32;
				imageData.width = pDataHead->Width;
				imageData.height = pDataHead->Height;
				imageData.pitch = (((imageData.bitCount >> 3) * imageData.width) + 3) & ~3;	//对齐
				imageData.data_size = imageData.pitch * imageData.height;
				imageData.pData = reinterpret_cast<R8G8B8A8U*>(MemoryManager::Instance()->Allocate(imageData.data_size));
				uint8_t* sourceData = buffer.m_pdata + pFileHead->Offset;
				uint32_t sourcePitch = (((pDataHead->BitCount >> 3) * imageData.width) + 3) & ~3;


				for (int y = imageData.height - 1; y >= 0; y--)
				{
					for (int x = 0; x < imageData.width; x++)
					{
						

					#if defined(_DEBUG)
						uint8_t* p = (sourceData + y * sourcePitch + x * (pDataHead->BitCount >> 3));
						uint8_t* h = sourceData;
						uint8_t* c= sourceData +y * sourcePitch -1;
						auto debugColor = *(reinterpret_cast<R8G8B8U*>(sourceData + 200));

					#endif // DEBUG

						
						(imageData.pData + x + ( imageData.height - y - 1)*imageData.width)->bgr =
							*(reinterpret_cast<R8G8B8U*>(sourceData + y * sourcePitch + x * (pDataHead->BitCount >> 3)));

						
					}
				}
			}
			return imageData;
		}
	};
}
