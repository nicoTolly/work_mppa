/*
 *  getGeometry: encode local geometry informations in a bitfield.
 *  NOTE: In case of more complex geometries, the bitfields might be created
 *  during initialisation and stored in device memory.
 */

#include "opal.h"

int getGeometry(config* Config, int x, int y, int z)
{
    int g = 0;

    if (x == Config->Lx-1) g |= G_W1;
    if (x == 0)    g |= G_W2;
    if (y == Config->Ly-1) g |= G_W3;
    if (y == 0)    g |= G_W4;
    if (z == Config->Lz-1) g |= G_W5;
    if (z == 0)    g |= G_W6;

    return g;
}

/*
 *  writePixel: compute RGB values corresponding to velocity variation u and
 *  write them to Bitmap array (u is supposed to range from 0 to 4).
 */
// void writePixel(results* Results, config* Config, int x, int z, float u)
// {
// 	int h;
// 	unsigned char s, r, g, b;

// 	unsigned char* Bitmap = (unsigned char*)Results->Bitmap;
// 	Bitmap += 3*(x + z*Config->Lx);
// 	h = (int)u % 6;
// 	s = (unsigned char)(255.F*(u - h));
// 	#if 0
// 	switch(h) {
// 	  case 0: r = 255; g = s; b = 0; break;
// 	  case 1: r = 255 - s; g = 255; b = 0; break;
// 	  case 2: r = 0; g = 255; b = s; break;
// 	  case 3: r = 0; g = 255 - s; b = 255; break;
// 	  case 4: r = s; g = 0; b = 255; break;
// 	  case 5: r = 255; g = 0; b = 255 - s; break;
// 	  default: r = 0; g = 0; b = 0;
// 	}
// 	#else
// 	if (h == 0) { r = 255; g = s; b = 0; }
// 	else if (h == 1) { r = 255 - s; g = 255; b = 0; }
// 	else if (h == 2) { r = 0; g = 255; b = s; }
// 	else if (h == 3) { r = 0; g = 255 - s; b = 255; }
// 	else if (h == 4) { r = s; g = 0; b = 255; }
// 	else if (h == 5) { r = 255; g = 0; b = 255 - s; }
// 	else { r = 0; g = 0; b = 0; }
// 	#endif
// 	Bitmap[0] = r;
// 	Bitmap[1] = g;
// 	Bitmap[2] = b;
// }

#ifdef TIME_PROFILING
uint64_t compute_time = 0;
uint64_t monitor_time = 0;
uint64_t wait_time = 0;
uint64_t get_time = 0;
uint64_t put_time = 0;
#endif // TIME_PROFILING

float async_velocity[ASYNC_PIPELINE_DEPTH][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X][D] = {{{{{0.0f}}}}};
unsigned char async_bitmap[CUBE_LENGTH_Z][CUBE_LENGTH_X][3] = {{{0.0f}}};
float delta_reduce_monocluster[NB_CORES] = {0.0f};
float delta_reduce_multiclusters[NB_CLUSTERS] = {0.0f};

#ifdef DUMP_DELTA
float async_delta[ASYNC_PIPELINE_DEPTH][CUBE_LENGTH_Z][CUBE_LENGTH_Y][CUBE_LENGTH_X] = {{{{0.0f}}}};
#endif // DUMP_DELTA

// async events array in get and put
mppa_async_event_t async_events_get[ASYNC_PIPELINE_DEPTH];
mppa_async_event_t async_events_put[ASYNC_PIPELINE_DEPTH];

