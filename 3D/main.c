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

// Global data set is float [128][128][128][19]
#define CAVITY (128)
#define Q (19)
#define NODE_SIZE_BYTES (Q*sizeof(float))

// Number of anticipated block (async) on a cluster
#define NB_ASYNC_CUBES (3)

// Dimension of an async block [16][16][16][19]
#define CUBE_LENGTH_X (16)
#define CUBE_LENGTH_Y (16)
#define CUBE_LENGTH_Z (16)

// Number of blocks/cubes in each dimension
#define NB_CUBES_X (CAVITY / CUBE_LENGTH_X)
#define NB_CUBES_Y (CAVITY / CUBE_LENGTH_Y)
#define NB_CUBES_Z (CAVITY / CUBE_LENGTH_Z)

#define NB_CUBES_PER_CLUSTER ((NB_CUBES_X * NB_CUBES_Y * NB_CUBES_Z) / NB_CLUSTER)

#define ASYNC_SEGMENT MPPA_ASYNC_DDR_0

static float async_buffers[NB_ASYNC_CUBES][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X][Q] = {{{{{0.0f}}}}};
static uint64_t wait_time = 0;
static uint64_t get_time = 0;
static uint64_t put_time = 0;
static mppa_async_event_t async_events[NB_ASYNC_CUBES];

static mppa_async_point3d_t local_cube = {
	.xpos = 0,
	.ypos = 0,
	.zpos = 0,
	.xdim = CUBE_LENGTH_X,
	.ydim = CUBE_LENGTH_Y,
	.zdim = CUBE_LENGTH_Z
};

/**
 * This Morton code is inspired from :
 * https://code.activestate.com/recipes/577558-interleave-bits-aka-morton-ize-aka-z-order-curve/
 */
unsigned int part1by2(unsigned int n){
	n&= 0x000003ff;
	n = (n ^ (n << 16)) & 0xff0000ff;
	n = (n ^ (n <<  8)) & 0x0300f00f;
	n = (n ^ (n <<  4)) & 0x030c30c3;
	n = (n ^ (n <<  2)) & 0x09249249;
	return n;
}

unsigned int unpart1by2(unsigned int n){
	n&= 0x09249249;
	n = (n ^ (n >>  2)) & 0x030c30c3;
	n = (n ^ (n >>  4)) & 0x0300f00f;
	n = (n ^ (n >>  8)) & 0xff0000ff;
	n = (n ^ (n >> 16)) & 0x000003ff;
	return n;
}

unsigned int interleave3(unsigned int x, unsigned int y, unsigned int z){
	return part1by2(x) | (part1by2(y) << 1) | (part1by2(z) << 2);
}

void deinterleave3(unsigned int n, unsigned int* x, unsigned int* y, unsigned int* z){
	*x = unpart1by2(n);
	*y = unpart1by2(n >> 1);
	*z = unpart1by2(n >> 2);
}

int get_cube_async(void* local_buffer, off64_t remote_src, const mppa_async_segment_t *segment,
	size_t elem_size,
	unsigned int icubex, unsigned int icubey, unsigned int icubez,
	mppa_async_event_t *event)
{
	// Remote cube position
	mppa_async_point3d_t remote_cube = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sget_block3d(local_buffer, segment, remote_src,
		elem_size, CUBE_LENGTH_X, CUBE_LENGTH_Y, CUBE_LENGTH_Z,
		&local_cube,
		&remote_cube,
		event);
}

int put_cube_async(const void* local_buffer, off64_t remote_src, const mppa_async_segment_t *segment,
	size_t elem_size,
	unsigned int icubex, unsigned int icubey, unsigned int icubez,
	mppa_async_event_t *event)
{
	// Remote cube position
	mppa_async_point3d_t remote_cube = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sput_block3d(local_buffer, segment, remote_src,
		elem_size, CUBE_LENGTH_X, CUBE_LENGTH_Y, CUBE_LENGTH_Z,
		&local_cube,
		&remote_cube,
		event);
}

