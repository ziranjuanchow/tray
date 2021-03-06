#ifndef MATTE_MATERIAL_H
#define MATTE_MATERIAL_H

#include "textures/texture.h"
#include "material.h"

/*
 * A material describing a purely diffuse surface, parameterized by
 * its diffuse color and a roughness value
 */
class MatteMaterial : public Material {
	const Texture *diffuse;
	const float roughness;

public:
	/*
	 * Create the matte material specifying the texture to use for diffuse color
	 * and a scalar roughness value (TODO roughness can be a texture)
	 */
	MatteMaterial(const Texture *diffuse, float roughness);
	/*
	 * Get the BSDF to compute the shading for the material at this
	 * piece of geometry
	 */
	BSDF* get_bsdf(const DifferentialGeometry &dg, MemoryPool &pool) const override;
};

#endif

