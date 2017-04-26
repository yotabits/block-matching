
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "../hardware_limits.hh"
#include <time.h>


__device__ unsigned int get_index_of(unsigned int x, unsigned int y, unsigned int size_x)
{
	unsigned int index = x + y * size_x;
	return index;
}

__global__ void extract_zone_and_replicate_k(unsigned char *img, unsigned char *output,
										   unsigned int size_x, unsigned int size_y, unsigned int size_x_y_roi,
										   unsigned int y_roi)
{
	unsigned int tid = blockIdx.x * blockDim.x + threadIdx.x;
	//unsigned int actual_pos = tid + starting_index;  //position in the image buffer

	//x y corresponding to thread in the input img
	//x y for tid 0 start at roi starting point
	//unsigned int y_img = actual_pos / size_x;
	//unsigned int x_img = actual_pos - (y_img * size_x);
	unsigned int x_base = tid;
    unsigned int y_base = y_roi;
	if (tid < size_x - size_x_y_roi)
	{
		unsigned long output_index = tid * size_x_y_roi * size_x_y_roi;
		for (unsigned int y = y_base; y < y_base + size_x_y_roi; y++)
		{
			for (unsigned int x = x_base; x < x_base + size_x_y_roi; x++)
			{
				output[output_index] = img[x + y * size_x];
				output_index = output_index + 1;
			}
		}
	}
}

void extract_and_replicate(unsigned char *img_gpu, unsigned int size_x, unsigned int size_y, unsigned int roi_size_x_y,
						   unsigned char *output_gpu_buffer, unsigned int y_roi)
{
	unsigned int blocks_to_compare = size_x - roi_size_x_y;
	unsigned int threads = 0;
	unsigned int blocks = 0;
	get_optimized_thread_blocks(&threads,&blocks, blocks_to_compare);
	//printf("threads %i\n", threads);
	//printf("blocks %i \n", blocks);


	extract_zone_and_replicate_k<<<blocks, threads>>>(img_gpu, output_gpu_buffer, size_x, size_y, roi_size_x_y, y_roi);

}



