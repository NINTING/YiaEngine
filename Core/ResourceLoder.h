#pragma once
#include"Graphic.h"
#include"Shader.h"

#include"ThirdParty/freeImage/FreeImage.h"
#include "Renderer/Image.h"
#include "OS/ThreadSystem.h"
namespace YiaEngine
{
	namespace Graphic
	{
		

		struct ShaderStageLoadDesc
		{
			const wchar_t* FileName;
			ShaderStage stage = Shader_Stage_None;
			const wchar_t* target;
		};
		struct ShaderLoadDesc
		{
			ShaderStageLoadDesc StageLoadDesc[Shader_Stage_Count];
		};
		void LoadShader(const ShaderLoadDesc& desc,Shader& shader);
		void CompilerShaderFromDXC(const wchar_t* path, const wchar_t* entry, const wchar_t* target, ID3D12ShaderReflection** ppOutReflect, ShaderBinaryData* pOut);




		inline void GetFullFilePath(const char* filename, char* fullPath)
		{
			char AssetPath[100] = "D:\\Yia\\YiaEngine\\Asset\\";

			strcpy(fullPath, AssetPath);
			strcat(fullPath, filename);
		}

		inline bool LoadImageFile(const char* filename, Graphic::ImageData* outImageData)
		{
			char fullPath[100];
			GetFullFilePath(filename, fullPath);

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			//pointer to the image, once loaded
			FIBITMAP* dib(0);
			//pointer to the image data
			BYTE* bits(0);

			fif = FreeImage_GetFileType(fullPath, 0);
			//if still unknown, try to guess the file format from the file extension
			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(fullPath);
			//if still unkown, return failure
			if (fif == FIF_UNKNOWN)
				return false;

			//check that the plugin has reading capabilities and load the file
			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, fullPath);
			//if the image failed to load, return failure


			if (!dib)
				return false;

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

			auto dibsize = FreeImage_GetDIBSize(dib);

			if ((bits == 0) || (width == 0) || (height == 0))
				return false;


			outImageData->Width = width;
			outImageData->Height = height;
			//outImageData->bitCount = info->bmiHeader.biBitCount;

			//outImageData->Pitch = pitch;
			outImageData->pData = nullptr;
			uint32_t image_size = 0;
			uint8_t* pdata = nullptr;
			switch (color_type)
			{
			case FIC_MINISWHITE:
				break;
			case FIC_MINISBLACK:
				break;
			case FIC_RGB:
				outImageData->Pitch = (((32 >> 3) * width) + 3) & ~3;
				image_size = outImageData->Pitch * height;
				outImageData->Size = image_size;
				outImageData->pData = std::shared_ptr<uint8_t>(new uint8_t[image_size]); // reinterpret_cast<R8G8B8A8U*>(Allocate(image_size));
				pdata = outImageData->pData.get();
				for (int y = height - 1; y >= 0; y--)
				{
					for (int x = 0; x < width; x++)
					{
						RGBQUAD quad;
						if (FreeImage_GetPixelColor(dib, x, y, &quad))
						{
							uint8_t color[4] = {  quad.rgbRed ,quad.rgbGreen ,quad.rgbBlue,255 };
							memcpy_s(pdata + x * 4 + (height - y - 1) * width*4, 4*sizeof(uint8_t), &color, 4 * sizeof(uint8_t));
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

			return true;



		}
		struct RequestDesc
		{

		};

		struct  ResourceLoder
		{
			CommandManager* pCommandManager;
			std::vector<RequestDesc> requestQueue;
			ThreadSystem::ThreadDesc threadDesc;

			ThreadSystem::ThreadHandle thread;
			bool isRun;
		};


		void InitGpuResourceLoder();
		void UpdateGpuResource(const  RequestDesc& desc);
	}
}