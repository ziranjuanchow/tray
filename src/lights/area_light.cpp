#include "scene.h"
#include "monte_carlo/util.h"
#include "lights/area_light.h"

AreaLight::AreaLight(const Transform &to_world, const Colorf &emit, int n_samples, float radius)
	: PBRLight(to_world, n_samples), emit(emit), geometry(std::make_unique<Sphere>(radius)),
	surface_area(geometry->surface_area())
{}
Colorf AreaLight::radiance(const Point&, const Normal &n, const Vector &w) const {
	return w.dot(n) > 0 ? emit : Colorf{0};
}
Colorf AreaLight::sample(const Point &p, const std::array<float, 2> &lsample, Vector &w_i,
	float &pdf_val, OcclusionTester &occlusion) const
{
	Normal normal;
	Point ps = geometry->sample(p, lsample, normal);
	w_i = (ps - p).normalized();
	pdf_val = geometry->pdf(p, w_i);
	occlusion.set_points(p, ps);
	return radiance(ps, normal, -w_i);
}
Colorf AreaLight::sample(const Scene&, const std::array<float, 2> &a, const std::array<float, 2> &b,
	Ray &ray, Normal &normal, float &pdf_val) const
{
	Point o = geometry->sample(a, normal);
	Vector d = uniform_sample_sphere(b);
	//Make sure the ray is heading out of the surface
	if (d.dot(normal) < 0){
		d *= -1;
	}
	ray = Ray{o, d, 0.001};
	pdf_val = geometry->pdf(o) * INV_TAU;
	return radiance(o, normal, d);
}
Colorf AreaLight::power(const Scene&) const {
	return emit * surface_area * PI;
}
bool AreaLight::delta_light() const {
	return false;
}
float AreaLight::pdf(const Point &p, const Vector &w_i) const {
	return geometry->pdf(p, w_i);
}

