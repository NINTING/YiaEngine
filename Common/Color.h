#pragma once

#include"YiaMath.h"

namespace YiaEngine
{
	enum ColorType
	{
		RGBA,
		HSV,
	};
	class Color
	{
	public:
		static Color Black;
		static Color White;
		static Color Red;
		static Color Green;
		static Color Blue;
	public:
		Color():R(White.R),G(White.G), B(White.B), A(White.A) {}
		Color(float r, float g, float b, float a) :R(r), G(g), B(b), A(a) {};
		float R;
		float G;
		float B;
		float A;
	};
}