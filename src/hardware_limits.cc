// File : hardware_limits.cc
// Object :
// Modification(s) :
//
/*
 * hardware_limits.cc
 *
 *  Created on: Apr 24, 2017
 *      Author: Thomas Kostas
 */

#include "hardware_limits.hh"

#include <stdio.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <cmath>

unsigned int get_max_threads_1d()
{
  static int max_threads_per_block_1d;

  static bool initialized = false;

  if (!initialized)
  {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    max_threads_per_block_1d = prop.maxThreadsPerBlock;
    initialized = true;
  }

  return max_threads_per_block_1d;
}

unsigned int get_max_threads_2d()
{
  static int max_threads_per_block_2d;
  static bool initialized = false;

  if (!initialized)
  {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    max_threads_per_block_2d = static_cast<unsigned int>(sqrt(prop.maxThreadsPerBlock));
    initialized = true;
  }

  return max_threads_per_block_2d;
}

unsigned int get_max_blocks()
{
  static int max_blocks;
  static bool initialized = false;

  if (!initialized)
  {
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop, 0);
    max_blocks = prop.maxGridSize[0];
    initialized = true;
  }

  return max_blocks;
}

void get_threads_blocks(unsigned int *threads, unsigned int *blocks, unsigned int data_size )
{
	*threads = get_max_threads_1d();
	*blocks = (data_size + *threads - 1) / *threads;
}

unsigned int get_warp_size()
{
	  static int warp_size;
	  static bool initialized = false;
	  if (!initialized)
	  {
	    cudaDeviceProp prop;
	    cudaGetDeviceProperties(&prop, 0);
	    warp_size = prop.warpSize;
	    initialized = true;
	  }

	  return warp_size;
}

void get_optimized_thread_blocks(unsigned int *threads, unsigned int *blocks, unsigned int data_size)
{
	*threads = get_warp_size();
	*blocks = (data_size + *threads - 1) / *threads;
	if (*blocks > get_max_blocks())
	{
		//tunning should happen instead of that
		printf("Optimized_launch failed\n");
		get_threads_blocks(threads , blocks, data_size);
	}
}


void get_blocks(unsigned int threads, unsigned int data_size, unsigned int *blocks)
{
	//check if tid > max_tid --> tunning
	*blocks = (data_size / threads) + 1; //dirty
}