// Get block from remote to local
// local_buffer = (void*) async_buffers_src[i]
// remote_src   = Lattice_{Even|Odd}
int opal_async_get_cube(void* local_buffer, off64_t remote_src, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event)
{
	// Local cube position
	mppa_async_point3d_t local_point = {
		.xpos =           0            + local_cube_get_begin_offset_nodes(icubex, NB_CUBES_X),
		.ypos =           0            + local_cube_get_begin_offset_nodes(icubey, NB_CUBES_Y),
		.zpos =           0            + local_cube_get_begin_offset_nodes(icubez, NB_CUBES_Z),
		.xdim = CUBE_FULL_LENGTH_X,
		.ydim = CUBE_FULL_LENGTH_Y,
		.zdim = CUBE_FULL_LENGTH_Z
	};

	// Remote cube position
	mppa_async_point3d_t remote_point = {
		.xpos = (icubex * CUBE_LENGTH_X) + remote_cube_get_begin_offset_nodes(icubex, NB_CUBES_X),
		.ypos = (icubey * CUBE_LENGTH_Y) + remote_cube_get_begin_offset_nodes(icubey, NB_CUBES_Y),
		.zpos = (icubez * CUBE_LENGTH_Z) + remote_cube_get_begin_offset_nodes(icubez, NB_CUBES_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sget_block3d(local_buffer, server, remote_src,
		elem_size,
		CUBE_FULL_LENGTH_X + cube_get_halo_cutoff_nodes(icubex, NB_CUBES_X) /* width */ ,
		CUBE_FULL_LENGTH_Y + cube_get_halo_cutoff_nodes(icubey, NB_CUBES_Y) /* height */ ,
		CUBE_FULL_LENGTH_Z + cube_get_halo_cutoff_nodes(icubez, NB_CUBES_Z) /* depth */ ,
		&local_point,
		&remote_point,
		event);
}

// Put block back from local to remote
int opal_async_put_cube(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event)
{
	// Local cube position
	mppa_async_point3d_t local_point = {
		.xpos = 1,
		.ypos = 1,
		.zpos = 1,
		.xdim = CUBE_FULL_LENGTH_X,
		.ydim = CUBE_FULL_LENGTH_Y,
		.zdim = CUBE_FULL_LENGTH_Z
	};

	// Remote cube position
	mppa_async_point3d_t remote_point = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sput_block3d(local_buffer, server, remote_src,
		elem_size,
		CUBE_LENGTH_X /* width */,
		CUBE_LENGTH_Y /* height */,
		CUBE_LENGTH_Z /* depth */,
		&local_point,
		&remote_point,
		event);
}

int opal_async_put_cube_shift(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event)
{
	// Local cube position
	mppa_async_point3d_t local_point = {
		.xpos = 0,
		.ypos = 0,
		.zpos = 0,
		.xdim = CUBE_FULL_LENGTH_X,
		.ydim = CUBE_FULL_LENGTH_Y,
		.zdim = CUBE_FULL_LENGTH_Z
	};

	// Remote cube position
	mppa_async_point3d_t remote_point = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sput_block3d(local_buffer, server, remote_src,
		elem_size,
		CUBE_LENGTH_X /* width */,
		CUBE_LENGTH_Y /* height */,
		CUBE_LENGTH_Z /* depth */,
		&local_point,
		&remote_point,
		event);
}

// Get block from remote to local, for Velovity and Delta, without halo
int opal_async_get_small_cube(void* local_buffer, off64_t remote_src, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event)
{
	// Local cube position
	mppa_async_point3d_t local_point = {
		.xpos = 0,
		.ypos = 0,
		.zpos = 0,
		.xdim = CUBE_LENGTH_X,
		.ydim = CUBE_LENGTH_Y,
		.zdim = CUBE_LENGTH_Z
	};

	// Remote cube position
	mppa_async_point3d_t remote_point = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sget_block3d(local_buffer, server, remote_src,
		elem_size,
		CUBE_LENGTH_X /* width */,
		CUBE_LENGTH_Y /* height */,
		CUBE_LENGTH_Z /* depth */,
		&local_point,
		&remote_point,
		event);
}

// Put block back from local to remote
int opal_async_put_small_cube(off64_t remote_src, const void* local_buffer, size_t elem_size,
	int icubex, int icubey, int icubez,
	const mppa_async_segment_t *server, mppa_async_event_t *event)
{
	// Local cube position
	mppa_async_point3d_t local_point = {
		.xpos = 0,
		.ypos = 0,
		.zpos = 0,
		.xdim = CUBE_LENGTH_X,
		.ydim = CUBE_LENGTH_Y,
		.zdim = CUBE_LENGTH_Z
	};

	// Remote cube position
	mppa_async_point3d_t remote_point = {
		.xpos = (icubex * CUBE_LENGTH_X),
		.ypos = (icubey * CUBE_LENGTH_Y),
		.zpos = (icubez * CUBE_LENGTH_Z),
		.xdim = CAVITY,
		.ydim = CAVITY,
		.zdim = CAVITY
	};

	return mppa_async_sput_block3d(local_buffer, server, remote_src,
		elem_size,
		CUBE_LENGTH_X /* width */,
		CUBE_LENGTH_Y /* height */,
		CUBE_LENGTH_Z /* depth */,
		&local_point,
		&remote_point,
		event);
}

/**
 * This Morton code is inspired from :
 * https://code.activestate.com/recipes/577558-interleave-bits-aka-morton-ize-aka-z-order-curve/
 */
int part1by1(int n){
	n&= 0x0000ffff;
	n = (n | (n << 8)) & 0x00FF00FF;
	n = (n | (n << 4)) & 0x0F0F0F0F;
	n = (n | (n << 2)) & 0x33333333;
	n = (n | (n << 1)) & 0x55555555;
	return n;
}

int unpart1by1(int n){
	n&= 0x55555555;
	n = (n ^ (n >> 1)) & 0x33333333;
	n = (n ^ (n >> 2)) & 0x0f0f0f0f;
	n = (n ^ (n >> 4)) & 0x00ff00ff;
	n = (n ^ (n >> 8)) & 0x0000ffff;
	return n;
}

int part1by2(int n){
	n&= 0x000003ff;
	n = (n ^ (n << 16)) & 0xff0000ff;
	n = (n ^ (n <<  8)) & 0x0300f00f;
	n = (n ^ (n <<  4)) & 0x030c30c3;
	n = (n ^ (n <<  2)) & 0x09249249;
	return n;
}

int unpart1by2(int n){
	n&= 0x09249249;
	n = (n ^ (n >>  2)) & 0x030c30c3;
	n = (n ^ (n >>  4)) & 0x0300f00f;
	n = (n ^ (n >>  8)) & 0xff0000ff;
	n = (n ^ (n >> 16)) & 0x000003ff;
	return n;
}

int interleave2(int x, int y){
	return part1by1(x) | (part1by1(y) << 1);
}

int interleave3(int x, int y, int z){
	return part1by2(x) | (part1by2(y) << 1) | (part1by2(z) << 2);
}

void deinterleave2(int n, int* x, int* y){
	*x = unpart1by1(n);
	*y = unpart1by1(n >> 1);
}

#if 0
void deinterleave3(int n, int* x, int* y, int* z){
#if (CUBE_LENGTH_X == CUBE_LENGTH_Y)
	*x = unpart1by2(n);
	*y = unpart1by2(n >> 1);
#elif (CUBE_LENGTH_X == 2*CUBE_LENGTH_Y)
	*y = unpart1by2(n);
	*x = unpart1by2(n >> 1);
#endif
	*z = unpart1by2(n >> 2);
}
#else
void deinterleave3(int n, int* x, int* y, int* z){
	int z_res = (n / (NB_CUBES_X*NB_CUBES_Y));
	int y_res = (n - z_res*(NB_CUBES_X*NB_CUBES_Y)) / NB_CUBES_X;
	int x_res = (n - z_res*(NB_CUBES_X*NB_CUBES_Y) - y_res*NB_CUBES_X);
	*z = z_res;
	*y = y_res;
	*x = x_res;
}
#endif

/*
 *  Init: init kernel
 */
void init(config* Config, results* Results)
{
	int cid = __k1_get_cluster_id();
	int status = 0;
	mppa_async_event_t events[4];

	// Cluster 0 initializes global memory
	if(cid == 0){
		off64_t addr_even = LatticeEven;
		off64_t addr_odd = LatticeOdd;
		off64_t addr_vel = Velocity;
#ifdef DUMP_DENSITY
		off64_t addr_density = Density;
#endif // DUMP_DENSITY
#ifdef DUMP_DELTA
		off64_t addr_delta = Delta;
#endif // DUMP_DELTA
		off64_t addr_bitmap = Bitmap;

		// Init Lattice{Even|Odd} by repeatly putting from async_buffers to DDR
		for(int i = 0; i < (NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z); i++){
			status |= mppa_async_put( &async_buffers_src[0][0][0][0][0],
				OPAL_ASYNC_SEGMENT_DEFAULT, addr_even,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*Q*sizeof(float), &events[0]);
			status |= mppa_async_put( &async_buffers_src[0][0][0][0][0],
#ifdef DUPLEX
				MPPA_ASYNC_DDR_1,
#else
				MPPA_ASYNC_DDR_0,
#endif
				addr_odd,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*Q*sizeof(float), &events[1]);
			addr_even += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*Q*sizeof(float);
			addr_odd += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*Q*sizeof(float);
		}

		// Init Velocity by repeatly putting from async_buffers to DDR
		for(int i = 0; i < (NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z); i++){
			status |= mppa_async_put( &async_buffers_src[0][0][0][0][0],
				OPAL_ASYNC_SEGMENT_DEFAULT, addr_vel,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*D*sizeof(float), &events[2]);
			addr_vel += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*D*sizeof(float);
		}

		// Init Density and Delta by repeatly putting from async_buffers to DDR
		for(int i = 0; i < (NB_CUBES_X*NB_CUBES_Y*NB_CUBES_Z); i++){
#ifdef DUMP_DENSITY
			status |= mppa_async_put( &async_buffers_src[0][0][0][0][0],
				OPAL_ASYNC_SEGMENT_DEFAULT, addr_density,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*sizeof(float), /*&events[3]*/ NULL);
			addr_density += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*sizeof(float);
#endif // DUMP_DENSITY
#ifdef DUMP_DELTA
			status |= mppa_async_put( &async_buffers_src[0][0][0][0][0],
				OPAL_ASYNC_SEGMENT_DEFAULT, addr_delta,
				CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*sizeof(float), /*&events[4]*/ NULL);
			addr_delta += CUBE_LENGTH_X*CUBE_LENGTH_Y*CUBE_LENGTH_Z*sizeof(float);
#endif // DUMP_DELTA
		}

		// // Write bitmap at step 0
		// if (Results->Bitmap) {
		// 	for(int z = 0; z < Config->Lz; z++){
		// 		for(int x = 0; x < Config->Lx; x++){
		// 			writePixel(Results, Config, x, z, 0); // Set Bitmap
		// 		}
		// 	}
		// }
		// // Export image at step 0
		// if(Config->Images) exportImage(Config, Results, 0);

		if(Bitmap){
			for(int i = 0; i < CUBE_LENGTH_Z; i++){
				for(int j = 0; j < CUBE_LENGTH_X; j++){
					async_bitmap[i][j][0] = 255;
					async_bitmap[i][j][1] = 0;
					async_bitmap[i][j][2] = 0;
				}
			}
			for(int i = 0; i < NB_CUBES_X*NB_CUBES_Z; i++){
				status |= mppa_async_put( &async_bitmap[0][0][0],
					OPAL_ASYNC_SEGMENT_DEFAULT, addr_bitmap,
					CUBE_LENGTH_X*CUBE_LENGTH_Z*3*sizeof(unsigned char), &events[3]);
				addr_bitmap += CUBE_LENGTH_X*CUBE_LENGTH_Z*3*sizeof(unsigned char);
			}
		}

		// Broadcast pointers to other clusters
		for(int i = 0; i < NB_CLUSTERS-1; i++){
			status |= mppa_async_put((void*)&LatticeEven,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&LatticeEven,
				sizeof(off64_t), NULL);
			status |= mppa_async_put((void*)&LatticeOdd,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&LatticeOdd,
				sizeof(off64_t), NULL);
			status |= mppa_async_put((void*)&Velocity,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&Velocity,
				sizeof(off64_t), NULL);
			status |= mppa_async_put((void*)&Bitmap,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&Bitmap,
				sizeof(off64_t), NULL);
			#ifdef DUMP_DENSITY
			status |= mppa_async_put((void*)&Density,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&Density,
				sizeof(off64_t), NULL);
			#endif // DUMP_DENSITY
			#ifdef DUMP_DELTA
			status |= mppa_async_put((void*)&Delta,
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC1 + i), (uintptr_t)&Delta,
				sizeof(off64_t), NULL);
			#endif // DUMP_DELTA
		}

		CHECK_ERROR(status, "OPAL init : fail to asynchronously put data\n");
		status |= mppa_async_event_waitall(4, events);
		CHECK_ERROR(status, "OPAL init : fail to wait for async_put on init\n");

		// export first bitmap
		if(Bitmap && Config->Images){
			int fd_bitmap = 0;
			char string[32];
			sprintf(string, "%s/%s%07d.bmp", ".", "ldc_mppa_async_", 0);
			fd_bitmap = open(string, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
			write(fd_bitmap, "BM", 2);
			write(fd_bitmap, Results->Header, 13*sizeof(int));
			write(fd_bitmap, (char*)((MPPA_ASYNC_DDR_0)->base) + Bitmap, Config->Lx*Config->Lz*3*sizeof(unsigned char));
			close(fd_bitmap);
		}

	}
	// every clusters wait for cluster 0 finish init data
	mppa_rpc_barrier_all();
	mOS_dinval();
	// printf("# [Cluster %2d] LatticeEven 0x%llx LatticeOdd 0x%llx Velocity 0x%llx Density 0x%llx Delta 0x%llx Bitmap 0x%llx\n",
	// 	cid, (uint64_t) ((int)LatticeEven), (uint64_t) ((int)LatticeOdd), (uint64_t) ((int)Velocity),
	// 	// (uint64_t) ((int)Density), (uint64_t) ((int)Delta),
	// 	(uint64_t) ((int)0), (uint64_t) ((int)0),
	// 	(uint64_t) ((int)Bitmap));
}

void monitor_cube(int pid, int icubex, int icubey, int icubez, int ibuf, struct utask_arg_opal_monitor* arg_opal)
{
	// Moments
	float rho, jx, jy, jz;

	// Auxiliary variables
	float t0, t1;

	// Local density distribution
	float f[19] = {0};

	uint64_t start;

	//////////////////////////////////////////////////////////////////////////////////
	// On local_cube[16][16][16][Q], each PE will work on [pid][16][16][Q]
	//////////////////////////////////////////////////////////////////////////////////
	// Global coordinates and ranges of a PE
#if 0
	#if (CUBE_LENGTH_Z == 16)
	int zl = 1 + pid; // in local_cube, start point in always +1, like an onion
	int zs = pid;
	int z = (icubez * CUBE_LENGTH_Z) + pid; // will not change during the cube compute, but will change between icubez

	// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
	for(int yl = 1, ys = 0, y = icubey * CUBE_LENGTH_Y; yl <= CUBE_LENGTH_Y ; yl++, y++, ys++){ // also increment global y
		for(int xl = 1, xs = 0, x = icubex * CUBE_LENGTH_X; xl <= CUBE_LENGTH_X ; xl++, x++, xs++){ // also increment global x
	#elif (CUBE_LENGTH_Z == 8)
	int zl = 1 + pid%CUBE_LENGTH_Z; // in local_cube, start point in always +1, like an onion
	int zs = pid%CUBE_LENGTH_Z;
	int z = (icubez * CUBE_LENGTH_Z) + pid%CUBE_LENGTH_Z; // will not change during the cube compute, but will change between icubez
	int yl_begin = 1 + (pid/CUBE_LENGTH_Z)*(CUBE_LENGTH_Y/2);
	int yl_end   = yl_begin + (CUBE_LENGTH_Y/2);

	// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
	for(int yl = yl_begin, ys = yl_begin-1, y = yl_begin-1 + (icubey * CUBE_LENGTH_Y); yl < yl_end ; yl++, y++, ys++){ // also increment global y
		for(int xl = 1, xs = 0, x = icubex * CUBE_LENGTH_X; xl <= CUBE_LENGTH_X ; xl++, x++, xs++){ // also increment global x
	#elif (CUBE_LENGTH_Z == 1)
	int zl = 1; // in local_cube, start point in always +1, like an onion
	int zs = 0;
	int z = (icubez * CUBE_LENGTH_Z); // will not change during the cube compute, but will change between icubez
	int yl_begin = 1 + pid*(CUBE_LENGTH_Y/NB_CORES);
	int yl_end   = yl_begin + (CUBE_LENGTH_Y/NB_CORES);

	// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
	for(int yl = yl_begin, ys = yl_begin-1, y = yl_begin-1 + (icubey * CUBE_LENGTH_Y); yl < yl_end ; yl++, y++, ys++){ // also increment global y
		for(int xl = 1, xs = 0, x = icubex * CUBE_LENGTH_X; xl <= CUBE_LENGTH_X ; xl++, x++, xs++){ // also increment global x
	#endif
#else
	int zl = 1 + pid; // in local_cube, start point in always +1, like an onion
	int zs = pid;
	int z = (icubez * CUBE_LENGTH_Z) + pid; // will not change during the cube compute, but will change between icubez

	// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
	for(int yl = 1, ys = 0, y = icubey * CUBE_LENGTH_Y; yl <= CUBE_LENGTH_Y ; yl++, y++, ys++){ // also increment global y
		for(int xl = 1, xs = 0, x = icubex * CUBE_LENGTH_X; xl <= CUBE_LENGTH_X ; xl++, x++, xs++){ // also increment global x
#endif
			if(pid < CUBE_LENGTH_Z) {
				ACC_TIME_FUNC(monitor_time,
					// Get geometry based on global coordinates
					int g = getGeometry(arg_opal->Config, x, y, z);

					// Fetch source density distribution based on local coordinates
					f[ 0] = async_buffers_src[ibuf][zl][yl][xl][0];
					if (g & G_BND) {
						// Simple bounce-back boundary condition
						if (g & G_E02) f[ 1] = async_buffers_src[ibuf][zl][yl][xl][ 2]; else f[ 1] = async_buffers_src[ibuf][zl  ][yl  ][xl-1][ 1];
						if (g & G_E01) f[ 2] = async_buffers_src[ibuf][zl][yl][xl][ 1]; else f[ 2] = async_buffers_src[ibuf][zl  ][yl  ][xl+1][ 2];
						if (g & G_E04) f[ 3] = async_buffers_src[ibuf][zl][yl][xl][ 4]; else f[ 3] = async_buffers_src[ibuf][zl  ][yl-1][xl  ][ 3];
						if (g & G_E03) f[ 4] = async_buffers_src[ibuf][zl][yl][xl][ 3]; else f[ 4] = async_buffers_src[ibuf][zl  ][yl+1][xl  ][ 4];
						if (g & G_E06) f[ 5] = async_buffers_src[ibuf][zl][yl][xl][ 6]; else f[ 5] = async_buffers_src[ibuf][zl-1][yl  ][xl  ][ 5];
						if (g & G_E05) f[ 6] = async_buffers_src[ibuf][zl][yl][xl][ 5]; else f[ 6] = async_buffers_src[ibuf][zl+1][yl  ][xl  ][ 6];
						if (g & G_E10) f[ 7] = async_buffers_src[ibuf][zl][yl][xl][10]; else f[ 7] = async_buffers_src[ibuf][zl  ][yl-1][xl-1][ 7];
						if (g & G_E09) f[ 8] = async_buffers_src[ibuf][zl][yl][xl][ 9]; else f[ 8] = async_buffers_src[ibuf][zl  ][yl-1][xl+1][ 8];
						if (g & G_E08) f[ 9] = async_buffers_src[ibuf][zl][yl][xl][ 8]; else f[ 9] = async_buffers_src[ibuf][zl  ][yl+1][xl-1][ 9];
						if (g & G_E07) f[10] = async_buffers_src[ibuf][zl][yl][xl][ 7]; else f[10] = async_buffers_src[ibuf][zl  ][yl+1][xl+1][10];
						if (g & G_E14) f[11] = async_buffers_src[ibuf][zl][yl][xl][14]; else f[11] = async_buffers_src[ibuf][zl-1][yl  ][xl-1][11];
						if (g & G_E13) f[12] = async_buffers_src[ibuf][zl][yl][xl][13]; else f[12] = async_buffers_src[ibuf][zl-1][yl  ][xl+1][12];
						if (g & G_E12) f[13] = async_buffers_src[ibuf][zl][yl][xl][12]; else f[13] = async_buffers_src[ibuf][zl+1][yl  ][xl-1][13];
						if (g & G_E11) f[14] = async_buffers_src[ibuf][zl][yl][xl][11]; else f[14] = async_buffers_src[ibuf][zl+1][yl  ][xl+1][14];
						if (g & G_E18) f[15] = async_buffers_src[ibuf][zl][yl][xl][18]; else f[15] = async_buffers_src[ibuf][zl-1][yl-1][xl  ][15];
						if (g & G_E17) f[16] = async_buffers_src[ibuf][zl][yl][xl][17]; else f[16] = async_buffers_src[ibuf][zl-1][yl+1][xl  ][16];
						if (g & G_E16) f[17] = async_buffers_src[ibuf][zl][yl][xl][16]; else f[17] = async_buffers_src[ibuf][zl+1][yl-1][xl  ][17];
						if (g & G_E15) f[18] = async_buffers_src[ibuf][zl][yl][xl][15]; else f[18] = async_buffers_src[ibuf][zl+1][yl+1][xl  ][18];

						// Imposed lid velocity
						if (z == arg_opal->Config->Lx-1) {
							f[13] += k0;
							f[14] -= k0;
						}
					} else {
						// Pull propagation
						f[ 1] = async_buffers_src[ibuf][zl  ][yl  ][xl-1][ 1];
						f[ 2] = async_buffers_src[ibuf][zl  ][yl  ][xl+1][ 2];
						f[ 3] = async_buffers_src[ibuf][zl  ][yl-1][xl  ][ 3];
						f[ 4] = async_buffers_src[ibuf][zl  ][yl+1][xl  ][ 4];
						f[ 5] = async_buffers_src[ibuf][zl-1][yl  ][xl  ][ 5];
						f[ 6] = async_buffers_src[ibuf][zl+1][yl  ][xl  ][ 6];
						f[ 7] = async_buffers_src[ibuf][zl  ][yl-1][xl-1][ 7];
						f[ 8] = async_buffers_src[ibuf][zl  ][yl-1][xl+1][ 8];
						f[ 9] = async_buffers_src[ibuf][zl  ][yl+1][xl-1][ 9];
						f[10] = async_buffers_src[ibuf][zl  ][yl+1][xl+1][10];
						f[11] = async_buffers_src[ibuf][zl-1][yl  ][xl-1][11];
						f[12] = async_buffers_src[ibuf][zl-1][yl  ][xl+1][12];
						f[13] = async_buffers_src[ibuf][zl+1][yl  ][xl-1][13];
						f[14] = async_buffers_src[ibuf][zl+1][yl  ][xl+1][14];
						f[15] = async_buffers_src[ibuf][zl-1][yl-1][xl  ][15];
						f[16] = async_buffers_src[ibuf][zl-1][yl+1][xl  ][16];
						f[17] = async_buffers_src[ibuf][zl+1][yl-1][xl  ][17];
						f[18] = async_buffers_src[ibuf][zl+1][yl+1][xl  ][18];
					}

					// Computation of moments
					t0 = f[ 1] + f[ 2] + f[ 3] + f[ 4] + f[ 5] + f[ 6];
					t1 = f[ 7] + f[ 8] + f[ 9] + f[10] + f[11] + f[12] + f[13] + f[14]
					  + f[15] + f[16] + f[17] + f[18];
					rho = f[ 0] + t0 + t1;
					// e = -30*f[ 0] - 11*t0 + 8*t1;
					// eps = 12*f[ 0] - 4*t0 + t1;
					t0 = f[ 1] - f[ 2];
					t1 = f[ 7] - f[ 8] + f[ 9] - f[10] + f[11] - f[12] + f[13] - f[14];
					jx = t0 + t1;
					// qx = -4*t0 + t1;
					t0 = f[ 3] - f[ 4];
					t1 = f[ 7] + f[ 8] - f[ 9] - f[10] + f[15] - f[16] + f[17] - f[18];
					jy = t0 + t1;
					// qy = -4*t0 + t1;
					t0 = f[ 5] - f[ 6];
					t1 = f[11] + f[12] - f[13] - f[14] + f[15] + f[16] - f[17] - f[18];
					jz = t0 + t1;
					// qz = -4*t0 + t1;

					// Fetch stored velocity components
					float ux = async_velocity[ibuf][zs][ys][xs][0];
					float uy = async_velocity[ibuf][zs][ys][xs][1];
					float uz = async_velocity[ibuf][zs][ys][xs][2];

					// Store current velocity
					float vx = jx/(1 + rho);
					float vy = jy/(1 + rho);
					float vz = jz/(1 + rho);
					async_velocity[ibuf][zs][ys][xs][0] = vx;
					async_velocity[ibuf][zs][ys][xs][1] = vy;
					async_velocity[ibuf][zs][ys][xs][2] = vz;

					// Compute and store squared velocity variation
					#ifdef DUMP_DELTA
					async_delta[ibuf][zs][ys][xs]  = (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);
					delta_reduce_monocluster[pid]  += async_delta[ibuf][zs][ys][xs];
					#else
					delta_reduce_monocluster[pid]  += (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);
					#endif // DUMP_DELTA

					// Write bitmap pixel
					// if (arg_opal->Results->Bitmap && y == (arg_opal->Config->Ly>>1)){
					if (arg_opal->Config->Images && icubey == (NB_CUBES_Y>>1) && y == (arg_opal->Config->Ly>>1)){
						// writePixel(arg_opal->Results, arg_opal->Config, x, z, 4*sqrt(vx*vx + vy*vy + vz*vz)/(arg_opal->Config->U0));
						unsigned char r, g, b;
						float u = 4*sqrt(vx*vx + vy*vy + vz*vz)/(arg_opal->Config->U0);
						int h = (int)u % 6;
						unsigned char s = (unsigned char)(255.F*(u - h));
						switch(h) {
						  case 0: r = 255; g = s; b = 0; break;
						  case 1: r = 255 - s; g = 255; b = 0; break;
						  case 2: r = 0; g = 255; b = s; break;
						  case 3: r = 0; g = 255 - s; b = 255; break;
						  case 4: r = s; g = 0; b = 255; break;
						  case 5: r = 255; g = 0; b = 255 - s; break;
						  default: r = 0; g = 0; b = 0;
						}
						// printf("Cluster %d pid %d updates bitmap rgb %d %d %d at zs %d xs %d\n", cid, pid, r, g, b, zs, xs);
						async_bitmap[zs][xs][0] = r;
						async_bitmap[zs][xs][1] = g;
						async_bitmap[zs][xs][2] = b;
					}
				); // end ACC_FUNC on monitor_time
			} // if(pid < CUBE_LENGTH_Z)
		} // end for xl
	} // end for yl
}

/* routine to be executed on each PE */
void* task_routine_opal_monitor(void *arg)
{
	mOS_dinval();

	int cid = __k1_get_cluster_id();
	int pid = __k1_get_cpu_id();
	int status = 0;
	uint64_t start;

	struct utask_arg_opal_monitor* arg_opal = (struct utask_arg_opal_monitor*) arg;

	int icubex, icubey, icubez;
	int ibuf = 0;
	int icube_begin = cid*NB_CUBES_PER_CLUSTER;
	int icube_end = (cid+1)*NB_CUBES_PER_CLUSTER;
	delta_reduce_monocluster[pid] = 0.0f;

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async cube copies */
	if(pid == 0){
		for(int i = 0; i < min(ASYNC_PIPELINE_DEPTH, NB_CUBES_PER_CLUSTER); i++){
			deinterleave3(icube_begin+i, &icubex, &icubey, &icubez);
			ACC_TIME_FUNC(get_time,
				status |= opal_async_get_cube(&async_buffers_src[i][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES,
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, &async_events_get[i]);
				status |= opal_async_get_small_cube(&async_velocity[i][0][0][0][0], arg_opal->Velocity, D*sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, &async_events_get[i]);
			);
		}
	}

	/* KERNEL */
	// Macro pipeline through all cubes per cluster
	for(int icube = icube_begin; icube < icube_end; icube++, ibuf = (ibuf+1)%ASYNC_PIPELINE_DEPTH){
		deinterleave3(icube, &icubex, &icubey, &icubez);
		// PE0 waits for cube's arrival, others PE too
		if (pid == 0) {
			ACC_TIME_FUNC(wait_time,
				status |= mppa_async_event_wait(&async_events_get[ibuf]);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to wait for getting cube. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}. \n", cid, pid, ibuf, icubex, icubey, icubez);
		}

		local_barrier()

		monitor_cube(pid, icubex, icubey, icubez, ibuf, arg_opal);

		// make a barrier to gather results of local_cube[16][16][16][Q], before PE0 putting back to Dst
		local_barrier()
		if (pid == 0) {
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_small_cube(arg_opal->Velocity, &async_velocity[ibuf][0][0][0][0], D*sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, &async_events_put[ibuf]);
			);
			#ifdef DUMP_DELTA
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_small_cube(arg_opal->Delta, &async_delta[ibuf][0][0][0], sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, &async_events_put[ibuf]);
			);
			#endif // DUMP_DELTA

			if (arg_opal->Config->Images && icubey == (NB_CUBES_Y>>1)){
				mppa_async_point2d_t local_point = {0, 0, CUBE_LENGTH_X, CUBE_LENGTH_Z};
				mppa_async_point2d_t remote_point = {icubex*CUBE_LENGTH_X,icubez*CUBE_LENGTH_Z, CAVITY, CAVITY};
				ACC_TIME_FUNC(put_time,
					status |= mppa_async_sput_block2d(&async_bitmap[0][0][0], OPAL_ASYNC_SEGMENT_DEFAULT, Bitmap,
						3*sizeof(unsigned char),                                /* elem_size */
						CUBE_LENGTH_X, CUBE_LENGTH_Z,
						&local_point,
						&remote_point,
						&async_events_put[ibuf]);
				);
			}

			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to put velocity and delta cubes to global memory. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, ibuf, icubex, icubey, icubez);

			// wait for the previous async put of local 'ibuf+2' before enqueueing the next get of remote 'icube+2'
			// into the same buffer, avoid data-racing
			int next_ibuf = (ibuf+ASYNC_PIPELINE_DEPTH) % ASYNC_PIPELINE_DEPTH;

			// This wait is not needed on the first loop, because it is prologue get.
			if(icube - icube_begin > 0){
				ACC_TIME_FUNC(wait_time,
					status |= mppa_async_event_wait(&async_events_put[next_ibuf]);
				);
				CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to wait for previous put. "
						"next_ibuf = %d\n", cid, pid, next_ibuf);
			}

			// then enqueues next async_get of the remote 'icube+2' (if exists) to the local 'ibuf+2'
			if((icube_end - icube) > ASYNC_PIPELINE_DEPTH-1){
				deinterleave3(icube+ASYNC_PIPELINE_DEPTH, &icubex, &icubey, &icubez);
				ACC_TIME_FUNC(get_time,
					status |= opal_async_get_cube(&async_buffers_src[next_ibuf][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES,
						icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, &async_events_get[next_ibuf]);
					status |= opal_async_get_small_cube(&async_velocity[next_ibuf][0][0][0][0], arg_opal->Velocity, D*sizeof(float),
						icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, &async_events_get[next_ibuf]);
				);
				CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to get next cube. "
						"next_ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, next_ibuf, icubex, icubey, icubez);
			}
		}
	} // end for icube

	/* EPILOGUE : PE0 wait for the last async put */
	if (pid == 0) {
		for(int i = 0; i < min(ASYNC_PIPELINE_DEPTH, NB_CUBES_PER_CLUSTER); i++){
			ACC_TIME_FUNC(wait_time,
				status |= mppa_async_event_wait(&async_events_put[i]);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to wait for last two puts. "
					"ibuf+2 = %d\n", cid, pid, (ibuf+2) % ASYNC_PIPELINE_DEPTH);
		}
	}

	#if NB_CUBES_TRAILING > 0
	// works on cid, compute trailing cube by blocking call
	if(cid < NB_CUBES_TRAILING){
		int trailing_ibuf = (ibuf+1) % ASYNC_PIPELINE_DEPTH;
		deinterleave3(cid + (NB_CLUSTERS*NB_CUBES_PER_CLUSTER), &icubex, &icubey, &icubez);
		if(pid == 0){
			ACC_TIME_FUNC(get_time,
				status |= opal_async_get_cube(&async_buffers_src[trailing_ibuf][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES,
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, NULL);
				status |= opal_async_get_small_cube(&async_velocity[trailing_ibuf][0][0][0][0], arg_opal->Velocity, D*sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, NULL);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_monitor failed to get trailing cube. "
						"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, trailing_ibuf, icubex, icubey, icubez);
		}

		local_barrier()
		monitor_cube(pid, icubex, icubey, icubez, trailing_ibuf, arg_opal);
		local_barrier()

		if(pid == 0){
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_small_cube(arg_opal->Velocity, &async_velocity[trailing_ibuf][0][0][0][0], D*sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, NULL);
			);
			#ifdef DUMP_DELTA
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_small_cube(arg_opal->Delta, &async_delta[trailing_ibuf][0][0][0], sizeof(float),
					icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DEFAULT, NULL);
			);
			#endif // DUMP_DELTA

			if (arg_opal->Config->Images && icubey == (NB_CUBES_Y>>1)){
				mppa_async_point2d_t local_point = {0, 0, CUBE_LENGTH_X, CUBE_LENGTH_Z};
				mppa_async_point2d_t remote_point = {icubex*CUBE_LENGTH_X,icubez*CUBE_LENGTH_Z, CAVITY, CAVITY};
				ACC_TIME_FUNC(put_time,
					status |= mppa_async_sput_block2d(&async_bitmap[0][0][0], OPAL_ASYNC_SEGMENT_DEFAULT, Bitmap,
						3*sizeof(unsigned char),                                /* elem_size */
						CUBE_LENGTH_X, CUBE_LENGTH_Z,
						&local_point,
						&remote_point,
						NULL);
				);
			}
		}
	}
	#endif // NB_CUBES_TRAILING > 0

	return NULL;
}

/*
 *  monitor: monitor kernel function
 */
void monitor(config* Config, results* Results, off64_t Src, off64_t Velocity, off64_t Delta, int step)
{
	int cid = __k1_get_cluster_id();
	int status = 0;

	struct utask_arg_opal_monitor arg_opal_monitor = {
		.Config = Config,
		.Results = Results,
		.Src = Src,
		.Velocity = Velocity,
		.Delta = Delta,
		.step = step
	};

	utask_t utask[NB_CLIENTS];
	for (int i = 0; i < NB_CLIENTS; i++) {
		utask_create(&utask[i], NULL, task_routine_opal_monitor, (void*)&arg_opal_monitor);
	}

	// start routine on self.
	task_routine_opal_monitor((void*)&arg_opal_monitor);

	// End of a timestep, join other threads
	for (int i = 0; i < NB_CLIENTS; i++) {
		utask_join(utask[i], NULL);
	}

	// Now get delta reduce of monocluster
	__builtin_k1_wpurge();
	__builtin_k1_fence();
	mOS_dinval();

	delta_reduce_multiclusters[cid] = 0.0f;
	for(int i = 0; i < NB_CORES; i++){
		delta_reduce_multiclusters[cid] += delta_reduce_monocluster[i];
	}
	// printf("Cluster %2d delta_reduce_multiclusters[%2d] %f \n", cid, cid, delta_reduce_multiclusters[cid]);
	__builtin_k1_wpurge();
	__builtin_k1_fence();

	// broadcast all-to-all of delta_reduce_multiclusters[cid]
	for(int i = 0; i < NB_CLUSTERS; i++){
		if(i != cid) {
			status |= mppa_async_put(&delta_reduce_multiclusters[cid],
				mppa_async_default_segment(MPPA_ASYNC_SERVER_CC0 + i),
				(uintptr_t)&delta_reduce_multiclusters[cid], sizeof(float), NULL);
		}
	}

	mppa_rpc_barrier_all();
	mOS_dinval();

	// Each cluster calculate total_delta
	float total_delta = 0.0f;
	for(int i = 0; i < NB_CLUSTERS; i++){
		total_delta += delta_reduce_multiclusters[i];
	}
	// Monitoring, print progress and exports image
	Results->L2 = sqrt(total_delta / Results->DomainSize);
   if (Results->L2 < Results->epsilon) Results->Converged = 1;

   // if(cid == 0) {printf("Cluster %2d monitor : total_delta %f L2 %.7f converged %d status %d\n",
   //    	cid, total_delta, Results->L2, Results->Converged, status);}

	if (cid == 0 && Config->Images){
		char string[32];
		sprintf(string, "%s/%s%07d.bmp", ".", "ldc_mppa_async_", step);
		int fd_bitmap = open(string, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		write(fd_bitmap, "BM", 2);
		write(fd_bitmap, Results->Header, 13*sizeof(int));
		write(fd_bitmap, (char*)((MPPA_ASYNC_DDR_0)->base) + Bitmap, Config->Lx*Config->Lz*3*sizeof(unsigned char));
		close(fd_bitmap);

		#ifdef DUMP_DELTA
		// export delta
		sprintf(string, "%s/%s%07d", ".", "delta_", step);
		fd_bitmap = open(string, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
		write(fd_bitmap, (char*)((MPPA_ASYNC_DDR_0)->base) + Delta, Config->Lx*Config->Ly*Config->Lz*sizeof(float));
		close(fd_bitmap);
		#endif
	}

	mppa_rpc_barrier_all();
}
