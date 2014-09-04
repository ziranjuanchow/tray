#include "material/blinn_phong.h"

BlinnPhong::BlinnPhong(const Colorf &diffuse, const Colorf &specular, float gloss)
	: diffuse(diffuse), specular(specular), gloss(gloss)
{}
Colorf BlinnPhong::shade(const Ray &r, const HitInfo &hitinfo) const {
	//TODO: BlinnPhong
	return diffuse;
}
