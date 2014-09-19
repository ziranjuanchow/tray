#ifndef STRATIFIED_SAMPLER_H
#define STRATIFIED_SAMPLER_H

#include <random>
#include <memory>
#include <array>
#include <vector>
#include "sampler.h"

/*
 * A stratified sampler, generates multipled jittered
 * samples per pixel in its sample region
 */
class StratifiedSampler : public Sampler {
	//Samples per pixel
	const int spp;

public:
	StratifiedSampler(int x_start, int x_end, int y_start, int y_end, int spp);
	/*
	 * Get some {x, y} positions to sample in the space being sampled
	 * If the sampler has finished sampling samples will be empty
	 */
	void get_samples(std::vector<std::array<float, 2>> &samples) override;
	/*
	 * Get subsamplers that divide the space to be sampled
	 * into count disjoint subsections where each samples a w x h
	 * section of the original sampler
	 */
	std::vector<std::unique_ptr<Sampler>> get_subsamplers(int w, int h) const override;

private:
	/*
	 * Generate a 2d pattern of stratified samples and return them
	 * vector returned will contain [{x, y}, ...] sample positions
	 * normalized between [0, 1)
	 */
	std::vector<float> sample2d(int nx, int ny, std::mt19937 &rng);
};

#endif

