
#include <stdio.h>
#include "GeomMath.h"

int main()
{
	printf("Tesr\n");
	const float in[3] = { 1,3,5 };
	const int iin[3] = { 1,3,5 };
	float out[3];
	//simpleTest(in,out,3);
	//printf("%f %f %f", out[0], out[1], out[2]);
 	YiaEngine::swizzle<int, 0, 1>  xy;
	YiaEngine::vec2f v2(3, 2);
	YiaEngine::vec2f vt2(4, 5);
	YiaEngine::vec2f v3(1,2);
	 auto v = v2.xy + vt2.yx;
 	 v3 = vt2.yx + v2.xy;
	 v3.yx = vt2.yx + v2.xy;
	 YiaEngine::vec2f v1 = vt2.yx ;
	 YiaEngine::DebugSwizzle(vt2.yx);
	 //YiaEngine::DebugSwizzle(v1);
	//YiaEngine::testV<float> t3 = t0 + t1;
///	printf("%f", t3.a);
}