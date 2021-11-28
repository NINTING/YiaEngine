#pragma once

#include"pch.h"
#include "Core/YiaGraphic.h"
#include "FrameResource.h"
namespace YiaEngine
{
	namespace FrameGraph
	{
		
		/// <summary>
		/// ����ִ��ʱ���ݣ��Լ�����׼����ִ�к���
		/// ��Graph�е�ִ������
		/// </summary>
		class FramePass
		{
		protected:
			virtual void Setup() = 0;
			virtual void Execute() = 0;
			void AddInput();
		
		};

		class PassNode
		{

		};

	}
}

