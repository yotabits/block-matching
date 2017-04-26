/*
 * compare.cuh
 *
 *  Created on: Apr 25, 2017
 *      Author: Thomas Kostas
 */

#ifndef COMPARE_CUH_
#define COMPARE_CUH_

 unsigned int *compute_sad(unsigned char *block_buffer, unsigned int block_size_x_y, unsigned int nb_blocks,
		 unsigned char *block_reference, unsigned int *output);



#endif /* COMPARE_CUH_ */
