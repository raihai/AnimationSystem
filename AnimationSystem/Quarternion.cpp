#include "Quarternion.h"
#include <cmath>

Quaternion AngleAxis(float angle, const vec3& axis)
{
    vec3 norm = Normalised(axis);
    float s = sinf(angle * 0.5f);
    return Quaternion(norm.x * s,
        norm.y * s,
        norm.z * s,
        cosf(angle * 0.5f)
    );

}

Quaternion FromTo(const vec3& from, const vec3& to)
{
    vec3 f = Normalised(from);
    vec3 t = Normalised(to);
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
        vec3 axis = Normalised(Cross(f, ortho));
        return Quaternion(axis.x, axis.y, axis.z, 0);
    }

    vec3 half = Normalised(f + t);
    vec3 axis = Cross(f, half);
    return Quaternion(axis.x, axis.y, axis.z, Dot(f, half));
}

vec3 GetAxis(const Quaternion& quat) {
    return Normalised(vec3(quat.x, quat.y, quat.z));
}


float GetAngle(const Quaternion& quat) {
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
    return q.vector * 2.0f * Dot(q.vector, v) +
        v * (q.scalar * q.scalar - Dot(q.vector, q.vector)) +
        Cross(q.vector, v) * 2.0f * q.scalar;
}

Quaternion operator^(const Quaternion& q, float f)
{
    float angle = 2.0f * acosf(q.scalar);
    vec3 axis = Normalised(q.vector);
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

float Dot(const Quaternion& a, const Quaternion& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

float LenSq(const Quaternion& q)
{
    return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
}

float Len(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return 0.0f;
    }
    return sqrtf(lenSq);
}

void Normalise(Quaternion& q)
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

Quaternion Normalised(const Quaternion& q)
{
    float lenSq = q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w;
    if (lenSq < QUAT_EPSILON) {
        return Quaternion();
    }
    float il = 1.0f / sqrtf(lenSq); // il: inverse length
    return Quaternion(q.x * il, q.y * il, q.z * il, q.w * il);
}

Quaternion Conjugate(const Quaternion& q)
{
    return Quaternion(
        -q.x,
        -q.y,
        -q.z,
        q.w
    );
}

Quaternion Inverse(const Quaternion& q)
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

Quaternion Mix(const Quaternion& from, const Quaternion& to, float t)
{
    return from * (1.0f - t) + to * t;
}

Quaternion Nlerp(const Quaternion& from, const Quaternion& to, float t)
{
    return Normalised(from + (to - from) * t);
}

Quaternion Slerp(const Quaternion& start, const Quaternion& end, float t)
{
    if (fabsf(Dot(start, end)) > 1.0f - QUAT_EPSILON) {
        return Nlerp(start, end, t);
    }
    Quaternion delta = Inverse(start) * end;
    return Normalised((delta ^ t) * start);
}

Quaternion LookRotation(const vec3& direction, const vec3& up)
{
    // Find orthonormal basis vectors
    vec3 f = Normalised(direction); // Object Forward
    vec3 u = Normalised(up); // Desired Up
    vec3 r = Cross(u, f); // Object Right
    u = Cross(f, r); // Object Up
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
    return Normalised(result);
}

mat4 QuatToMat4(const Quaternion& q)
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

Quaternion Mat4ToQuat(const mat4& m)
{
    vec3 up = Normalised(vec3(m.up.x, m.up.y, m.up.z));
    vec3 forward = Normalised(
        vec3(m.forward.x, m.forward.y, m.forward.z));
    vec3 right = Cross(up, forward);
    up = Cross(forward, right);

    return LookRotation(forward, up);
}
