#pragma once
#include "GpuResource.h"
#include"DescriptorHeap.h"
#include "Common/Color.h"
namespace YiaEngine
{
    namespace Graphic
    {
        enum class ResourceDimension
        {
            UNKNOWN = 0,
			BUFFER = 1,
			TEXTURE1D = 2,
			TEXTURE2D = 3,
			TEXTURE3D = 4
        };

        enum TextureFlag
        {
			NONE = 0,
			ALLOW_RENDER_TARGET = 0x1,
			ALLOW_DEPTH_STENCIL = 0x2,
			ALLOW_UNORDERED_ACCESS = 0x4,
        };
        enum class TextureLayout
        {
            UNKNOWN = 0,
			ROW_MAJOR = 1,
			UNDEFINED_SWIZZLE_64KB = 2,
			STANDARD_SWIZZLE_64KB = 3
        };
        

        struct DepthStencilValue
        {
			float Depth;
			UINT8 Stencil;
        };
		
        struct ClearState
        {
            ClearState() {
                ClearColor = 0;
            }
            union 
            {
				Color ClearColor;
                DepthStencilValue DepthStencilClearValue;
            };
            static ClearState DefaultState()
            {
                ClearState ret;
                ret.ClearColor = DefaultColor;
                ret.DepthStencilClearValue.Depth = 1;
                ret.DepthStencilClearValue.Stencil= 0x00;
                return ret;
            }
        };
		struct TextureDescribe
		{
            static TextureDescribe RenderTarget2Describe(
                UINT width,
                UINT height,
                DXGI_FORMAT format,
                TextureLayout layout = TextureLayout::UNKNOWN,
                ClearState clearvalue = ClearState::DefaultState(),
                UINT16 mipLevels = 1,
                UINT64 alignment = 0,
                UINT msaaCount = 1,
                UINT msaaQuality = 0) {
                TextureDescribe ret;

                ret.Dimension = ResourceDimension::TEXTURE2D;
                ret.Width = width;
                ret.Height = height;
                ret.DepthOrArraySize = 1;
                ret.MipLevels = mipLevels;
                ret.Format = format;
                ret.MsaaCount = msaaCount;
                ret.MsaaQuality = msaaQuality;
                ret.Layout = layout;
                ret.Flags = ALLOW_RENDER_TARGET;
            }

			ResourceDimension Dimension;
			
			UINT64 Width;
			UINT Height;
			UINT16 DepthOrArraySize;
			UINT16 MipLevels;
			DXGI_FORMAT Format;
			UINT MsaaCount;
			UINT MsaaQuality;
			TextureLayout Layout;
			TextureFlag Flags;
            ClearState ClearValue;
        };

        class GpuTexture : public GpuResource
        {
        public:
         //   const DescriptorHandle& SrvHandle()const;
            ~GpuTexture() { Destroy(); }
           
            DXGI_FORMAT GetFormat() { return (DXGI_FORMAT)describe_.Format; };
            virtual DescriptorHandle SrvCpuHandle()const = 0;
            Math::Vec2f Size() { return Math::Vec2f{ describe_.Width,describe_.Height }; };
            TextureDescribe GetDescribe() { return describe_; }
        protected:
            TextureDescribe DescribeTex2D(UINT widht, UINT Height, DXGI_FORMAT format, UINT depthOrArraySize = 1, UINT mips = 0, UINT flag = D3D12_RESOURCE_FLAG_NONE);
            void CreateTextureResource(const wchar_t* Name, const ClearState value = ClearState::DefaultState());

            DXGI_FORMAT GetStencilFormat(DXGI_FORMAT format);
        protected:
            TextureDescribe describe_;
      //      DescriptorHandle srvHandle_;
        };

    }
}