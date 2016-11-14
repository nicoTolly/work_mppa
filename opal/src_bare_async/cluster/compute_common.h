
#ifndef _COMPUTE_COMMON_H
#define _COMPUTE_COMMON_H

#include <mppa_async.h>
#include "global.h"

// to use double DDR
#if (NB_DDR == 2)
  #define DUPLEX
#endif

#define TIME_PROFILING 1
// #define DUMP_DELTA 1
// #define DUMP_DENSITY 1

#define min(x, y) (x < y ? x : y)

#define CHECK_ERROR(status, fmt, ...) if(status != 0) {printf(fmt, ## __VA_ARGS__);}

#ifdef TIME_PROFILING
#define ACC_TIME_FUNC(time_acc, ...)                                   \
do {                                                                   \
	if(pid == 0) {start = __k1_read_dsu_timestamp();}                   \
	__VA_ARGS__ ;                                                       \
	if(pid == 0) {time_acc += (__k1_read_dsu_timestamp() - start);}     \
} while(0)
#else
#define ACC_TIME_FUNC(time_acc, ...)  __VA_ARGS__
#endif // TIME_PROFILING

#ifndef NB_CLUSTERS
	#define NB_CLUSTERS 1
#endif

#ifndef NB_CORES
	#define NB_CORES 16
#endif
#define NB_CLIENTS (NB_CORES-1)

#if (NB_CLIENTS > 0)
#define local_barrier() __builtin_k1_wpurge(); \
				__builtin_k1_fence(); \
				bsp_barrier(&barrier_0); \
				mOS_dinval();
#else
#define local_barrier()
#endif

#define OPAL_ASYNC_SEGMENT_DEFAULT MPPA_ASYNC_DDR_0

#ifdef DUPLEX
 #define OPAL_ASYNC_SEGMENT_SRC (arg_opal->step & 1 ? MPPA_ASYNC_DDR_1 : MPPA_ASYNC_DDR_0)
 #define OPAL_ASYNC_SEGMENT_DST (arg_opal->step & 1 ? MPPA_ASYNC_DDR_0 : MPPA_ASYNC_DDR_1)
#else
 #define OPAL_ASYNC_SEGMENT_SRC MPPA_ASYNC_DDR_0
 #define OPAL_ASYNC_SEGMENT_DST MPPA_ASYNC_DDR_0
#endif

// Number of anticipated block (async) on a cluster
// In Makefile now
// #define ASYNC_PIPELINE_DEPTH 4

// Dimension of an async block
#if 1
#if defined(INPLACE) || defined(SHIFT)
#if ASYNC_PIPELINE_DEPTH<=3
	#define CUBE_LENGTH_X (16)
	#define CUBE_LENGTH_Y (16)
	#define CUBE_LENGTH_Z (16)
#elif ASYNC_PIPELINE_DEPTH==4
	#define CUBE_LENGTH_X (16)
	#define CUBE_LENGTH_Y (8)
	#define CUBE_LENGTH_Z (16)
#elif ASYNC_PIPELINE_DEPTH>4
	#define CUBE_LENGTH_X (8)
	#define CUBE_LENGTH_Y (8)
	#define CUBE_LENGTH_Z (8)
#endif
#else // TWO_LATTICE
#if ASYNC_PIPELINE_DEPTH<=3
	#define CUBE_LENGTH_X (16)
	#define CUBE_LENGTH_Y (8)
	#define CUBE_LENGTH_Z (16)
#elif ASYNC_PIPELINE_DEPTH==4
	#define CUBE_LENGTH_X (8)
	#define CUBE_LENGTH_Y (8)
	#define CUBE_LENGTH_Z (16)
#elif ASYNC_PIPELINE_DEPTH>4
	#define CUBE_LENGTH_X (8)
	#define CUBE_LENGTH_Y (8)
	#define CUBE_LENGTH_Z (8)
#endif
#endif // INPLACE || SHIFT
#else
#define CUBE_LENGTH_X (8)
#define CUBE_LENGTH_Y (8)
#define CUBE_LENGTH_Z (8)
#endif

// Halo = 2 on D3Q19 (stencil degree 1)
#define CUBE_HALO_X (2)
#define CUBE_HALO_Y (2)
#define CUBE_HALO_Z (2)

// Full length of block in each dimension (halo included)
#define CUBE_FULL_LENGTH_X (CUBE_LENGTH_X + CUBE_HALO_X)
#define CUBE_FULL_LENGTH_Y (CUBE_LENGTH_Y + CUBE_HALO_Y)
#define CUBE_FULL_LENGTH_Z (CUBE_LENGTH_Z + CUBE_HALO_Z)

