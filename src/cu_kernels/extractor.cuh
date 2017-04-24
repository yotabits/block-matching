/*
 * extractor.cuh
 *
 *  Created on: Apr 24, 2017
 *      Author: Thomas Kostas
 */

#ifndef EXTRACTOR_CUH_
#define EXTRACTOR_CUH_

void extract_and_replicate(unsigned char *img_gpu, unsigned int size_x, unsigned int size_y, unsigned int roi_size_x_y,
						   unsigned char *output_gpu_buffer, unsigned int y_roi);



#endif /* EXTRACTOR_CUH_ */