void init_global_memory()
{
	int cid = __k1_get_cluster_id();
	int status = 0;
	mppa_async_event_t event;

	// Cluster 0 initializes global memory
	if(cid == 0){
		//BD3 ptr_alloc = (float*) mppa_async_global_malloc(ASYNC_SEGMENT, CAVITY*CAVITY*CAVITY*NODE_SIZE_BYTES);
		off64_t offset = 0;
		mppa_async_malloc(ASYNC_SEGMENT, CAVITY*CAVITY*CAVITY*NODE_SIZE_BYTES, &offset, 0);
		ptr_alloc = offset;

		// Repeatly put async_buffers to DDR
		for(unsigned int i = 0; i < (NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z); i++){
			status |= mppa_async_put(&async_buffers[0][0][0][0][0], ASYNC_SEGMENT, offset,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*NODE_SIZE_BYTES, &event);
			offset += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*NODE_SIZE_BYTES;
		}
		// wait for global memory init
		status |= mppa_async_event_wait(&event);
		if(status != 0){ERROR_PRINTF("# [Cluster %2d] : fail to init\n", cid);}

		// Broadcast ptr_alloc to other clusters
		for(int i = 1; i < NB_CLUSTER; i++){
			off64_t offset = 0;
			mppa_async_offset(mppa_async_default_segment(MPPA_ASYNC_SERVER_CC0+i), &ptr_alloc, &offset);
			mppa_async_put((void*)&ptr_alloc, mppa_async_default_segment(MPPA_ASYNC_SERVER_CC0+i),
				offset, sizeof(off64_t), NULL);
		}
	}
	mppa_rpc_barrier_all();
	mOS_dinval();
	printf("# [Cluster %2d] nb_Cluster %2d ptr_alloc 0x%llx size %d\n",
		cid, NB_CLUSTER, (uint64_t) ((unsigned int)ptr_alloc), CAVITY*CAVITY*CAVITY*NODE_SIZE_BYTES);
}

