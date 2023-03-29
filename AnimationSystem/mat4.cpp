#include "mat4.h"
#include <math.h>

bool operator==(const mat4& a, const mat4& b) {
	for (int i = 0; i < 16; ++i) {
		if (fabsf(a.v[i] - b.v[i]) > MAT4_EPSILON) {
			return false;
		}
	}
	return true;
}

bool operator!=(const mat4& a, const mat4& b) {
	return !(a == b);
}

mat4 operator+(const mat4& a, const mat4& b) {
	return mat4(
		a.xx + b.xx, a.xy + b.xy, a.xz + b.xz, a.xw + b.xw,
		a.yx + b.yx, a.yy + b.yy, a.yz + b.yz, a.yw + b.yw,
		a.zx + b.zx, a.zy + b.zy, a.zz + b.zz, a.zw + b.zw,
		a.tx + b.tx, a.ty + b.ty, a.tz + b.tz, a.tw + b.tw
	);
}

mat4 operator*(const mat4& m, float f)
{
	return mat4(
		m.xx * f, m.xy * f, m.xz * f, m.xw * f,
		m.yx * f, m.yy * f, m.yz * f, m.yw * f,
		m.zx * f, m.zy * f, m.zz * f, m.zw * f,
		m.tx * f, m.ty * f, m.tz * f, m.tw * f
	);
}
