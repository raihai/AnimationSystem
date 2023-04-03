#ifndef _H_TRANSFORM_
#define _H_TRANSFORM_

#include "vec3.h"
#include "Quaternion.h"

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
Transform Mix(const Transform& a, const Transform& b, float t);
mat4 TransformToMat4(const Transform& t);
Transform Mat4ToTransform(const mat4& m);

vec3 TransformPoint(const Transform& a, const vec3& b);
vec3 TransformVector(const Transform& a, const vec3& b);


#endif // !_TRANSFORM_