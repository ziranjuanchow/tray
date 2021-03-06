#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <chrono>
#include <memory>
#include <atomic>
#include "samplers/sampler.h"

/*
 * A queue that hands out blocks of pixels to be worked on
 * by threads in the form of samplers for the block of pixels
 */
class BlockQueue {
	std::vector<std::unique_ptr<Sampler>> samplers;
	//The index of the next sampler to be handed out
	std::atomic_uint sampler_idx, loops;
	//Total time and previous time we printed out timing info
	std::chrono::milliseconds total_time;
	std::chrono::time_point<std::chrono::high_resolution_clock> prev;

public:
	/*
	 * Create a queue of work blocks by subsampling the sampler
	 * into blocks subsamplers
	 */
	BlockQueue(const Sampler &sampler, int bwidth, int bheight);
	/*
	 * Return the next block to be worked on, returns nullptrt
	 * when all samplers have been completed
	 */
	Sampler* get_block();
};

#endif

