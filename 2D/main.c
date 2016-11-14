/*
 * Copyright (C) 2013 Kalray SA. All rights reserved.
 * This code is Kalray proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of Kalray SA.
 */

#include "mOS_common_types_c.h"
#include "mOS_vcore_u.h"
#include "mOS_segment_manager_u.h"
#include "stdlib.h"
#include "stdio.h"
#include "vbsp.h"
#include "utask.h"
#include <HAL/hal/core/cpu.h>
#include <HAL/hal/core/mp.h>
#include <math.h>
#include <stdlib.h>
#include <mppa_power.h>
#include <mppa_async.h>
#include <mppa_remote.h>
#include <vbsp.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/_default_fcntl.h>
#include <assert.h>

#define DEBUG 1

#ifdef DEBUG
 #define ERROR_PRINTF(...) printf(__VA_ARGS__)
#else
 #define ERROR_PRINTF(...)
#endif

#if 1
#define CHECK_ERROR_PRINTF(...) printf(__VA_ARGS__)
#else
#define CHECK_ERROR_PRINTF(...)
#endif

// #define DUMP_FILE 1

static off64_t ptr_alloc;

#define EPSILON 0.0000000001f

#define N_CORES 16

#ifndef NB_CLUSTER
	#define NB_CLUSTER 1
#endif

#ifndef N_CORES
	#define N_CORES 16
#endif
#define NB_CLIENTS (N_CORES-1)

static int diff[N_CORES] = { 0 };
static bsp_barrier_t barrier_0;

#if (NB_CLIENTS > 0)
#define local_barrier() __builtin_k1_wpurge(); \
				__builtin_k1_fence(); \
				bsp_barrier(&barrier_0); \
				mOS_dinval();
#else
#define local_barrier()
#endif

// Global data set is float [CAVITY][CAVITY][Q]
#ifdef DUMP_FILE
 #define CAVITY (128)
#else
 #define CAVITY (8192)
#endif

#define Q (3)
#define PIXEL_SIZE_BYTES (Q*sizeof(float))

// Number of anticipated block (async) on a cluster
#define NB_ASYNC_IMAGES (3)

// Dimension of an async local image [IMAGE_LENGTH_X][IMAGE_LENGTH_Y][Q]
#ifdef DUMP_FILE
 #define IMAGE_LENGTH_X (16)
 #define IMAGE_LENGTH_Y (16)
#else
 #define IMAGE_LENGTH_X (128)
 #define IMAGE_LENGTH_Y (128)
#endif

// Number of blocks/images in each dimension
#define NB_IMAGES_X (CAVITY / IMAGE_LENGTH_X)
#define NB_IMAGES_Y (CAVITY / IMAGE_LENGTH_Y)

#define NB_IMAGES_PER_CLUSTER ((NB_IMAGES_X * NB_IMAGES_Y) / NB_CLUSTER)

#define ASYNC_SEGMENT (mppa_async_default_segment(MPPA_ASYNC_SERVER_DDR0))

static float async_buffers[NB_ASYNC_IMAGES][IMAGE_LENGTH_Y][IMAGE_LENGTH_X][Q] = {{{{0.0f}}}};
static uint64_t wait_time = 0;
static uint64_t get_time = 0;
static uint64_t put_time = 0;
static mppa_async_event_t async_events[NB_ASYNC_IMAGES];

static mppa_async_point2d_t local_image = {
	.xpos = 0,
	.ypos = 0,
	.xdim = IMAGE_LENGTH_X,
	.ydim = IMAGE_LENGTH_Y
};

/**
 * This Morton code is inspired from :
 * https://code.activestate.com/recipes/577558-interleave-bits-aka-morton-ize-aka-z-order-curve/
 */
unsigned int part1by1(unsigned int n){
	n&= 0x0000ffff;
	n = (n | (n << 8)) & 0x00FF00FF;
	n = (n | (n << 4)) & 0x0F0F0F0F;
	n = (n | (n << 2)) & 0x33333333;
	n = (n | (n << 1)) & 0x55555555;
	return n;
}

unsigned int unpart1by1(unsigned int n){
	n&= 0x55555555;
	n = (n ^ (n >> 1)) & 0x33333333;
	n = (n ^ (n >> 2)) & 0x0f0f0f0f;
	n = (n ^ (n >> 4)) & 0x00ff00ff;
	n = (n ^ (n >> 8)) & 0x0000ffff;
	return n;
}

