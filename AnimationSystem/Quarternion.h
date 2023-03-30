#ifndef _H_QUAT_
#define _H_QUAT_
#include "vec3.h"
#include "mat4.h"

#define QUAT_EPSILON 0.000001f

struct Quaternion {
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		struct {
			vec3 vector;
			float scalar;
		};
		float v[4];
	};
	inline Quaternion() :
		x(0), y(0), z(0), w(1) { }
	inline Quaternion(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {}
};
#endif


Quaternion AngleAxis(float angle, const vec3& axis);
Quaternion FromTo(const vec3& from, const vec3& to);
vec3 getAxis(const Quaternion& quat);
float getAngle(const Quaternion& quat);

Quaternion operator+(const Quaternion& a, const Quaternion& b);
Quaternion operator-(const Quaternion& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, const Quaternion& b);
Quaternion operator*(const Quaternion& a, float b);
Quaternion operator-(const Quaternion& q);
vec3 operator*(const Quaternion& q, const vec3& v);
Quaternion operator^(const Quaternion& q, float f);

bool operator==(const Quaternion& left, const Quaternion& right);
bool operator!=(const Quaternion& a, const Quaternion& b);
bool SameOrientation(const Quaternion& l, const Quaternion& r);

float dot(const Quaternion& a, const Quaternion& b);
float lenSq(const Quaternion& q);
float len(const Quaternion& q);

void normalize(Quaternion& q);
Quaternion normalized(const Quaternion& q);

Quaternion conjugate(const Quaternion& q);
Quaternion inverse(const Quaternion& q);

Quaternion mix(const Quaternion& from, const Quaternion& to, float t);
Quaternion nlerp(const Quaternion& from, const Quaternion& to, float t);
Quaternion slerp(const Quaternion& start, const Quaternion& end, float t);

Quaternion lookRotation(const vec3& direction, const vec3& up);

mat4 quatToMat4(const Quaternion& q);
Quaternion mat4ToQuat(const mat4& m);