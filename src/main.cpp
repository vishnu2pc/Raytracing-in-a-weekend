#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define ARRAY_COUNT(array) (sizeof(array)/sizeof(array[0]))

#include "vector.h"
#include "math.h"
#include "materials.h"
#include "render.h"

#define NUM_SPHERES 500
int count = 0;

vec3 ray_color(ray& r, sphere* spheres, int num, int depth) {
	hit_record rec;
	if(depth<0) return vec3{0,0,0};
	bool hit = hit_sphere(r, 0.001, DBL_MAX, spheres, num, rec);
	if(hit) {
		vec3 reflect;
		vec3 refract;
		ray scatter;
		vec3 attenuation;
		switch(rec.mat.type) {
			case METAL:
				reflect = r.dir - 2*vec3_dot(r.dir, rec.normal)*rec.normal;
				scatter = ray{rec.point, reflect + rec.mat.fuzz*rand_in_unit_sphere()};
				attenuation = rec.mat.albedo;
				if(vec3_dot(scatter.dir, rec.normal)<0)
					return vec3{0,0,0};
				break;
			case LAMBERTIAN: 
				reflect = rec.normal + rand_unit_vector();
				if(vec3_near_zero(reflect)) reflect = rec.normal; 
				scatter = ray{rec.point, reflect};
				attenuation = rec.mat.albedo;
				break;
			case DIELECTRIC:
				attenuation = vec3{1.0,1.0,1.0};
				double refraction_ratio;
				if(rec.front_face) refraction_ratio = 1.0 /rec.mat.ir;
				else refraction_ratio = rec.mat.ir;
				double cos_theta = fmin(vec3_dot(vec3{0,0,0}-r.dir, rec.normal), 1.0);
				double sin_theta = sqrt(1-cos_theta*cos_theta);
				bool can_refract = refraction_ratio*sin_theta < 1.0;
				vec3 dir;
				if(can_refract && get_reflectance(cos_theta, refraction_ratio) < rand_double()) {
					vec3 refract = get_refraction(r.dir, rec.normal, refraction_ratio);
					scatter = ray{rec.point, refract};
				} else {
					vec3 reflect = get_reflection(r.dir, rec.normal);
					scatter = ray{rec.point, reflect};
				}
				break;
		}
		return attenuation*ray_color(scatter, spheres, num, depth-1);
	}
	double t = 0.5*(r.dir.y+1.0);
	return (1-t)*vec3{1,1,1} + t*vec3{0.5,0.7,1.0};
}

int main() 
{
	clock_t begin = clock();
	material ground_mat = make_material_lambertian(vec3{0.5,0.5,0.5});
	double R = cos(PI/4);

	sphere sphere_list[NUM_SPHERES];
	sphere_list[count++] = {vec3{ 0.0, -1000.0, 0}, 1000.0, ground_mat};

	for(int a=-11; a<11; a++) {
		for(int b=-11; b<11; b++) {
			double choose_mat = rand_double();
			vec3 center{a + 0.9*rand_double(), 0.2, b + 0.9*rand_double()};

			if(vec3_length((center-vec3{4,0.2,0})) > 0.9) {
				material mat;
				if(choose_mat < 0.8) {
					vec3 albedo = vec3_rand()*vec3_rand();
					mat = make_material_lambertian(albedo);
					sphere_list[count++] = {center, 0.2, mat};
				} else if(choose_mat<0.95) {
					vec3 albedo = vec3_rand_in_range(0.5,1);
					double fuzz = rand_double_in_range(0,0.5);
					mat = make_material_metal(albedo, fuzz);
					sphere_list[count++] = {center, 0.2, mat};
				} else {
					mat = make_material_dielectric(1.5);
					sphere_list[count++] = {center, 0.2, mat};
				}
			}
		}
	}
	material mat1 = make_material_dielectric(1.5);
	material mat2 = make_material_lambertian(vec3{0.4,0.2,0.1});
	material mat3 = make_material_metal(vec3{0.7,0.6,0.5}, 0.0);

	sphere_list[count++] = {vec3{0,1,0}, 1.0, mat1};
	sphere_list[count++] = {vec3{-4,1,0}, 1.0, mat2};
	sphere_list[count++] = {vec3{4,1,0}, 1.0, mat3};

	const double aspect_ratio = 3.0/2.0;
	const int image_width = 1200;
	const int image_height = (int)(image_width/aspect_ratio);
	const int samples_per_pixel = 30;
	const int max_depth = 50;

	vec3 origin = vec3{13,2,3};
	vec3 lookat = vec3{0,0,0};
	vec3 vup = vec3{0,1,0};
	double dist_to_focus = 10;
	double aperture = 0.1;
	camera cam = make_camera(origin, lookat, vup, aspect_ratio, 20, aperture, dist_to_focus);

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for(int j=image_height-1; j >= 0; --j ) {
		std::cerr << " \rScanlines remaining: " << j << ' ' << std::flush;
		for(int i=0; i<image_width; ++i)  {
			vec3 pixel = vec3{0,0,0};
			for(int s=0; s<samples_per_pixel; ++s) {
				double u = (i+rand_double())/(image_width-1);
				double v = (j+rand_double())/(image_height-1);
				ray r = get_camera_ray(cam, u, v);
				pixel = pixel + ray_color(r, sphere_list, count, max_depth);
			}
			double r = pixel.x;
			double g = pixel.y;
			double b = pixel.z;

			double scale = 1.0/samples_per_pixel;
			r = sqrt(r*scale);
			g = sqrt(g*scale);
			b = sqrt(b*scale);

			int ir = (int)(256*clamp(r,0,0.999));
			int ig = (int)(256*clamp(g,0,0.999));
			int ib = (int)(256*clamp(b,0,0.999));
			std::cout << ir << ' ' << ig << ' ' << ib << '\n';
		}
	}
	clock_t end = clock();
	double time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	std::cerr << "\nDone.\n";
	std::cerr << "Time spent: " << time_spent;
	getchar();
	return 0;
};
