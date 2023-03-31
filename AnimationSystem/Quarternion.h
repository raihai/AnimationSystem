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
vec3 GetAxis(const Quaternion& quat);
float GetAngle(const Quaternion& quat);

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

float Dot(const Quaternion& a, const Quaternion& b);
float LenSq(const Quaternion& q);
float Len(const Quaternion& q);

void Normalise(Quaternion& q);
Quaternion Normalised(const Quaternion& q);

Quaternion Conjugate(const Quaternion& q);
Quaternion Inverse(const Quaternion& q);

Quaternion Mix(const Quaternion& from, const Quaternion& to, float t);
Quaternion Nlerp(const Quaternion& from, const Quaternion& to, float t);
Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t);

Quaternion LookRotation(const vec3& direction, const vec3& up);

mat4 QuatToMat4(const Quaternion& q);
Quaternion Mat4ToQuat(const mat4& m);