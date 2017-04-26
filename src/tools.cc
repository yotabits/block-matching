
#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>
#include <cuda.h>
#include <iostream>
#include <stdio.h>

using namespace std;

unsigned char *allocate_blocks_gpu_buffer(unsigned int size_x_y_roi, unsigned int size_y_img)
{
	unsigned int byte_size = sizeof(unsigned char) * size_x_y_roi * size_x_y_roi * (size_y_img - size_x_y_roi);
	unsigned char *gpu_buffer;
	printf("hello\n");
	printf("byte_size = %i\n",byte_size);
	if (cudaMalloc(&gpu_buffer,byte_size) == cudaSuccess)
		printf("blocks buffer allocated \n");
	else
		printf("blocks buffer allocation FAILED \n");
	cudaMemset(gpu_buffer,0,byte_size);
	return gpu_buffer;
}

void save_to_raw(unsigned char *gpu_img, char *filename, unsigned int img_byte_size)
{
		//printf("byte_size: %i\n",img_byte_size);
		unsigned char *cpu_img = (unsigned char*) malloc(img_byte_size * sizeof(unsigned char));
		if (cpu_img == NULL)
			printf("cannot allocate host mem \n");

		cudaMemcpy(cpu_img, gpu_img, img_byte_size, cudaMemcpyDeviceToHost);

        FILE *pFile = fopen(filename,"w+b");
        unsigned int element_written = fwrite(cpu_img, img_byte_size, 1,  pFile);
        if (element_written == 1)
        	fclose(pFile);
        else
        	printf("Failed to write img on host hard drive\n");

}

void displayHeader()
{
    const int kb = 1024;
    const int mb = kb * kb;
    wcout << "NBody.GPU" << endl << "=========" << endl << endl;


    int devCount;
    cudaGetDeviceCount(&devCount);
    wcout << "CUDA Devices: " << endl << endl;

    for(int i = 0; i < devCount; ++i)
    {
        cudaDeviceProp props;
        cudaGetDeviceProperties(&props, i);
        wcout << i << ": " << props.name << ": " << props.major << "." << props.minor << endl;
        wcout << "  Global memory:   " << props.totalGlobalMem / mb << "mb" << endl;
        wcout << "  Shared memory:   " << props.sharedMemPerBlock / kb << "kb" << endl;
        wcout << "  Constant memory: " << props.totalConstMem / kb << "kb" << endl;
        wcout << "  Block registers: " << props.regsPerBlock << endl << endl;

        wcout << "  Warp size:         " << props.warpSize << endl;
        wcout << "  Threads per block: " << props.maxThreadsPerBlock << endl;
        wcout << "  Max block dimensions: [ " << props.maxThreadsDim[0] << ", " << props.maxThreadsDim[1]  << ", " << props.maxThreadsDim[2] << " ]" << endl;
        wcout << "  Max grid dimensions:  [ " << props.maxGridSize[0] << ", " << props.maxGridSize[1]  << ", " << props.maxGridSize[2] << " ]" << endl;
        wcout << endl;
    }
}
