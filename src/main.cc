
#include <stdio.h>
#include <stdlib.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include "image.hh"
#include "CImg.h"
#include "tools.hh"
#include "cu_kernels/extractor.cuh"
#include "cu_kernels/compare.cuh"
using namespace cimg_library;


img_s load_img(char *filename)
{
	img_s img;
	unsigned int buffer_index = 0;
	CImg<unsigned char> image(filename);
	unsigned int size_x = image.width();
	unsigned int size_y = image.height();
	img.size_x = size_x;
	img.size_y = size_y;
	unsigned int img_byte_size = size_x * size_y * sizeof(unsigned char);
	img.byte_size = img_byte_size;

	printf("img: size_x = %i, size_y = %i\n",img.size_x , img.size_y);
	//let's allocate some page locked memory
	cudaMallocHost(&(img.data_cpu), img_byte_size);
	cudaMalloc(&(img.data_gpu), img_byte_size);

	//we prepare data for gpu, this preparation time has no reason to be taken
	//into account for benchmarking as we assume receiving an already grey scaled
	// unsigned char buffer of pixels organised as following
	for (unsigned int y = 0; y < size_y; y++)
		for (unsigned int x = 0; x < size_x; x++)
		{
			//////////TESTING PURPOSE///////////
			unsigned int r = (unsigned int)image(x,y,0,0);
			unsigned int g = (unsigned int)image(x,y,0,1);
			unsigned int b = (unsigned int)image(x,y,0,2);
			unsigned char grey_value = (unsigned char)((r + g + b)/3);
			image(x,y,0,0) = grey_value;
			image(x,y,0,1) = grey_value;
			image(x,y,0,2) = grey_value;
			//////////TESTING PURPOSE///////////

			img.data_cpu[buffer_index] = grey_value;
			buffer_index++;
		}


	//for test purpose
	/*  CImgDisplay main_disp(image,"Click a point");
	  while (!main_disp.is_closed()) {
	    main_disp.wait();
	    if (main_disp.button() && main_disp.mouse_y()>=0) {
	      const int y = main_disp.mouse_y();
	      }
	    }*/
	  return img;
}

int main(int argc, char **argv)
{
	unsigned int size_x_y_roi = 20;

	//Init
	img_s img = load_img("left.png");
	unsigned int nb_blocks = img.size_y - size_x_y_roi;

	unsigned char *block_reference; // Dummy, won't have impact on perfs in reality (small data transfer small cpu to extract)
	cudaMalloc(&block_reference, sizeof(unsigned char) * size_x_y_roi * size_x_y_roi);
	cudaMemset(&block_reference, 0, size_x_y_roi * size_x_y_roi);

	unsigned char *block_gpu_buffer = allocate_blocks_gpu_buffer(size_x_y_roi, img.size_y);
	unsigned int *output;
	cudaMalloc(&output, nb_blocks * sizeof(unsigned int));
	printf("img: size_x = %i, size_y = %i\n",img.size_x , img.size_y);

	//Performance TEST//

	clock_t begin = clock();
	unsigned int i = 0;
	while (true)
	{
		cudaMemcpy((img.data_gpu), (img.data_cpu), img.byte_size, cudaMemcpyHostToDevice);
		//save_to_raw(img.data_gpu,"test.raw",img.byte_size); //remove for perf test
		extract_and_replicate(img.data_gpu,img.size_x, img.size_y, size_x_y_roi,block_gpu_buffer, 100);
		cudaDeviceSynchronize();
	    compute_sad(block_gpu_buffer,size_x_y_roi, nb_blocks,block_reference,output);
		cudaDeviceSynchronize();
		//save_to_raw(block_gpu_buffer,"block_black.raw",sizeof(unsigned char) * size_x_y_roi * size_x_y_roi * (img.size_y - size_x_y_roi));
	    clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		i++;
		if (time_spent >= 1)
		{
			begin = clock();
			printf("one sec %i\n", i);
			i = 0;
		}
	}


	//save_to_raw(block_gpu_buffer,"block.raw",sizeof(unsigned char) * size_x_y_roi * size_x_y_roi * (img.size_y - size_x_y_roi));

	return 0;
}
