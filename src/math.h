#ifndef MATH_H
#define MATH_H

#define PI 3.14159265358979323846

inline double deg_to_rad(double deg) {
	return deg*(PI/180.0);
}

inline double clamp(double x, double min, double max) {
	if(x<min) return min;
	if(x>max) return max;
	return x;
}

inline double rand_double() {
	return rand()/(RAND_MAX+1.0);
}

inline double rand_double_in_range(double min, double max) {
	return min + (max-min)*rand_double();
}

inline vec3 vec3_rand() {
	return vec3{rand_double(),rand_double(),rand_double()};
}

inline vec3 vec3_rand_in_range(double min, double max) {
	return vec3{rand_double_in_range(min,max), rand_double_in_range(min,max), rand_double_in_range(min,max)};
}

vec3 rand_in_unit_sphere() {
	while(true) {
		vec3 p = vec3_rand_in_range(-1,1);
		if(vec3_length_squared(p) >= 1) continue;
		return p;
	}
}

vec3 rand_in_hemisphere(vec3& normal) {
	vec3 a = rand_in_unit_sphere();
	if(vec3_dot(a, normal)>0.0) return a;
	return vec3{0,0,0}-a;
}

vec3 rand_in_unit_disk() {
	while(true) {
		vec3 p = vec3{rand_double_in_range(-1,1), rand_double_in_range(-1,1), 0};
		if(vec3_length_squared(p)>=1) continue;
		return p;
	}
}

vec3 rand_unit_vector() {
	return vec3_unit(rand_in_unit_sphere());
}

#endif
