
/*
 * compare.cu
 *
 *  Created on: Apr 25, 2017
 *      Author: Thomas Kostas
 */
#include <stdio.h>
#include <stdlib.h>

#include <cuda.h>
#include "extractor.cuh"
#include "../hardware_limits.hh"

//output, size = nb_blocks in buffer 1 sad index per block
__global__ void sad_k(unsigned char *block_buffer, unsigned int block_size_x_y, unsigned int block_buffer_size,
											 unsigned char *block_reference, unsigned int *output)
{
	__shared__ unsigned int sad[1]; //sad will be in position 0
	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int ref_index = tid % (block_size_x_y * block_size_x_y);
	unsigned int output_index = tid / (block_size_x_y* block_size_x_y);
	if (tid < block_buffer_size)
	{
		unsigned int diff = abs(block_buffer[tid] - block_reference[ref_index]);
		block_buffer[tid] = 0;
	    atomicAdd(sad,diff);
	}
	__syncthreads();

	if (threadIdx.x == 0)
	{
		output[output_index] = *sad;
	}
}

 unsigned int *compute_sad(unsigned char *block_buffer, unsigned int block_size_x_y, unsigned int nb_blocks,
		 unsigned char *block_reference, unsigned int *output)
 {
	unsigned int threads = block_size_x_y * block_size_x_y;
	unsigned int blocks;
	unsigned int data_size = block_size_x_y * block_size_x_y * nb_blocks;
	get_blocks(threads, data_size, &blocks);
	//printf("blocks = %i threads %i \n", blocks, threads);
	sad_k<<<blocks, threads>>>(block_buffer, block_size_x_y, data_size, block_reference,output);
	 return output;
 }

