#include"pch.h"

#include"Color.h"


namespace YiaEngine
{
	const Color Black =0x000000FF;
	const Color White = 0xFFFFFFFF;
	const Color Red = 0xFF0000FF;
	const Color Green = 0x00FF00FF;
	const Color Blue = 0x0000FFFF;
	const Color DefaultColor = 0x8ECAE6FF;
	const Color ErrorColor = 0xF72585FF;

	Color Color::CreateFromUINT(int32_t r, int32_t g, int32_t b, int32_t a)
	{
		Color ret;
		ret.R = (float)r / 255.f;
		ret.G = (float)g / 255.f;
		ret.B = (float)b / 255.f;
		ret.A = (float)a / 255.f;
		return ret;
	}

	Color Color::CreateFromHEX(int32_t  hex)
	{
		UINT r = hex & 0xff000000;
		UINT g = hex & 0x00ff0000;
		UINT b = hex & 0x0000ff00;
		UINT a = hex & 0x000000ff;
		return CreateFromUINT(r>>24, g>>16, b>>8, a);
	}

}