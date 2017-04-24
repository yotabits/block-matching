/*
 * tools.hh
 *
 *  Created on: Apr 24, 2017
 *      Author: Thomas Kostas
 */

#ifndef TOOLS_HH_
#define TOOLS_HH_

void save_to_raw(unsigned char *gpu_img, char *filename, unsigned int img_byte_size);
void displayHeader();
unsigned char *allocate_blocks_gpu_buffer(unsigned int size_x_y_roi, unsigned int size_y_img);

#endif /* TOOLS_HH_ */
