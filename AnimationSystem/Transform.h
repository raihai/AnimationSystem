#pragma once

#include "vec3.h"
#include "Quarternion.h"

struct Transform {
	vec3 position;
	Quaternion rotation;
	vec3 scale;

	Transform(const vec3& p, const Quaternion& r, const vec3& s) :
		position(p), rotation(r), scale(s) {}
	Transform() :
		position(vec3(0, 0, 0)),
		rotation(Quaternion(0, 0, 0, 1)),
		scale(vec3(1, 1, 1))
	{}
}; // End of transform struct


Transform Combine(const Transform& a, const Transform& b);
Transform Inverse(const Transform& t);
Transform mix(const Transform& a, const Transform& b, float t);
mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const mat4& m);

vec3 transformPoint(const Transform& a, const vec3& b);
vec3 transformVector(const Transform& a, const vec3& b);