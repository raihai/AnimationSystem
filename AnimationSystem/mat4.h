#pragma once
#include "vec4.h"

#define MAT4_EPSILON  0.000001f

struct mat4 {
	union {
		float v[16];
		struct {
			vec4 right;
			vec4 up;
			vec4 forward;
			vec4 position;
		};

		struct {
			// row 1 row 2 row 3 row 4
			/*col 1*/float xx; float xy; float xz; float xw;
			/*col 2*/float yx; float yy; float yz; float yw;
			/*col 3*/float zx; float zy; float zz; float zw;
			/*col 4*/float tx; float ty; float tz; float tw;
		};

		// coloumn-row notation //
		struct {
			float c0r0; float c0r1; float c0r2; float c0r3;
			float c1r0; float c1r1; float c1r2; float c1r3;
			float c2r0; float c2r1; float c2r2; float c2r3;
			float c3r0; float c3r1; float c3r2; float c3r3;
		};

		// row-column notation //
		struct {
			float r0c0; float r1c0; float r2c0; float r3c0;
			float r0c1; float r1c1; float r2c1; float r3c1;
			float r0c2; float r1c2; float r2c2; float r3c2;
			float r0c3; float r1c3; float r2c3; float r3c3;
		};
	};

	inline mat4(
		float _00, float _01, float _02, float _03,
		float _10, float _11, float _12, float _13,
		float _20, float _21, float _22, float _23,
		float _30, float _31, float _32, float _33):
		xx(_00), xy(_01), xz(_02), xw(_03),
		yx(_10), yy(_11), yz(_12), yw(_13),
		zx(_20), zy(_21), zz(_22), zw(_23),
		tx(_30), ty(_31), tz(_32), tw(_33) { }
};

bool operator!=(const mat4& a, const mat4& b);
bool operator==(const mat4& a, const mat4& b);
mat4 operator+(const mat4& a, const mat4& b);
mat4 operator*(const mat4& m, float f);
