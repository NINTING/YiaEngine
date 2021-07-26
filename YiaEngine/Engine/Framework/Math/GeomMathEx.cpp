#include"GeomMath.h"


namespace YiaEngine
{

	Mat3x3f RotationByAxis(const Vec3f axis, float radians)
	{
		float c = cosf(radians);
		float s = sinf(radians);
		float subc = 1 - c;
		float  n_xy = axis.x * axis.y;
		float  n_xz = axis.x * axis.z;
		float  n_yz = axis.z * axis.y;

		float n_xx = axis.x * axis.x;
		float n_yy = axis.y * axis.y;
		float n_zz = axis.z * axis.z;

		float n_xs = axis.x * s;
		float n_ys = axis.y * s;
		float n_zs = axis.z * s;

		return {
			n_xx * subc + c	  , n_xy * subc + n_zs, n_xz * subc - n_ys,
			n_xy * subc - n_zs, n_yy * subc + c	  , n_yz * subc + n_xs,
			n_xz * subc + n_ys, n_yz * subc - n_xs, n_zz * subc + c,
		};
	}
}
