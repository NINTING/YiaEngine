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
	
	public:
		Color():R(1),G(1), B(1), A(1) {}
		Color(float r, float g, float b, float a) :R(r), G(g), B(b), A(a) {};
		float R;
		float G;
		float B;
		float A;
	};
	extern Color Black;
	extern Color White;
	extern Color Red;
	extern Color Green;
	extern Color Blue;
}