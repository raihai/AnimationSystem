#include "vec3.h"
#include <cmath>
#include <iostream>

vec3 operator+(const vec3& l, const vec3& r) {
	return vec3(l.x + r.x, l.y + r.y, l.z + r.z);
}

vec3 operator-(const vec3& l, const vec3& r) {
	return vec3(l.x - r.x, l.y - r.y, l.z - r.z);
}

vec3 operator*(const vec3& v, float f) {
	return vec3(v.x * f, v.y * f, v.z * f);
}

vec3 operator*(const vec3& l, const vec3& r) {
	return vec3(l.x * r.x, l.y * r.y, l.z * r.z);
}

float Dot(const vec3& l, const vec3& r) {
	return l.x * r.x + l.y * r.y + l.z * r.z;
}

float LenSq(const vec3& v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Len(const vec3& v) {
	float LenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (LenSq < VEC3_EPSILON) {
		return 0.0f;
	}
	return sqrtf(LenSq);
}

void Normalise(vec3& v) {
	float LenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (LenSq < VEC3_EPSILON) { return; }
	float invLen = 1.0f / sqrtf(LenSq);
	v.x *= invLen;
	v.y *= invLen;
	v.z *= invLen;
}

vec3 Normalised(const vec3& v) {
	float LenSq = v.x * v.x + v.y * v.y + v.z * v.z;
	if (LenSq < VEC3_EPSILON) { return v; }
	float invLen = 1.0f / sqrtf(LenSq);
	return vec3(
		v.x * invLen,
		v.y * invLen,
		v.z * invLen
	);
}

float Angle(const vec3& l, const vec3& r) {
	float sqMagL = l.x * l.x + l.y * l.y + l.z * l.z;
	float sqMagR = r.x * r.x + r.y * r.y + r.z * r.z;
	if (sqMagL < VEC3_EPSILON || sqMagR < VEC3_EPSILON) {
		return 0.0f;
	}
	float Dot = l.x * r.x + l.y * r.y + l.z * r.z;
	float Len = sqrtf(sqMagL) * sqrtf(sqMagR);
	return acosf(Dot / Len); 

	/*The acosf function returns angles in radians. To convert radians to
	degrees, multiply by 57.2958f. To convert degrees to radians, multiply by
	0.0174533f*/
}

vec3 Project(const vec3& a, const vec3& b) {
	float magBSq = Len(b);
	if (magBSq < VEC3_EPSILON) {
		return vec3();
	}
	float scale = Dot(a, b) / magBSq;
	return b * scale;
}

vec3 Reject(const vec3& a, const vec3& b) {
	vec3 Projection = Project(a, b);
	return a - Projection;
}

vec3 Reflect(const vec3& a, const vec3& b) {
	float magBSq = Len(b);
	if (magBSq < VEC3_EPSILON) {
		return vec3();
	}
	float scale = Dot(a, b) / magBSq;
	vec3 proj2 = b * (scale * 2);
	return a - proj2;
}

vec3 Cross(const vec3& l, const vec3& r) {
	return vec3(
		l.y * r.z - l.z * r.y,
		l.z * r.x - l.x * r.z,
		l.x * r.y - l.y * r.x
	);
}

vec3 Lerp(const vec3& s, const vec3& e, float t) {
	return vec3(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
}

vec3 Slerp(const vec3& s, const vec3& e, float t) {
	if (t < 0.01f) {
		return Lerp(s, e, t);
	}
	vec3 from = Normalised(s);
	vec3 to = Normalised(e);
	float theta = Angle(from, to);
	float sin_theta = sinf(theta);
	float a = sinf((1.0f - t) * theta) / sin_theta;
	float b = sinf(t * theta) / sin_theta;
	return from * a + to * b;
}

vec3 Nlerp(const vec3& s, const vec3& e, float t) {
	vec3 linear(
		s.x + (e.x - s.x) * t,
		s.y + (e.y - s.y) * t,
		s.z + (e.z - s.z) * t
	);
	return Normalised(linear);
}


bool operator==(const vec3& l, const vec3& r) {
	vec3 diff(l - r);
	return LenSq(diff) < VEC3_EPSILON;
}
bool operator!=(const vec3& l, const vec3& r) {
	return !(l == r);
}