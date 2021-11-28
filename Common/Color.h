#pragma once

#include"YiaMath.h"

namespace YiaEngine
{
	enum ColorType
	{
		RGBA,
		HSV,
	};
	struct Color
	{
	public:
	
	public:
		Color():R(1),G(1), B(1), A(1) {}
		Color(int hex) {*this = CreateFromHEX(hex); }
		Color(float r, float g, float b, float a) :R(r), G(g), B(b), A(a) {};
		static Color CreateFromUINT(int32_t r, int32_t g, int32_t b, int32_t a);
		static Color CreateFromHSV(int32_t r, int32_t g, int32_t b, int32_t a);
		static Color CreateFromHEX(int32_t hex);

		Color operator = (int hex) { return CreateFromHEX(hex); }

		float R;
		float G;
		float B;
		float A;
	};
	extern const Color Black;
	extern const Color White;
	extern const Color Red;
	extern const Color Green;
	extern const Color Blue;
	extern const Color DefaultColor;
	extern const Color ErrorColor;
}