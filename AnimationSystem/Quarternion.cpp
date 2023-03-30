#include "Quarternion.h"
#include <cmath>

Quaternion AngleAxis(float angle, const vec3& axis)
{
    vec3 norm = normalized(axis);
    float s = sinf(angle * 0.5f);
    return Quaternion(norm.x * s,
        norm.y * s,
        norm.z * s,
        cosf(angle * 0.5f)
    );

}

Quaternion FromTo(const vec3& from, const vec3& to)
{
    vec3 f = normalized(from);
    vec3 t = normalized(to);
    if (f == t) {
        return Quaternion();
    }
    else if (f == t * -1.0f) {
        vec3 ortho = vec3(1, 0, 0);
        if (fabsf(f.y) < fabsf(f.x)) {
            ortho = vec3(0, 1, 0);
        }
        if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
            ortho = vec3(0, 0, 1);
        }
        vec3 axis = normalized(cross(f, ortho));
        return Quaternion(axis.x, axis.y, axis.z, 0);
    }

    vec3 half = normalized(f + t);
    vec3 axis = cross(f, half);
    return Quaternion(axis.x, axis.y, axis.z, dot(f, half));
}

vec3 getAxis(const Quaternion& quat) {
    return normalized(vec3(quat.x, quat.y, quat.z));
}


float getAngle(const Quaternion& quat) {
    return 2.0f * acosf(quat.w);
}

Quaternion operator+(const Quaternion& a, const Quaternion& b)
{
    return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

Quaternion operator-(const Quaternion& a, const Quaternion& b)
{
    return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

Quaternion operator*(const Quaternion& Q1, const Quaternion& Q2)
{
    return Quaternion(
        Q2.x * Q1.w + Q2.y * Q1.z - Q2.z * Q1.y + Q2.w * Q1.x,
        -Q2.x * Q1.z + Q2.y * Q1.w + Q2.z * Q1.x + Q2.w * Q1.y,
        Q2.x * Q1.y - Q2.y * Q1.x + Q2.z * Q1.w + Q2.w * Q1.z,
        -Q2.x * Q1.x - Q2.y * Q1.y - Q2.z * Q1.z + Q2.w * Q1.w);
}

Quaternion operator*(const Quaternion& a, float b)
{
    return Quaternion(a.x * b, a.y * b, a.z * b, a.w * b);
}

Quaternion operator-(const Quaternion& q)
{
    return Quaternion(-q.x, -q.y, -q.z, -q.w);
}

vec3 operator*(const Quaternion& q, const vec3& v)
{
    return q.vector * 2.0f * dot(q.vector, v) +
        v * (q.scalar * q.scalar - dot(q.vector, q.vector)) +
        cross(q.vector, v) * 2.0f * q.scalar;
}

Quaternion operator^(const Quaternion& q, float f)
{
    float angle = 2.0f * acosf(q.scalar);
    vec3 axis = normalized(q.vector);
    float halfCos = cosf(f * angle * 0.5f);
    float halfSin = sinf(f * angle * 0.5f);
    return Quaternion(axis.x * halfSin,
        axis.y * halfSin,
        axis.z * halfSin,
        halfCos
    );
}

bool operator==(const Quaternion& left, const Quaternion& right)
{
    return(fabsf(left.x - right.x) <= QUAT_EPSILON &&
        fabsf(left.y - right.y) <= QUAT_EPSILON &&
        fabsf(left.z - right.z) <= QUAT_EPSILON &&
        fabsf(left.w - right.w) <= QUAT_EPSILON);
}

bool operator!=(const Quaternion& a, const Quaternion& b)
{
    return !(a == b);
}

bool SameOrientation(const Quaternion& l, const Quaternion& r)
{
    return (fabsf(l.x - r.x) <= QUAT_EPSILON &&
        fabsf(l.y - r.y) <= QUAT_EPSILON &&
        fabsf(l.z - r.z) <= QUAT_EPSILON &&
        fabsf(l.w - r.w) <= QUAT_EPSILON) ||
        (fabsf(l.x + r.x) <= QUAT_EPSILON &&
            fabsf(l.y + r.y) <= QUAT_EPSILON &&
            fabsf(l.z + r.z) <= QUAT_EPSILON &&
            fabsf(l.w + r.w) <= QUAT_EPSILON);
}

float dot(const Quaternion& a, const Quaternion& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float lenSq(const Quaternion& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float len(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void normalize(Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return;
    }
    float i_len = 1.0f / sqrtf(lenSq);
    q.x *= i_len;
    q.y *= i_len;
    q.z *= i_len;
    q.w *= i_len;

}

Quaternion normalized(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return Quaternion();
    }
    float il = 1.0f / sqrtf(lenSq); // il: inverse length
    return Quaternion(q.x * il, q.y * il, q.z * il, q.w * il);
}

Quaternion conjugate(const Quaternion& q)
{
    return Quaternion(
        -q.x,
        -q.y,
        -q.z,
        q.w
    );
}

Quaternion inverse(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return Quaternion();
    }
    float recip = 1.0f / lenSq;
    return Quaternion(
        -q.x * recip,
        -q.y * recip,
        -q.z * recip,
        q.w * recip
    );
}

Quaternion mix(const Quaternion& from, const Quaternion& to, float t)
{
    return from * (1.0f - t) + to * t;
}

Quaternion nlerp(const Quaternion& from, const Quaternion& to, float t)
{
    return normalized(from + (to - from) * t);
}

Quaternion slerp(const Quaternion& start, const Quaternion& end, float t)
{
    if (fabsf(dot(start, end)) > 1.0f - QUAT_EPSILON) {
        return nlerp(start, end, t);
    }
    Quaternion delta = inverse(start) * end;
    return normalized((delta ^ t) * start);
}

Quaternion lookRotation(const vec3& direction, const vec3& up)
{
    // Find orthonormal basis vectors
    vec3 f = normalized(direction); // Object Forward
    vec3 u = normalized(up); // Desired Up
    vec3 r = cross(u, f); // Object Right
    u = cross(f, r); // Object Up
    // From world forward to object forward
    Quaternion worldToObject = FromTo(vec3(0, 0, 1), f);
    // what direction is the new object up?
    vec3 objectUp = worldToObject * vec3(0, 1, 0);
    // From object up to desired up
    Quaternion u2u = FromTo(objectUp, u);
    // Rotate to forward direction first
    // then twist to correct up
    Quaternion result = worldToObject * u2u;

    // Don't forget to normalize the result
    return normalized(result);
}

mat4 quatToMat4(const Quaternion& q)
{
    vec3 r = q * vec3(1, 0, 0);
    vec3 u = q * vec3(0, 1, 0);
    vec3 f = q * vec3(0, 0, 1);
    return mat4(r.x, r.y, r.z, 0,
        u.x, u.y, u.z, 0,
        f.x, f.y, f.z, 0,
        0, 0, 0, 1
    );
}

Quaternion mat4ToQuat(const mat4& m)
{
    vec3 up = normalized(vec3(m.up.x, m.up.y, m.up.z));
    vec3 forward = normalized(
        vec3(m.forward.x, m.forward.y, m.forward.z));
    vec3 right = cross(up, forward);
    up = cross(forward, right);

    return lookRotation(forward, up);
}
