/*
 * image.hh
 *
 *  Created on: Apr 24, 2017
 *      Author: root
 */

#ifndef IMAGE_HH_
#define IMAGE_HH_

typedef struct img
{
	unsigned char *data_cpu;
	unsigned char *data_gpu;
	unsigned int byte_size;
	unsigned int size_x;
	unsigned int size_y;
} img_s;



#endif /* IMAGE_HH_ */