unsigned int interleave2(unsigned int x, unsigned int y){
	return part1by1(x) | (part1by1(y) << 1);
}

void deinterleave2(unsigned int n, unsigned int* x, unsigned int* y){
	*x = unpart1by1(n);
	*y = unpart1by1(n >> 1);
}

int get_image_async(void* local_buffer, const off64_t remote_src,
	const mppa_async_segment_t *segment,
	size_t elem_size,
	unsigned int iimagex, unsigned int iimagey,
	mppa_async_event_t *event)
{
	// Remote image position
	mppa_async_point2d_t remote_image = {
		.xpos = (iimagex * IMAGE_LENGTH_X),
		.ypos = (iimagey * IMAGE_LENGTH_Y),
		.xdim = CAVITY,
		.ydim = CAVITY
	};

	return mppa_async_sget_block2d(local_buffer, segment, (uintptr_t)remote_src,
		elem_size, IMAGE_LENGTH_X, IMAGE_LENGTH_Y,
		&local_image, &remote_image,
		event);
}

int put_image_async(void* local_buffer, const off64_t remote_src,
	const mppa_async_segment_t *segment,
	size_t elem_size,
	unsigned int iimagex, unsigned int iimagey,
	mppa_async_event_t *event)
{
	// Remote image position
	mppa_async_point2d_t remote_image = {
		.xpos = (iimagex * IMAGE_LENGTH_X),
		.ypos = (iimagey * IMAGE_LENGTH_Y),
		.xdim = CAVITY,
		.ydim = CAVITY
	};

	return mppa_async_sput_block2d(local_buffer, segment, (uintptr_t)remote_src,
		elem_size, IMAGE_LENGTH_X, IMAGE_LENGTH_Y,
		&local_image, &remote_image,
		event);
}

void init_global_memory()
{
	int cid = __k1_get_cluster_id();
	int status = 0;
	mppa_async_event_t event;

	// Cluster 0 initializes global memory
	if(cid == 0){

		//BD3 mppa_async_global_posix_memalign(ASYNC_SEGMENT, (void**)&ptr_alloc, 1<<13, CAVITY*CAVITY*PIXEL_SIZE_BYTES);
		off64_t offset = 0;
		mppa_async_memalign(ASYNC_SEGMENT, 1<<13, CAVITY*CAVITY*PIXEL_SIZE_BYTES, &offset, 0);
		ptr_alloc = offset;

		printf("Cluster 0 init DDR\n");
		// Repeatly put async_buffers to DDR
		for(unsigned int i = 0; i < (NB_IMAGES_X*NB_IMAGES_Y); i++){
			status |= mppa_async_put(&async_buffers[0][0][0][0], ASYNC_SEGMENT, offset,
				IMAGE_LENGTH_X*IMAGE_LENGTH_Y*PIXEL_SIZE_BYTES, &event);
			offset += IMAGE_LENGTH_X*IMAGE_LENGTH_Y*PIXEL_SIZE_BYTES;
		}
		// wait for global memory init
		status |= mppa_async_event_wait(&event);
		if(status != 0){ERROR_PRINTF("# [Cluster %d] : fail to init\n", cid);}

		// Broadcast ptr_alloc to other clusters
		for(unsigned int i = 0; i < NB_CLUSTER-1; i++){
			status |= mppa_async_put((void*)&ptr_alloc, mppa_async_default_segment((int)(MPPA_ASYNC_SERVER_CC1 + i)), (uintptr_t)&ptr_alloc, sizeof(void*), NULL);
		}
		printf("Cluster 0 init DDR done !\n");
	}
	mppa_rpc_barrier_all();
	mOS_dinval();
	printf("# [Cluster %d] nb_cluster %d ptr_alloc 0x%llx size %d\n",
		cid, NB_CLUSTER, (uint64_t) ((unsigned int)ptr_alloc), CAVITY*CAVITY*PIXEL_SIZE_BYTES);
}

