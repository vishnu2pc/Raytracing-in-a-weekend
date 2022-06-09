#ifndef MATERIALS_H
#define MATERIALS_H

enum MATERIAL_TYPE { LAMBERTIAN, METAL, DIELECTRIC };
struct material {
	enum MATERIAL_TYPE type;
	union {
		struct { vec3 albedo; };
		struct { vec3 albedo; double fuzz; };
		struct { double ir; };
	};
};

material make_material_lambertian(vec3 albedo) {
	material result;
	result.type = LAMBERTIAN;
	result.albedo = albedo;
	return result;
}

material make_material_metal(vec3 albedo, double fuzz) {
	material result;
	result.type = METAL;
	result.albedo = albedo;
	result.fuzz= fuzz;
	return result;
}

material make_material_dielectric(double ir) {
	material result;
	result.type = DIELECTRIC;
	result.ir= ir;
	return result;
}

#endif
