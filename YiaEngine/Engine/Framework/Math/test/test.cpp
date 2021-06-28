
#include <stdio.h>
#include "GeomMath.h"
#include<string>

enum pri{

	triangle,
	list
};

int main()
{
	
	printf("Tesr\n");
	const float in[3] = { 1,3,5 };
	const int iin[3] = { 1,3,5 };
	float out[3];
	//simpleTest(in,out,3);
	//printf("%f %f %f", out[0], out[1], out[2]);
 	YiaEngine::swizzle<int, 0, 1>  xy;
	YiaEngine::Vec2f v2(3, 2);
	YiaEngine::Vec2f vt2(4, 5);
	YiaEngine::Vec2f v3(1,2);
	YiaEngine::Vec2f s = v3.yx + vt2.yx;
	//s = v3.yx + vt2;
	//v2.xy = v2.yx;
	 /*v3 = vt2.yx + v2.xy;
	 v3.yx = vt2.yx + v2.xy;
	 YiaEngine::Vec2f v1 = vt2.yx ;*/

	

}