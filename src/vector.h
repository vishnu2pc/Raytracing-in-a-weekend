#ifndef VEC3_H
#define VEC3_H

union vec3 {
	struct {
		double x, y, z;
	};
	struct {
		double r, g, b;
	};
	double elements[3];
};

inline vec3 vec3_add(vec3& left, vec3& right) {
	vec3 result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;
	return result;
}

inline vec3 operator+(vec3& left, vec3& right) {
	return vec3_add(left, right);
}

inline vec3 vec3_sub(vec3& left, vec3& right) {
	vec3 result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;
	return result;
}

inline vec3 operator-(vec3& left, vec3& right) {
	return vec3_sub(left, right);
}

inline vec3 vec3_muld(vec3& left, double right) {
	vec3 result;
	result.x = left.x * right;
	result.y = left.y * right;
	result.z = left.z * right;
	return result;
}

inline vec3 operator*(vec3& left, double right) {
	return vec3_muld(left, right);
}

inline vec3 operator*(double left, vec3& right) {
	return vec3_muld(right, left);
}

inline vec3 operator*(vec3& left, vec3& right) {
	vec3 result;
	result.x = left.x * right.x;
	result.y = left.y * right.y;
	result.z = left.z * right.z;
	return result;
}

inline vec3 vec3_divd(vec3& left, double right) {
	vec3 result;
	result.x = left.x / right;
	result.y = left.y / right;
	result.z = left.z / right;
	return result;
}

inline vec3 operator/(vec3& left, double right) {
	return vec3_divd(left, right);
}

inline vec3 operator/(double left, vec3& right) {
	return vec3_divd(right, left);
}

bool vec3_near_zero(vec3& a) {
	double s = 1e-8;
	return ((fabs(a.x)<s) && (fabs(a.y)<s) && (fabs(a.z)<s));
}

inline double vec3_dot(vec3 a, vec3& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline double vec3_length_squared(vec3& a) {
	return vec3_dot(a,a);
}

inline double vec3_length(vec3& a) {
	return sqrt(vec3_length_squared(a));
}

inline vec3 vec3_unit(vec3& a) {
	return a/vec3_length(a);
}

inline vec3 vec3_cross(vec3& a, vec3& b) {
	vec3 result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;
	return result;
}

#endif