void* task_routine_compute(void *arg)
{
	mOS_dinval();
	int cid = __k1_get_cluster_id();
	int pid = __k1_get_cpu_id();
	int status = 0;

	// Local density distribution
	float f[Q] = {0};

	unsigned int icube_begin, icube_end;
	unsigned int icubex, icubey, icubez;
	unsigned int ibuf = 0;

	// If 0 : get, check to zero, then change to morton
	// If 1 : get, check to morton
	unsigned int compute_type = *((unsigned int*)arg);
	if(compute_type == 0){
		// This is my workspace
		icube_begin = cid*NB_CUBES_PER_CLUSTER;
		icube_end = (cid+1)*NB_CUBES_PER_CLUSTER;
	}else{ // verif
		// To be honest, I will check data of my friend+1.
		// Self-verification is not allowed here
		icube_begin = ((cid+1)%NB_CLUSTER)*NB_CUBES_PER_CLUSTER;
		icube_end = (((cid+1)%NB_CLUSTER) + 1)*NB_CUBES_PER_CLUSTER;
	}

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async cube copies */
	if(pid == 0){
		deinterleave3(icube_begin  , &icubex, &icubey, &icubez);
		uint64_t start = __k1_read_dsu_timestamp();
		status |= get_cube_async(&async_buffers[0][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[0]);
		get_time += (__k1_read_dsu_timestamp() - start);
		deinterleave3(icube_begin+1, &icubex, &icubey, &icubez);
		start = __k1_read_dsu_timestamp();
		status |= get_cube_async(&async_buffers[1][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[1]);
		get_time += (__k1_read_dsu_timestamp() - start);
	}

	/* KERNEL */
	// Macro pipeline through all cubes per cluster
	for(unsigned int icube = icube_begin; icube < icube_end; icube++){

		// everyone deinterleave3(icube) to get the current {icubex, icubey, icubez}
		deinterleave3(icube, &icubex, &icubey, &icubez);

		// PE0 waits for cube's arrival, others PE too
		if (pid == 0) {
			uint64_t start = __k1_read_dsu_timestamp();
			status |= mppa_async_event_wait(&async_events[ibuf]);
			wait_time += (__k1_read_dsu_timestamp() - start);
			// ERROR_PRINTF("Cluster %2d pid %d - wait end for get icube %d - ibuf %d\n", cid, pid, icube, ibuf);
			if (status != 0) {
				ERROR_PRINTF("Cluster %2d - PE %d : task_routine_compute failed to wait for getting cube. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}. \n", cid, pid, ibuf, icubex, icubey, icubez);
			}
		}

		local_barrier()

		// On local_cube[16][16][16][Q], each PE will work on [pid][16][16][Q]
		// Global coordinates and ranges of a PE
		unsigned int z = (icubez * CUBE_LENGTH_Z) + pid; // will not change during the cube compute, but will change between icubez
		unsigned int zl = pid; // coordinates in local cube

		// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
		for(unsigned int yl = 0, y = (icubey * CUBE_LENGTH_Y); yl < CUBE_LENGTH_Y ; yl++, y++){ // also increment global y
			for(unsigned int xl = 0, x = (icubex * CUBE_LENGTH_X); xl < CUBE_LENGTH_X ; xl++, x++){ // also increment global x
				for(unsigned int i = 0; i < Q; i++){
					f[i] = async_buffers[ibuf][zl][yl][xl][i];

					//////////////////////////////////////////////////////////////////////////////////
					// Read zero and change to morton
					if(compute_type == 0){
						if(fabs(f[i]) > EPSILON || f[i] != f[i]) { // if f[i] is not zero or Nan
							ERROR_PRINTF("Error : Cluster %2d pid %2d icube %3d (icubex, icubey, icubez) = (%d, %d, %d) : f[%2d] is %.3f\n",
								cid, pid, icube, icubex, icubey, icubez, i, f[i]);
							diff[pid]++;
						}
						// update value to morton(x,y,z)+i
						f[i] =  (float)(interleave3(x, y, z)+i);
						async_buffers[ibuf][zl][yl][xl][i] = f[i];
					}
					//////////////////////////////////////////////////////////////////////////////////
					// verif morton
					else {
						if(fabs(f[i] - (float)(interleave3(x, y, z)+i)) > EPSILON){
							ERROR_PRINTF("Error : Cluster %2d pid %2d icube %3d (icubex, icubey, icubez) = (%d, %d, %d) : "
								"check morton f[%2d] is %.3f, should be %.3f\n",
								cid, pid, icube, icubex, icubey, icubez, i, f[i], (float)(interleave3(x, y, z)+i));
							diff[pid]++;
						}
					}
					//////////////////////////////////////////////////////////////////////////////////

				}
			} // end for xl
		} // end for yl

		// make a barrier to gather results of local_cube[16][16][16][Q], before PE0 putting back to Dst
		local_barrier()
		if (pid == 0) {
			uint64_t start = __k1_read_dsu_timestamp();
			status |= put_cube_async(&async_buffers[ibuf][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[ibuf]);
			put_time += (__k1_read_dsu_timestamp() - start);
			// ERROR_PRINTF("Cluster %2d pid %d - put      for icube %d - ibuf %d\n", cid, pid, icube, ibuf);
			if (status != 0) {
				ERROR_PRINTF("Cluster %2d - PE %d : task_routine_compute failed to put cube to global memory. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, ibuf, icubex, icubey, icubez);
			}

			// wait for the previous async put of local 'ibuf+2' before enqueueing the next get of remote 'icube+2'
			// into the same buffer, avoid data-racing
			unsigned int next_ibuf = (ibuf+2) % NB_ASYNC_CUBES;

			// This wait is not needed on the first loop, because it is prologue get.
			if(icube > icube_begin){
				uint64_t start = __k1_read_dsu_timestamp();
				status |= mppa_async_event_wait(&async_events[next_ibuf]);
				wait_time += (__k1_read_dsu_timestamp() - start);
				// ERROR_PRINTF("Cluster %2d pid %d - wait end for put icube %d - ibuf %d\n", cid, pid, icube-1, next_ibuf);
				if (status != 0) {
					// May quit here, because what's happening next will be completely false
					ERROR_PRINTF("Cluster %2d - PE %d : task_routine_compute failed to wait for previous put. "
						"next_ibuf = %d\n", cid, pid, next_ibuf);
				}
			}

			// then enqueues next async_get of the remote 'icube+2' (if exists) to the local 'ibuf+2'
			if(icube+2 < icube_end){
				deinterleave3(icube+2, &icubex, &icubey, &icubez);
				uint64_t start = __k1_read_dsu_timestamp();
				status |= get_cube_async(&async_buffers[next_ibuf][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[next_ibuf]);
				get_time += (__k1_read_dsu_timestamp() - start);
				// ERROR_PRINTF("Cluster %2d pid %d - get      for icube %d - ibuf %d\n", cid, pid, icube+2, next_ibuf);
				if (status != 0) {
					ERROR_PRINTF("Cluster %2d - PE %d : task_routine_compute failed to get next cube. "
						"next_ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, next_ibuf, icubex, icubey, icubez);
				}
			}
		}
		// end of a cube update, increment ibuf
		ibuf = (ibuf+1) % NB_ASYNC_CUBES;
	} // end for icube

	/* EPILOGUE : PE0 wait for the last async put */
	if (pid == 0) {
		uint64_t start = __k1_read_dsu_timestamp();
		status |= mppa_async_event_wait(&async_events[(ibuf+2) % NB_ASYNC_CUBES]);
		wait_time += (__k1_read_dsu_timestamp() - start);
		// ERROR_PRINTF("Cluster %2d pid %d - wait end for put last icube - ibuf %d\n", cid, pid, (ibuf+2) % NB_ASYNC_CUBES);
		if (status != 0) {
			ERROR_PRINTF("Cluster %2d - PE %d : task_routine_compute failed to wait for last two puts. "
				"ibuf+2 = %d\n", cid, pid, (ibuf+2) % NB_ASYNC_CUBES);
		}
	}
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

	int fd = open("result_async_3d_buffer.txt", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);

	unsigned int icubex, icubey, icubez;
	unsigned int ibuf = 0;

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async cube copies */
	deinterleave3(0  , &icubex, &icubey, &icubez);
	status |= get_cube_async(&async_buffers[0][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[0]);
	deinterleave3(1, &icubex, &icubey, &icubez);
	status |= get_cube_async(&async_buffers[1][0][0][0][0], ptr_alloc, ASYNC_SEGMENT, NODE_SIZE_BYTES, icubex, icubey, icubez, &async_events[1]);

	/* KERNEL */
	for(unsigned int icube = 0; icube < NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z; icube++){

		// everyone deinterleave3(icube) to get the current {icubex, icubey, icubez}
		deinterleave3(icube, &icubex, &icubey, &icubez);

		// PE0 waits for cube's arrival, others PE too
		status |= mppa_async_event_wait(&async_events[ibuf]);

		// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
		for(unsigned int zl = 0; zl < CUBE_LENGTH_Z ; zl++){
			for(unsigned int yl = 0; yl < CUBE_LENGTH_Y ; yl++){
				char string[20];
				for(unsigned int xl = 0; xl < CUBE_LENGTH_X ; xl++){
					sprintf(string, "%d %s", (unsigned int)async_buffers[ibuf][zl][yl][xl][0], (xl == CUBE_LENGTH_X-1) ? "\n" : "");
					write(fd, string, strlen(string));
				} // end for xl
			} // end for yl
			write(fd, "\n", 1);
		} // end for zl

		// no need to put back
		// status |= put_cube_async(ptr_alloc, &async_buffers[ibuf][0][0][0][0], NODE_SIZE_BYTES, icubex, icubey, icubez, ASYNC_SEGMENT, &async_events[ibuf]);

		// wait for the previous async put of local 'ibuf+2' before enqueueing the next get of remote 'icube+2'
		// into the same buffer, avoid data-racing
		unsigned int next_ibuf = (ibuf+2) % NB_ASYNC_CUBES;

		// // This wait is not needed on the first loop, because it is prologue get.
		// if(icube - icube_begin > 0){
		// 	status |= mppa_async_event_wait(&async_events[next_ibuf]);
		// }

		// then enqueues next async_get of the remote 'icube+2' (if exists) to the local 'ibuf+2'
		if(icube+2 < NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z){
			deinterleave3(icube+2, &icubex, &icubey, &icubez);
			status |= get_cube_async(&async_buffers[next_ibuf][0][0][0][0], ptr_alloc, NODE_SIZE_BYTES, icubex, icubey, icubez, ASYNC_SEGMENT, &async_events[next_ibuf]);
		}
		// end of a cube update, increment ibuf
		ibuf = (ibuf+1) % NB_ASYNC_CUBES;
	} // end for icube

	/* EPILOGUE : PE0 wait for the last async put */
	// status |= mppa_async_event_wait(&async_events[(ibuf+2) % NB_ASYNC_CUBES]);

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
	float bw_d = 4.0f*(float)(NB_CUBES_PER_CLUSTER*CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*NODE_SIZE_BYTES);
	float bw_c = ((float)bw_d) / (compute_time / 1000.0f) / 1000000000.0f;
	float bw_g = ((float)bw_d) / (compute_time / 1000.0f) / 1000000000.0f * NB_CLUSTER;
	int nb_request = 4 * NB_CUBES_PER_CLUSTER * CUBE_LENGTH_Z;
	int nb_cycle_per_req = (int)((float)((float)CHIP_FREQ * (float)compute_time / (float)nb_request));
	int total_diff = 0;
	for(unsigned int i=0;i<N_CORES;i++) {total_diff += diff[i];}

	printf("Cluster %2d init %.1f ms compute %.1f ms wait %.1f ms get %.1f ms put %.1f ms Total %.1f ms - "
		"bwc %.3f GB/s bwg %.3f GB/s nb_request %d  %d cycles/req diff %d => %s\n",
		cid, init_time, compute_time, (float)(((float) wait_time) / CHIP_FREQ),
		(float)(((float) get_time) / CHIP_FREQ), (float)(((float) put_time) / CHIP_FREQ), total_time,
		bw_c, bw_g, nb_request, nb_cycle_per_req, total_diff, (total_diff != 0 ? "FAILED" : "SUCCESS")
	);

	// should assert or return ?
	// assert(total_diff == 0);
	// Clean up
	if(cid == 0){
		mppa_async_free(ASYNC_SEGMENT, ptr_alloc, NULL);
	}

	mppa_async_final();
	return /*0*/ total_diff;
}
