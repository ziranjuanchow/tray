#ifndef PBR_LIGHT_H
#define PBR_LIGHT_H

#include <array>
#include "linalg/transform.h"
#include "film/color.h"
#include "occlusion_tester.h"
#include "scene.h"

/*
 * TODO: Replace Light with this
 * Base class for lights, stores the transformation from world space to light space
 * and the number of samples that should be taken when sampling this light
 */
class PBRLight {
protected:
	//Transforms from light to world space and world to light space
	const Transform to_world, to_light;
	const int n_samples;

public:
	/*
	 * Create the light with some transformation to world space
	 * and desired number of samples to be taken
	 */
	PBRLight(const Transform &to_world, int n_samples = 1);
	/*
	 * Sample the illumination from the light arriving at the point
	 * returns the color along with the incident light direction for the point,
	 * the PDF of the point that was sampled and fills out the occlusion tester
	 * for shadow testing if the color returned wasn't black
	 */
	virtual Colorf sample(const Point &p, const std::array<float, 2> &lsample,
		Vector &wi, float &pdf_val, OcclusionTester &occlusion) const = 0;
	/*
	 * Compute the total power emitted by the light in the scene
	 */
	virtual Colorf power(const Scene &scene) const = 0;
	/*
	 * Check if this light is a delta light, eg. a point or directional light
	 */
	virtual bool delta_light() const = 0;
	/*
	 * Compute the PDF for sampling the point with the incident direction wi
	 */
	virtual float pdf(const Point &p, const Vector &wi) const = 0;
};

#endif