void* task_routine_compute(void *arg)
{
	mOS_dinval();
	int cid = __k1_get_cluster_id();
	int pid = __k1_get_cpu_id();
	int status = 0;

	// Local density distribution
	float f[Q] = {0};

	unsigned int iimage_begin, iimage_end;
	unsigned int iimagex, iimagey;
	unsigned int ibuf = 0;

	// If 0 : get, check to zero, then change to morton
	// If 1 : get, check to morton
	unsigned int compute_type = *((unsigned int*)arg);
	if(compute_type == 0){
		// This is my workspace
		iimage_begin = cid*NB_IMAGES_PER_CLUSTER;
		iimage_end = (cid+1)*NB_IMAGES_PER_CLUSTER;
	}else{ // verif
		// To be honest, I will check data of my friend+1.
		// Self-verification is not allowed here
		iimage_begin = ((cid+1)%NB_CLUSTER)*NB_IMAGES_PER_CLUSTER;
		iimage_end = (((cid+1)%NB_CLUSTER) + 1)*NB_IMAGES_PER_CLUSTER;
	}

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async image copies */
	if(pid == 0){
		deinterleave2(iimage_begin  , &iimagex, &iimagey);
		uint64_t start = __k1_read_dsu_timestamp();
		status |= get_image_async(&async_buffers[0][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[0]);
		get_time += (__k1_read_dsu_timestamp() - start);
		deinterleave2(iimage_begin+1, &iimagex, &iimagey);
		start = __k1_read_dsu_timestamp();
		status |= get_image_async(&async_buffers[1][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[1]);
		get_time += (__k1_read_dsu_timestamp() - start);
	}
#if 1
	/* KERNEL */
	// Macro pipeline through all images per cluster
	for(unsigned int iimage = iimage_begin; iimage < iimage_end; iimage++){

		// everyone deinterleave2(iimage) to get the current {iimagex, iimagey}
		deinterleave2(iimage, &iimagex, &iimagey);

		// PE0 waits for image's arrival, others PE too
		if (pid == 0) {
			uint64_t start = __k1_read_dsu_timestamp();
			status |= mppa_async_event_wait(&async_events[ibuf]);
			wait_time += (__k1_read_dsu_timestamp() - start);
			// ERROR_PRINTF("Cluster %d pid %d - wait end for get iimage %d - ibuf %d\n", cid, pid, iimage, ibuf);
			if (status != 0) {
				ERROR_PRINTF("Cluster %d - PE %d : task_routine_compute failed to wait for getting image. "
					"ibuf = %d - {iimagex,iimagey} = {%d, %d}. \n", cid, pid, ibuf, iimagex, iimagey);
			}
			//printf("C%d PE%d data ok !\n", cid, pid);
		}

		local_barrier()

		for(unsigned int yl = pid*(IMAGE_LENGTH_Y/N_CORES), y = yl + (iimagey*IMAGE_LENGTH_Y); yl < (pid+1)*(IMAGE_LENGTH_Y/N_CORES); yl++, y++){
			for(unsigned int xl = 0, x = iimagex * IMAGE_LENGTH_X; xl < IMAGE_LENGTH_X ; xl++, x++){

				for(unsigned int i = 0; i < Q; i++){
					f[i] = async_buffers[ibuf][yl][xl][i];

					//////////////////////////////////////////////////////////////////////////////////
					// Read zero and change to morton
					if(compute_type == 0){
						if(fabs(f[i]) > EPSILON || f[i] != f[i]) { // if f[i] is zero or Nan
							CHECK_ERROR_PRINTF("Error : Cluster %2d pid %2d iimage %3d (iimagex, iimagey) = (%d, %d) : f[%2d] is %.3f\n",
								cid, pid, iimage, iimagex, iimagey, i, f[i]);
							diff[pid]++;
						}
						// update value to morton(x,y)+i
						f[i] =  (float)(interleave2(x, y)+i);
						async_buffers[ibuf][yl][xl][i] = f[i];
					}
					//////////////////////////////////////////////////////////////////////////////////
					// verif morton
					else {
						if(fabs(f[i] - (float)(interleave2(x, y)+i)) > EPSILON){
							CHECK_ERROR_PRINTF("Error : Cluster %2d pid %2d iimage %3d (iimagex, iimagey) = (%d, %d) : "
								"check morton f[%2d] is %.3f, should be %.3f\n",
								cid, pid, iimage, iimagex, iimagey, i, f[i], (float)(interleave2(x, y)+i));
							diff[pid]++;
						}
					}
					//////////////////////////////////////////////////////////////////////////////////

				}
			} // end for xl
		} // end for yl

		// make a barrier to gather results of local_image[16][16][16][Q], before PE0 putting back to Dst
		local_barrier()
		if (pid == 0) {
			uint64_t start = __k1_read_dsu_timestamp();
			//printf("ptr_alloc %llx\n", (uint64_t) (uintptr_t)ptr_alloc );
			status |= put_image_async(&async_buffers[ibuf][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[ibuf]);
			put_time += (__k1_read_dsu_timestamp() - start);
			// ERROR_PRINTF("Cluster %d pid %d - put      for iimage %d - ibuf %d\n", cid, pid, iimage, ibuf);
			if (status != 0) {
				ERROR_PRINTF("Cluster %d - PE %d : task_routine_compute failed to put image to global memory. "
					"ibuf = %d - {iimagex,iimagey} = {%d, %d}\n", cid, pid, ibuf, iimagex, iimagey);
			}

			// wait for the previous async put of local 'ibuf+2' before enqueueing the next get of remote 'iimage+2'
			// into the same buffer, avoid data-racing
			unsigned int next_ibuf = (ibuf+2) % NB_ASYNC_IMAGES;

			// This wait is not needed on the first loop, because it is prologue get.
			if(iimage - iimage_begin > 0){
				uint64_t start = __k1_read_dsu_timestamp();
				status |= mppa_async_event_wait(&async_events[next_ibuf]);
				wait_time += (__k1_read_dsu_timestamp() - start);
				// ERROR_PRINTF("Cluster %d pid %d - wait end for put iimage %d - ibuf %d\n", cid, pid, iimage-1, next_ibuf);
				if (status != 0) {
					// May quit here, because what's happening next will be completely false
					ERROR_PRINTF("Cluster %d - PE %d : task_routine_compute failed to wait for previous put. "
						"next_ibuf = %d\n", cid, pid, next_ibuf);
				}
			}

			// then enqueues next async_get of the remote 'iimage+2' (if exists) to the local 'ibuf+2'
			if(iimage+2 < iimage_end){
				deinterleave2(iimage+2, &iimagex, &iimagey);
				uint64_t start = __k1_read_dsu_timestamp();
				status |= get_image_async(&async_buffers[next_ibuf][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[next_ibuf]);
				get_time += (__k1_read_dsu_timestamp() - start);
				// ERROR_PRINTF("Cluster %d pid %d - get      for iimage %d - ibuf %d\n", cid, pid, iimage+2, next_ibuf);
				if (status != 0) {
					ERROR_PRINTF("Cluster %d - PE %d : task_routine_compute failed to get next image. "
						"next_ibuf = %d - {iimagex,iimagey} = {%d, %d}\n", cid, pid, next_ibuf, iimagex, iimagey);
				}
			}
		}
		// end of a image update, increment ibuf
		ibuf = (ibuf+1) % NB_ASYNC_IMAGES;
	} // end for iimage

	/* EPILOGUE : PE0 wait for the last async put */
	if (pid == 0) {
		uint64_t start = __k1_read_dsu_timestamp();
		status |= mppa_async_event_wait(&async_events[(ibuf+2) % NB_ASYNC_IMAGES]);
		wait_time += (__k1_read_dsu_timestamp() - start);
		// ERROR_PRINTF("Cluster %d pid %d - wait end for put last iimage - ibuf %d\n", cid, pid, (ibuf+2) % NB_ASYNC_IMAGES);
		if (status != 0) {
			ERROR_PRINTF("Cluster %d - PE %d : task_routine_compute failed to wait for last two puts. "
				"ibuf+2 = %d\n", cid, pid, (ibuf+2) % NB_ASYNC_IMAGES);
		}
	}
#endif
	return NULL;
}

void compute(unsigned int compute_type)
{
	utask_t utask[NB_CLIENTS];
	for (unsigned int i = 0; i < NB_CLIENTS; i++) {
		utask_create(&utask[i], NULL, task_routine_compute, (void*)&compute_type);
	}

	// start routine on self.
	task_routine_compute((void*)&compute_type);

	// End of a timestep, join other threads
	for (unsigned int i = 0; i < NB_CLIENTS; i++) {
		utask_join(utask[i], NULL);
	}
}

#ifdef DUMP_FILE
void dump_file()
{
	mOS_dinval();
	int status = 0;

	int fd = open("result_async_2d_buffer.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

	unsigned int iimagex, iimagey;
	unsigned int ibuf = 0;

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async image copies */
	deinterleave2(0  , &iimagex, &iimagey);
	status |= get_image_async(&async_buffers[0][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[0]);
	deinterleave2(1, &iimagex, &iimagey);
	status |= get_image_async(&async_buffers[1][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[1]);

	/* KERNEL */
	for(unsigned int iimage = 0; iimage < NB_IMAGES_X*NB_IMAGES_Y; iimage++){
		deinterleave2(iimage, &iimagex, &iimagey);
		status |= mppa_async_event_wait(&async_events[ibuf]);

		for(unsigned int yl = 0; yl < IMAGE_LENGTH_Y ; yl++){
			char string[20];
			for(unsigned int xl = 0; xl < IMAGE_LENGTH_X ; xl++){
				sprintf(string, "%4d %s", (unsigned int)async_buffers[ibuf][yl][xl][0], (xl == IMAGE_LENGTH_X-1) ? "\n" : "");
				write(fd, string, strlen(string));
			} // end for xl
		} // end for yl
		write(fd, "\n", 1);

		unsigned int next_ibuf = (ibuf+2) % NB_ASYNC_IMAGES;

		// then enqueues next async_get of the remote 'iimage+2' (if exists) to the local 'ibuf+2'
		if(iimage+2 < NB_IMAGES_X*NB_IMAGES_Y){
			deinterleave2(iimage+2, &iimagex, &iimagey);
			status |= get_image_async(&async_buffers[next_ibuf][0][0][0], ptr_alloc, ASYNC_SEGMENT, PIXEL_SIZE_BYTES, iimagex, iimagey, &async_events[next_ibuf]);
		}
		// end of a image update, increment ibuf
		ibuf = (ibuf+1) % NB_ASYNC_IMAGES;
	} // end for iimage

	/* EPILOGUE : PE0 wait for the last async put */
	// status |= mppa_async_event_wait(&async_events[(ibuf+2) % NB_ASYNC_IMAGES]);
	close(fd);
}
#endif

int main(void)
{
	int cid = __k1_get_cluster_id();

	mppa_rpc_client_init();
	mppa_async_init();
	mppa_remote_client_init();

	bsp_barrier_init(&barrier_0, BSP_BARRIER_USER);

	mppa_rpc_barrier_all();

	// Compute
	uint64_t start = __k1_read_dsu_timestamp();

	// Init
	init_global_memory();

	mppa_rpc_barrier_all();

	uint64_t middle = __k1_read_dsu_timestamp();

	compute(0); // get, check for zero, then change value, put
	compute(1); // get, check for correct value, put

	uint64_t end = __k1_read_dsu_timestamp();

	mppa_rpc_barrier_all();

#ifdef DUMP_FILE
	dump_file();
#endif

#define CHIP_FREQ (__bsp_frequency/1000)
	float init_time = (float) ((middle - start)/CHIP_FREQ);
	float compute_time = (float) ((end - middle)/CHIP_FREQ);
	float total_time = (float) ((end - start)/CHIP_FREQ);
	float bw_d = 4.0f*(float)(NB_IMAGES_PER_CLUSTER*IMAGE_LENGTH_X*IMAGE_LENGTH_Y*PIXEL_SIZE_BYTES);
	float bw_c = ((float)bw_d) / (compute_time / 1000.0f) / 1000000000.0f;
	float bw_g = ((float)bw_d) / (compute_time / 1000.0f) / 1000000000.0f * NB_CLUSTER;
	int nb_request = 4 * NB_IMAGES_PER_CLUSTER;
	int nb_cycle_per_req = (int)((float)((float)CHIP_FREQ * (float)compute_time / (float)nb_request));
	int total_diff = 0;
	for(unsigned int i=0;i<N_CORES;i++) {total_diff += diff[i];}

	printf("Cluster %d init %.1f ms compute %.1f ms wait %.1f ms get %.1f ms put %.1f ms Total %.1f ms - "
		"bwc %.3f GB/s bwg %.3f GB/s nb_request %d  %d cycles/req diff %d => %s\n",
		cid, init_time, compute_time, (float)(((float) wait_time) / CHIP_FREQ),
		(float)(((float) get_time) / CHIP_FREQ), (float)(((float) put_time) / CHIP_FREQ), total_time,
		bw_c, bw_g, nb_request, nb_cycle_per_req, total_diff, (total_diff != 0 ? "FAILED" : "SUCCESS")
	);

	// assert or return ?
	// assert(total_diff == 0);
	// Clean up
	mppa_async_final();
	return /*0*/ total_diff;
}
