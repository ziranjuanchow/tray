#include <cmath>
#include "material/torrance_sparrow.h"


TorranceSparrow::TorranceSparrow(const Colorf &reflectance, std::unique_ptr<Fresnel> fresnel,
	std::unique_ptr<MicrofacetDistribution> distribution)
	: BxDF(BxDFTYPE(BxDFTYPE::REFLECTION | BxDFTYPE::GLOSSY)), reflectance(reflectance),
	fresnel(std::move(fresnel)), distribution(std::move(distribution))
{}

Colorf TorranceSparrow::operator()(const Vector &w_o, const Vector &w_i) const {
	float cos_to = std::abs(cos_theta(w_o));
	float cos_ti = std::abs(cos_theta(w_i));
	if (cos_to == 0 || cos_ti == 0){
		return Colorf{0};
	}
	Vector w_h = (w_i + w_o).normalized();
	float cos_th = w_i.dot(w_h);
	return reflectance * (*distribution)(w_h) * distribution->geom_atten(w_o, w_i, w_h) * (*fresnel)(cos_th)
		/ (4 * cos_to * cos_ti);
}