// Number of blocks/cubes in each dimension
#define NB_CUBES_X (CAVITY / CUBE_LENGTH_X)
#define NB_CUBES_Y (CAVITY / CUBE_LENGTH_Y)
#define NB_CUBES_Z (CAVITY / CUBE_LENGTH_Z)
#if NB_CLUSTERS==1
	#define NB_CUBES_PER_CLUSTER (NB_CUBES_X * NB_CUBES_Y * NB_CUBES_Z)
	#define NB_CUBES_TRAILING (0)
#else
	#define NB_CUBES_PER_CLUSTER ((NB_CUBES_X * NB_CUBES_Y * NB_CUBES_Z) / NB_CLUSTERS)
	#define NB_CUBES_TRAILING ((NB_CUBES_X * NB_CUBES_Y * NB_CUBES_Z) % NB_CLUSTERS)
#endif

// Real 3D block size (halo included)
#define NODE_SIZE_BYTES (Q*sizeof(float))
#define CUBE_SIZE_NODES (CUBE_FULL_LENGTH_X * CUBE_FULL_LENGTH_Y * CUBE_FULL_LENGTH_Z)
#define CUBE_SIZE_BYTES (CUBE_SIZE_NODES * NODE_SIZE_BYTES)

// offset from begin of local buffer, depends on icube
#define local_cube_get_begin_offset_nodes(icube, nb_blocks) (icube == 0 ? 1 : 0)

// offset from begin of remote block, depends on icube
#define remote_cube_get_begin_offset_nodes(icube, nb_blocks) (icube == 0 ? 0 : -1)

// halo reduction of the {remote|local} block, depends on icube
#define cube_get_halo_cutoff_nodes(icube, nb_blocks) \
	(((icube == 0) || (icube == (nb_blocks-1))) ? -1 : 0)

extern bsp_barrier_t barrier_0;
extern off64_t LatticeEven;
extern off64_t LatticeOdd;
extern off64_t Velocity;
extern off64_t Bitmap;
#ifdef DUMP_DENSITY
extern off64_t Density;
#endif // DUMP_DENSITY
#ifdef DUMP_DELTA
extern off64_t Delta;
#endif // DUMP_DELTA

#ifdef TIME_PROFILING
extern uint64_t compute_time;
extern uint64_t monitor_time;
extern uint64_t wait_time;
extern uint64_t get_time;
extern uint64_t put_time;
#endif // TIME_PROFILING

// async buffers
extern float async_buffers_src[ASYNC_PIPELINE_DEPTH][CUBE_FULL_LENGTH_Z][CUBE_FULL_LENGTH_Y][CUBE_FULL_LENGTH_X][Q];

#ifdef INPLACE
extern float tmp_wall[2][CUBE_FULL_LENGTH_Z][CUBE_FULL_LENGTH_X][Q];
#endif // INPLACE

#ifdef TWO_LATTICE
extern float async_buffers_dst[ASYNC_PIPELINE_DEPTH][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X][Q];
#endif // TWO_LATTICE

extern float async_velocity[ASYNC_PIPELINE_DEPTH][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X][D];
extern unsigned char async_bitmap[CUBE_LENGTH_Z][CUBE_LENGTH_X][3];
extern float delta_reduce_monocluster[NB_CORES];
extern float delta_reduce_multiclusters[NB_CLUSTERS];

#ifdef DUMP_DELTA
extern float async_delta[ASYNC_PIPELINE_DEPTH][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X];
#endif // DUMP_DELTA

extern mppa_async_event_t async_events_get[ASYNC_PIPELINE_DEPTH];
extern mppa_async_event_t async_events_put[ASYNC_PIPELINE_DEPTH];

struct utask_arg_opal_compute {
	config* Config;
	off64_t Src;
	off64_t Dst;
	int step;
};

struct utask_arg_opal_monitor {
	config* Config;
	results* Results;
	off64_t Src;
	off64_t Velocity;
	off64_t Delta;
	int step;
};

int getGeometry(config* Config, int x, int y, int z);
int opal_async_get_cube(void* local_buffer, off64_t remote_src, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event);
int opal_async_put_cube(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event);
int opal_async_put_cube_shift(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event);
int opal_async_get_small_cube(void* local_buffer, off64_t remote_src, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event);
int opal_async_put_small_cube(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event);
void deinterleave3(int n, int* x, int* y, int* z);

#endif // _COMPUTE_COMMON_H
