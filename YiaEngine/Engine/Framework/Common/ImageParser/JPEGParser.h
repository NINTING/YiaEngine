#pragma once


#include"Image.h"
#include"ImageParser.h"
#include"Buffer.h"
#include"Common/ColorSpace.h"
#include"GeomMath.h"
#include"Math/operation/DCT8X8.h"
namespace YiaEngine
{
	namespace {


#pragma pack(push,1)
		struct ThumbnailFormatUnit {
			uint8_t     data;
		};
		struct Thumbnail_RGB
		{
			uint8_t     Xthumbnail;
			uint8_t     Ythumbnail;
			ThumbnailFormatUnit* ThumbnailDatas[3];
		};
		struct Thumbnail_PALETTE
		{
			uint8_t     Xthumbnail;
			uint8_t     Ythumbnail;
			uint8_t		palette[768];
			ThumbnailFormatUnit* ThumbnailDatas;
		};

		struct SOI
		{
			uint16_t SOI;//文件头到位图数据的偏移

		};
		struct JFIF_Segment
		{
			uint16_t marker;
			uint16_t length;
			
		};
		struct APPO : JFIF_Segment
		{
			char     Identifier[5];
			uint8_t     MajorVersion;
			uint8_t     MinorVersion;
			uint8_t     DensityUnits;
			uint16_t    Xdensity;
			uint16_t    Ydensity;
			uint8_t     Xthumbnail;
			uint8_t     Ythumbnail;
		};
		
		struct JFIF_EX_Segment
		{
			uint16_t marker;
			uint16_t length;
			char     Identifier[5];
			char	 Thumbnailformat;
		};
		Matrix<float, 8, 8> DCT8X8(const Matrix<int, 8, 8>& in_mat)
		{
			Matrix<float, 8, 8> out_mat;
			ispc::DCT8X8(in_mat, out_mat);
			return out_mat;
		}
		Matrix<int, 8, 8> IDCT8X8(const Matrix<int, 8, 8>& in_mat)
		{
			Matrix<int, 8, 8> out_mat;
			ispc::IDCT8X8(in_mat, out_mat);
			return out_mat;
		}
	}

#pragma pack(pop)

	uint32_t SmallEnd2BigEnd32(uint32_t in)
	{
		return (in & 0xff000000) >> 24 |
			(in & 0x00ff0000) >> 8 |
			(in & 0x0000ff00) << 8 |
			(in & 0x000000ff) << 24;
	}
	uint32_t SmallEnd2BigEnd16(uint16_t in)
	{
		return (in & 0xff00) >> 8 |
			(in & 0x00ff) << 8;
	}
	//
	//JPEG解析 https://blog.csdn.net/u012877472/article/details/50272771
	class JPEGParser:public ImageParser
	{
	public:
		virtual Image Parser(const Buffer& buffer)
		{
			
			SOI* pFileHead = reinterpret_cast<SOI*>(buffer.m_pdata);

			const uint8_t* data_ptr = buffer.m_pdata;
			const uint8_t* data_end = buffer.m_pdata + buffer.m_size;
			data_ptr += sizeof(SOI);
			while (data_ptr < data_end)
			{
				const JFIF_Segment* segment = reinterpret_cast<const JFIF_Segment*>(data_ptr);
				switch (SmallEnd2BigEnd16(segment->marker))
				{
				case 0xFFE0:
					printf("0xFFE0 段\n 长度 = %d\n", SmallEnd2BigEnd16(segment->length));
					data_ptr += SmallEnd2BigEnd16(segment->length)+2;
					break;
				case 0XFFDB:
					printf("0xFFEB 段\n 长度 = %d\n", SmallEnd2BigEnd16(segment->length));
					data_ptr += SmallEnd2BigEnd16(segment->length)+2;
					break;
				case 0xFFC0:
					printf("0xFFCO 段\n 长度 = %d\n", SmallEnd2BigEnd16(segment->length));
					data_ptr += SmallEnd2BigEnd16(segment->length)+2;
					break;
				case 0XFFC4:
					printf("0xFFC4 段\n 长度 = %d\n", SmallEnd2BigEnd16(segment->length));
					data_ptr += SmallEnd2BigEnd16(segment->length)+2;
					break;
				case 0xFFDA:
					while(1)
					{
						data_ptr += 1;
						if (*data_ptr == 0xff && *(data_ptr + 1) == 0xD9)
						{
							data_ptr += 1;
							break;
						}
					};
					break;
				default:
					printf("0xFFEn 段\n 长度 = %d\n", SmallEnd2BigEnd16(segment->length));
					data_ptr += SmallEnd2BigEnd16(segment->length) + 2;
					break;
				}

			}

			Image imageData;
			//if (pFileHead->Sinature == 0x4D42)
			//{
			//	printf("bmp图片长宽(w:%d,h:%d)\n", pDataHead->Width, pDataHead->Height);
			//	printf("bmp颜色位数%d\n", pDataHead->BitCount);
			//	printf("bmp图片大小位数%d\n", pDataHead->ImageSize);
			//	imageData.bitCount = 32;
			//	imageData.width = pDataHead->Width;
			//	imageData.height = pDataHead->Height;
			//	imageData.pitch = (((imageData.bitCount >> 3) * imageData.width) + 3) & ~3;	//对齐
			//	imageData.data_size = imageData.pitch * imageData.height;
			//	imageData.pData = reinterpret_cast<R8G8B8A8U*>(MemoryManager::Instance()->Allocate(imageData.data_size));
			//	uint8_t* sourceData = buffer.m_pdata + pFileHead->Offset;
			//	uint32_t sourcePitch = (((pDataHead->BitCount >> 3) * imageData.width) + 3) & ~3;


			//	for (int y = imageData.height - 1; y >= 0; y--)
			//	{
			//		for (int x = 0; x < imageData.width; x++)
			//		{
			//			

			//		#if defined(_DEBUG)
			//			uint8_t* p = (sourceData + y * sourcePitch + x * (pDataHead->BitCount >> 3));
			//			uint8_t* h = sourceData;
			//			uint8_t* c= sourceData +y * sourcePitch -1;
			//			auto debugColor = *(reinterpret_cast<R8G8B8U*>(sourceData + 200));

			//		#endif // DEBUG

			//			
			//			(imageData.pData + x + ( imageData.height - y - 1)*imageData.width)->bgr =
			//				*(reinterpret_cast<R8G8B8U*>(sourceData + y * sourcePitch + x * (pDataHead->BitCount >> 3)));

			//			
			//		}
			//	}
			//}
			return imageData;
		}
	};
}
