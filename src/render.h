#ifndef RENDER_H
#define RENDER_H

struct camera {
	vec3 origin;
	vec3 lower_left_corner;
	vec3 hor;
	vec3 ver;
	vec3 u,v,w;
	double lens_radius;
};

struct hit_record {
	vec3 point;
	vec3 normal;
	material mat;
	double distance;
	vec3 u,v,w;
	bool front_face;
};

struct ray {
	vec3 origin;
	vec3 dir;
};

struct sphere {
	vec3 origin;
	double radius;
	material mat;
};

vec3 ray_end(ray& r, double d) {
	return r.origin + r.dir*d;
}

void rec_set_face_normal(hit_record& rec, ray& r, vec3& outward_normal) {
	rec.front_face = vec3_dot(r.dir, outward_normal) < 0;
	if(rec.front_face) rec.normal = outward_normal;
	else rec.normal = vec3{0,0,0}-outward_normal;
}

camera make_camera(vec3 origin, vec3 lookat, vec3 vup, double aspect_ratio, double fov, double aperture, double focus_dist) {
	camera result;
	double theta = deg_to_rad(fov);
	double h = tan(theta/2);
	double vp_height = 2.0 * h;
	double vp_width = aspect_ratio*vp_height;

	result.w = vec3_unit(origin-lookat);
	result.u = vec3_unit(vec3_cross(vup,result.w));
	result.v = vec3_cross(result.w, result.u);

	result.origin = origin;
	result.hor = focus_dist*vp_width*result.u;
	result.ver = focus_dist*vp_height*result.v;
	result.lower_left_corner = origin - result.hor/2 - result.ver/2 - focus_dist*result.w;
	result.lens_radius = aperture/2;

	return result;
}

ray get_camera_ray(camera &cam, double s, double t) {
	ray result;
	vec3 rd = cam.lens_radius * rand_in_unit_disk();
	vec3 offset = cam.u*rd.x + cam.v*rd.y;

	result.origin = cam.origin + offset;
	result.dir = vec3_unit(cam.lower_left_corner + s*cam.hor + t*cam.ver - cam.origin - offset);
	return result;
}

vec3 get_refraction(vec3& i, vec3& n, double refraction_ratio) {
	double cos_theta = fmin(vec3_dot(vec3{0,0,0}-i, n), 1.0);
	vec3 r_out_perp = refraction_ratio*(i+cos_theta*n);
	vec3 r_out_parallel = -sqrt(fabs(1.0-vec3_length_squared(r_out_perp)))*n;
	return r_out_perp + r_out_parallel;
}

vec3 get_reflection(vec3& i, vec3& n) {
	return i - 2*vec3_dot(i,n)*n;
}

double get_reflectance(double cos, double refraction_ratio) {
	double r0 = (1-refraction_ratio)/(1+refraction_ratio);	
	r0 = r0*r0;
	return r0 + (1-r0)*pow((1-cos),5);
};

bool hit_sphere(ray& r, double t_min, double t_max, sphere* spheres, int num, hit_record& rec) {
	double t_new_max = t_max;

	bool hit = false;
	for(int i=0; i<num; i++) {
		double radius = spheres[i].radius;
		vec3 center = spheres[i].origin;
		material mat =spheres[i].mat;

		vec3 oc = r.origin - center;
		double a = vec3_length_squared(r.dir);
		double half_b = vec3_dot(oc, r.dir);
		double c = vec3_length_squared(oc) - radius*radius;
		double dis = half_b*half_b - a*c;
		if(dis<0) continue;
		double sqrtd = sqrt(dis);
		double root = (-half_b-sqrtd)/a;
		if(root<t_min || root>t_new_max) {
			root = (-half_b+sqrtd)/a;
			if(root<t_min || root>t_new_max) {
				continue;
			}
		}

		hit = true;
		t_new_max = root;
		rec.distance = root;
		rec.point = ray_end(r, root);
		vec3 outward_normal = (rec.point-center)/radius;
		rec_set_face_normal(rec, r, outward_normal);
		rec.mat = mat;
	}
	return hit;
}

#endif
