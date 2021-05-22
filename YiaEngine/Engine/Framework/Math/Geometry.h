#pragma once
#ifndef FRAMEWORL_MATH_GEOMETRY_H_
#define FRAMEWORL_MATH_GEOMETRY_H_



#include"GeomMath.h"

using namespace YiaEngine;

namespace YiaEngine
{
	struct Vertex
	{
		vec3f position;
		vec4f color;
		vec2f uv;
	};
}//YiaEngine

#endif // !FRAMEWORL_MATH_GEOMETRY_H_