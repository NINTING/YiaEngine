
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
	//const float in[3] = { 1,3,5 };
	//const int iin[3] = { 1,3,5 };
	//float out[3];
	////simpleTest(in,out,3);
	////printf("%f %f %f", out[0], out[1], out[2]);
 //	YiaEngine::swizzle<int, 0, 1>  xy;
	//YiaEngine::Vec2f v2(3, 2);
	//YiaEngine::Vec2f vt2(4, 5);
	
	//YiaEngine::Vec2f s = v3.yx + vt2.yx;
	//s = v3.yx + vt2;
	//v2.xy = v2.yx;
	 /*v3 = vt2.yx + v2.xy;
	 v3.yx = vt2.yx + v2.xy;
	 YiaEngine::Vec2f v1 = vt2.yx ;*/
	YiaEngine::Matrix<int,8,8> pixel_block = {
		-76, -73, -67, -62, -58, -67, -64, -55,
		-65, -69, -73, -38, -19, -43, -59, -56,
		-66, -69, -60, -15,  16, -24, -62, -55,
		-65, -70, -57, -6,  26, -22, -58, -59,
		-61, -67, -60, -24, -2, -40, -60, -58,
		-49, -63, -68, -58, -51, -60, -70, -53,
		-43, -57, -64, -69, -73, -67, -63, -45,
		-41, -49, -59, -60, -63, -52, -50, -34
	};
	

	auto out = YiaEngine::DCT8X8(pixel_block);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			auto v = out[i][j];
			printf("%lf ", v);
		}
		printf("\n");
	}
}