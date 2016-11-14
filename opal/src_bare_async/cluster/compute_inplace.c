/*
 * OPAL (OpenCL Processor Array LBM solver)
 *
 * Copyright (c) 2014, Christian Obrecht <courriel@obrecht.fr>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 *
 *  compute.c: main computation kernel.
 *
 */

#include "opal.h"
#include "global.h"

// async buffers
float async_buffers_src[ASYNC_PIPELINE_DEPTH][CUBE_FULL_LENGTH_Z][CUBE_FULL_LENGTH_Y][CUBE_FULL_LENGTH_X][Q] = {{{{{0.0f}}}}};
float tmp_wall[2][CUBE_FULL_LENGTH_Z][CUBE_FULL_LENGTH_X][Q] = {{{{0.0f}}}};

// Copy the wall y = {CUBE_FULL_LENGTH_Z x CUBE_FULL_LENGTH_X} into tmp_wall
// before doing in-place update
// pid 0 and 15 will copy two lines, other pid copy one line
#define copy_current_wall(yl, pid, ibuf) \
do{ \
	for(int i = 0; i < CUBE_FULL_LENGTH_X; i++){ \
		for(int j = 0; j < Q; j++){ \
			tmp_wall[yl&1][pid+1][i][j] = async_buffers_src[ibuf][pid+1][yl][i][j];  \
		} \
	} \
	if(pid == 0 || pid == CUBE_LENGTH_Z-1){ \
		int z_tmp = (pid == 0 ? 0 : (CUBE_FULL_LENGTH_Z-1)); \
		for(int i = 0; i < CUBE_FULL_LENGTH_X; i++){ \
			for(int j = 0; j < Q; j++){ \
				tmp_wall[yl&1][z_tmp][i][j] = async_buffers_src[ibuf][z_tmp][yl][i][j];  \
			} \
		} \
	} \
}while(0)

void collide_cube(int pid, int icubex, int icubey, int icubez, int ibuf, struct utask_arg_opal_compute* arg_opal)
{
	// Moments
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
	    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

	// Auxiliary variables
	float t0, t1, t2, t3;

	// Local density distribution
	float f[19] = {0};

	uint64_t start;

	//////////////////////////////////////////////////////////////////////////////////
	// On local_cube[16][16][16][Q], each PE will work on [pid][16][16][Q]
	//////////////////////////////////////////////////////////////////////////////////
	// Global coordinates and ranges of a PE
	int zl = 1 + pid; // in local_cube, start point in always +1, like an onion
	int z = (icubez * CUBE_LENGTH_Z) + pid; // will not change during the cube compute, but will change between icubez

	if(pid < CUBE_LENGTH_Z) copy_current_wall(0, pid, ibuf);

	// Each PE sweeps CUBE_LENGTH_Y * CUBE_LENGTH_X, with ylocal and xlocal starting from 1 to CUBE_LENGTH_{X|Y}
	for(int yl = 1, y = icubey * CUBE_LENGTH_Y; yl <= CUBE_LENGTH_Y ; yl++, y++){ // also increment global y

		if(pid < CUBE_LENGTH_Z) copy_current_wall(yl, pid, ibuf);

		local_barrier()

		if(pid < CUBE_LENGTH_Z){
			for(int xl = 1, x = icubex * CUBE_LENGTH_X; xl <= CUBE_LENGTH_X ; xl++, x++){ // also increment global x

				ACC_TIME_FUNC(compute_time,
					// Get geometry based on global coordinates
					int g = getGeometry(arg_opal->Config, x, y, z);

					// Fetch source density distribution based on local coordinates
					f[ 0] = tmp_wall[yl&1][zl][xl][0];
					if (g & G_BND) {
						// Simple bounce-back boundary condition
						if (g & G_E02) f[ 1] = tmp_wall[yl&1][zl][xl][ 2]; else f[ 1] = /* async_buffers_src[ibuf][zl  ][yl  ][xl-1][ 1]; */ tmp_wall[(yl  )&1][zl  ][xl-1][ 1];
						if (g & G_E01) f[ 2] = tmp_wall[yl&1][zl][xl][ 1]; else f[ 2] = /* async_buffers_src[ibuf][zl  ][yl  ][xl+1][ 2]; */ tmp_wall[(yl  )&1][zl  ][xl+1][ 2];
						if (g & G_E04) f[ 3] = tmp_wall[yl&1][zl][xl][ 4]; else f[ 3] = /* async_buffers_src[ibuf][zl  ][yl-1][xl  ][ 3]; */ tmp_wall[(yl-1)&1][zl  ][xl  ][ 3];
						if (g & G_E03) f[ 4] = tmp_wall[yl&1][zl][xl][ 3]; else f[ 4] =    async_buffers_src[ibuf][zl  ][yl+1][xl  ][ 4];
						if (g & G_E06) f[ 5] = tmp_wall[yl&1][zl][xl][ 6]; else f[ 5] = /* async_buffers_src[ibuf][zl-1][yl  ][xl  ][ 5]; */ tmp_wall[(yl  )&1][zl-1][xl  ][ 5];
						if (g & G_E05) f[ 6] = tmp_wall[yl&1][zl][xl][ 5]; else f[ 6] = /* async_buffers_src[ibuf][zl+1][yl  ][xl  ][ 6]; */ tmp_wall[(yl  )&1][zl+1][xl  ][ 6];
						if (g & G_E10) f[ 7] = tmp_wall[yl&1][zl][xl][10]; else f[ 7] = /* async_buffers_src[ibuf][zl  ][yl-1][xl-1][ 7]; */ tmp_wall[(yl-1)&1][zl  ][xl-1][ 7];
						if (g & G_E09) f[ 8] = tmp_wall[yl&1][zl][xl][ 9]; else f[ 8] = /* async_buffers_src[ibuf][zl  ][yl-1][xl+1][ 8]; */ tmp_wall[(yl-1)&1][zl  ][xl+1][ 8];
						if (g & G_E08) f[ 9] = tmp_wall[yl&1][zl][xl][ 8]; else f[ 9] =    async_buffers_src[ibuf][zl  ][yl+1][xl-1][ 9];
						if (g & G_E07) f[10] = tmp_wall[yl&1][zl][xl][ 7]; else f[10] =    async_buffers_src[ibuf][zl  ][yl+1][xl+1][10];
						if (g & G_E14) f[11] = tmp_wall[yl&1][zl][xl][14]; else f[11] = /* async_buffers_src[ibuf][zl-1][yl  ][xl-1][11]; */ tmp_wall[(yl  )&1][zl-1][xl-1][11];
						if (g & G_E13) f[12] = tmp_wall[yl&1][zl][xl][13]; else f[12] = /* async_buffers_src[ibuf][zl-1][yl  ][xl+1][12]; */ tmp_wall[(yl  )&1][zl-1][xl+1][12];
						if (g & G_E12) f[13] = tmp_wall[yl&1][zl][xl][12]; else f[13] = /* async_buffers_src[ibuf][zl+1][yl  ][xl-1][13]; */ tmp_wall[(yl  )&1][zl+1][xl-1][13];
						if (g & G_E11) f[14] = tmp_wall[yl&1][zl][xl][11]; else f[14] = /* async_buffers_src[ibuf][zl+1][yl  ][xl+1][14]; */ tmp_wall[(yl  )&1][zl+1][xl+1][14];
						if (g & G_E18) f[15] = tmp_wall[yl&1][zl][xl][18]; else f[15] = /* async_buffers_src[ibuf][zl-1][yl-1][xl  ][15]; */ tmp_wall[(yl-1)&1][zl-1][xl  ][15];
						if (g & G_E17) f[16] = tmp_wall[yl&1][zl][xl][17]; else f[16] =    async_buffers_src[ibuf][zl-1][yl+1][xl  ][16];
						if (g & G_E16) f[17] = tmp_wall[yl&1][zl][xl][16]; else f[17] = /* async_buffers_src[ibuf][zl+1][yl-1][xl  ][17]; */ tmp_wall[(yl-1)&1][zl+1][xl  ][17];
						if (g & G_E15) f[18] = tmp_wall[yl&1][zl][xl][15]; else f[18] =    async_buffers_src[ibuf][zl+1][yl+1][xl  ][18];

						// Imposed lid velocity
						if (z == arg_opal->Config->Lx-1) {
							f[13] += k0;
							f[14] -= k0;
						}
					} else {
						// Pull propagation
						f[ 1] = /* async_buffers_src[ibuf][zl  ][yl  ][xl-1][ 1]; */ tmp_wall[(yl  )&1][zl  ][xl-1][ 1];
						f[ 2] = /* async_buffers_src[ibuf][zl  ][yl  ][xl+1][ 2]; */ tmp_wall[(yl  )&1][zl  ][xl+1][ 2];
						f[ 3] = /* async_buffers_src[ibuf][zl  ][yl-1][xl  ][ 3]; */ tmp_wall[(yl-1)&1][zl  ][xl  ][ 3];
						f[ 4] =    async_buffers_src[ibuf][zl  ][yl+1][xl  ][ 4];
						f[ 5] = /* async_buffers_src[ibuf][zl-1][yl  ][xl  ][ 5]; */ tmp_wall[(yl  )&1][zl-1][xl  ][ 5];
						f[ 6] = /* async_buffers_src[ibuf][zl+1][yl  ][xl  ][ 6]; */ tmp_wall[(yl  )&1][zl+1][xl  ][ 6];
						f[ 7] = /* async_buffers_src[ibuf][zl  ][yl-1][xl-1][ 7]; */ tmp_wall[(yl-1)&1][zl  ][xl-1][ 7];
						f[ 8] = /* async_buffers_src[ibuf][zl  ][yl-1][xl+1][ 8]; */ tmp_wall[(yl-1)&1][zl  ][xl+1][ 8];
						f[ 9] =    async_buffers_src[ibuf][zl  ][yl+1][xl-1][ 9];
						f[10] =    async_buffers_src[ibuf][zl  ][yl+1][xl+1][10];
						f[11] = /* async_buffers_src[ibuf][zl-1][yl  ][xl-1][11]; */ tmp_wall[(yl  )&1][zl-1][xl-1][11];
						f[12] = /* async_buffers_src[ibuf][zl-1][yl  ][xl+1][12]; */ tmp_wall[(yl  )&1][zl-1][xl+1][12];
						f[13] = /* async_buffers_src[ibuf][zl+1][yl  ][xl-1][13]; */ tmp_wall[(yl  )&1][zl+1][xl-1][13];
						f[14] = /* async_buffers_src[ibuf][zl+1][yl  ][xl+1][14]; */ tmp_wall[(yl  )&1][zl+1][xl+1][14];
						f[15] = /* async_buffers_src[ibuf][zl-1][yl-1][xl  ][15]; */ tmp_wall[(yl-1)&1][zl-1][xl  ][15];
						f[16] =    async_buffers_src[ibuf][zl-1][yl+1][xl  ][16];
						f[17] = /* async_buffers_src[ibuf][zl+1][yl-1][xl  ][17]; */ tmp_wall[(yl-1)&1][zl+1][xl  ][17];
						f[18] =    async_buffers_src[ibuf][zl+1][yl+1][xl  ][18];
					}

					// Computation of moments
					t0 = f[ 1] + f[ 2] + f[ 3] + f[ 4] + f[ 5] + f[ 6];
					t1 = f[ 7] + f[ 8] + f[ 9] + f[10] + f[11] + f[12] + f[13] + f[14]
					  + f[15] + f[16] + f[17] + f[18];
					rho = f[ 0] + t0 + t1;
					e = -30*f[ 0] - 11*t0 + 8*t1;
					eps = 12*f[ 0] - 4*t0 + t1;
					t0 = f[ 1] - f[ 2];
					t1 = f[ 7] - f[ 8] + f[ 9] - f[10] + f[11] - f[12] + f[13] - f[14];
					jx = t0 + t1;
					qx = -4*t0 + t1;
					t0 = f[ 3] - f[ 4];
					t1 = f[ 7] + f[ 8] - f[ 9] - f[10] + f[15] - f[16] + f[17] - f[18];
					jy = t0 + t1;
					qy = -4*t0 + t1;
					t0 = f[ 5] - f[ 6];
					t1 = f[11] + f[12] - f[13] - f[14] + f[15] + f[16] - f[17] - f[18];
					jz = t0 + t1;
					qz = -4*t0 + t1;

					// Default case
					t0 = 2*(f[ 1] + f[ 2]) - (f[ 3] + f[ 4] + f[ 5] + f[ 6]);
					t1 = (f[ 7] + f[ 8] + f[ 9] + f[10] + f[11] + f[12] + f[13] + f[14])
					 - 2*(f[15] + f[16] + f[17] + f[18]);
					px = t0 + t1;
					pix = -2*t0 + t1;
					t0 = (f[ 3] + f[ 4]) - (f[ 5] + f[ 6]);
					t1 = (f[ 7] + f[ 8] + f[ 9] + f[10]) - (f[11] + f[12] + f[13] + f[14]);
					pw = t0 + t1;
					piw = -2*t0 + t1;
					pxy = f[ 7] - f[ 8] - f[ 9] + f[10];
					pyz = f[15] - f[16] - f[17] + f[18];
					pxz = f[11] - f[12] - f[13] + f[14];
					mx = f[ 7] - f[ 8] + f[ 9] - f[10] - f[11] + f[12] - f[13] + f[14];
					my = -f[ 7] - f[ 8] + f[ 9] + f[10] + f[15] - f[16] + f[17] - f[18];
					mz = f[11] + f[12] - f[13] - f[14] - f[15] - f[16] + f[17] + f[18];

					// Collision
					t0 = jx*jx;
					t1 = jy*jy;
					t2 = jz*jz;
					e   += s1*(-11*rho + 19*(t0 + t1 + t2) - e);
					eps += s2*(3*rho - eps);
					qx  += s4*((-2.F/3)*jx - qx);
					qy  += s4*((-2.F/3)*jy - qy);
					qz  += s4*((-2.F/3)*jz - qz);
					px  += s9*(2*t0 - t1 - t2 - px);
					pix += -s10*pix;
					pw  += s9*(t1 - t2 - pw);
					piw += -s10*piw;
					pxy += s9*(jx*jy - pxy);
					pyz += s9*(jy*jz - pyz);
					pxz += s9*(jz*jx - pxz);
					mx  += -s16*mx;
					my  += -s16*my;
					mz  += -s16*mz;

					// Computation of updated density distribution
					f[ 0] = c0*rho - c1*e + c4*eps;

					t0 = c0*rho - c2*e - c5*eps;
					t1 = px - pix;
					t2 = pw - piw;
					f[ 1] = t0 + c9*t1 + c7*(jx - qx);
					f[ 2] = t0 + c9*t1 - c7*(jx - qx);
					f[ 3] = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
					f[ 4] = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
					f[ 5] = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
					f[ 6] = t0 - c10*t1 - c12*t2 - c7*(jz - qz);

					t0 = c0*rho + c3*e + c6*eps;
					t1 = c7*jx + c8*qx;
					t2 = c7*jy + c8*qy;
					t3 = c7*jz + c8*qz;
					f[ 7] = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
						+ c15*(mx - my);
					f[ 8] = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
						- c15*(mx + my);
					f[ 9] = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
						+ c15*(mx + my);
					f[10] = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
						- c15*(mx - my);
					f[11] = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
						- c15*(mx - mz);
					f[12] = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
						+ c15*(mx + mz);
					f[13] = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
						- c15*(mx + mz);
					f[14] = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
						+ c15*(mx - mz);
					f[15] = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
					f[16] = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
					f[17] = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
					f[18] = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);

					// Store results to local cube before putting back to global
					async_buffers_src[ibuf][zl][yl][xl][ 0] = f[ 0];
					async_buffers_src[ibuf][zl][yl][xl][ 1] = f[ 1];
					async_buffers_src[ibuf][zl][yl][xl][ 2] = f[ 2];
					async_buffers_src[ibuf][zl][yl][xl][ 3] = f[ 3];
					async_buffers_src[ibuf][zl][yl][xl][ 4] = f[ 4];
					async_buffers_src[ibuf][zl][yl][xl][ 5] = f[ 5];
					async_buffers_src[ibuf][zl][yl][xl][ 6] = f[ 6];
					async_buffers_src[ibuf][zl][yl][xl][ 7] = f[ 7];
					async_buffers_src[ibuf][zl][yl][xl][ 8] = f[ 8];
					async_buffers_src[ibuf][zl][yl][xl][ 9] = f[ 9];
					async_buffers_src[ibuf][zl][yl][xl][10] = f[10];
					async_buffers_src[ibuf][zl][yl][xl][11] = f[11];
					async_buffers_src[ibuf][zl][yl][xl][12] = f[12];
					async_buffers_src[ibuf][zl][yl][xl][13] = f[13];
					async_buffers_src[ibuf][zl][yl][xl][14] = f[14];
					async_buffers_src[ibuf][zl][yl][xl][15] = f[15];
					async_buffers_src[ibuf][zl][yl][xl][16] = f[16];
					async_buffers_src[ibuf][zl][yl][xl][17] = f[17];
					async_buffers_src[ibuf][zl][yl][xl][18] = f[18];

				); // end ACC_FUNC on compute_time

			} // end for xl
		} // if(pid < CUBE_LENGTH_Z)
	} // end for yl
}

/* routine to be executed on each PE */
void* task_routine_opal_compute(void *arg)
{
	mOS_dinval();

	int cid = __k1_get_cluster_id();
	int pid = __k1_get_cpu_id();
	int status = 0;
	uint64_t start;

	struct utask_arg_opal_compute* arg_opal = (struct utask_arg_opal_compute*) arg;

	int icubex, icubey, icubez;
	int ibuf = 0;

	int icube_begin = cid*NB_CUBES_PER_CLUSTER;
	int icube_end = (cid+1)*NB_CUBES_PER_CLUSTER;

	/* PROLOGUE : PE0 of each cluster enqueues 2 first async cube copies */
	if(pid == 0){
		for(int i = 0; i < min(ASYNC_PIPELINE_DEPTH, NB_CUBES_PER_CLUSTER); i++){
			deinterleave3(icube_begin+i, &icubex, &icubey, &icubez);
			ACC_TIME_FUNC(get_time,
				status |= opal_async_get_cube(&async_buffers_src[i][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES, icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, &async_events_get[i]);
			);
		}
	}

	/* KERNEL */
	// Macro pipeline through all cubes per cluster
	for(int icube = icube_begin; icube < icube_end; icube++, ibuf = (ibuf+1) % ASYNC_PIPELINE_DEPTH){

		// everyone deinterleave3(icube) to get the current {icubex, icubey, icubez}
		deinterleave3(icube, &icubex, &icubey, &icubez);

		// PE0 waits for cube's arrival, others PE too
		if (pid == 0) {
			ACC_TIME_FUNC(wait_time,
				status |= mppa_async_event_wait(&async_events_get[ibuf]);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to wait for getting cube. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}. \n", cid, pid, ibuf, icubex, icubey, icubez);
		}

		local_barrier()

		collide_cube(pid, icubex, icubey, icubez, ibuf, arg_opal);

		// make a barrier to gather results of local_cube[16][16][16][Q], before PE0 putting back to Dst
		local_barrier()

		if (pid == 0) {
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_cube(arg_opal->Dst, &async_buffers_src[ibuf][0][0][0][0], NODE_SIZE_BYTES, icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DST, &async_events_put[ibuf]);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to put cube to global memory. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, ibuf, icubex, icubey, icubez);

			// then enqueues next async_get of the remote 'icube+ASYNC_PIPELINE_DEPTH' (if exists) to the local next_ibuf
			if((icube_end - icube) > ASYNC_PIPELINE_DEPTH-1){
				int next_ibuf = (ibuf+ASYNC_PIPELINE_DEPTH) % ASYNC_PIPELINE_DEPTH;

				// wait for previous put of next_ibuf
				if((icube - icube_begin) > ASYNC_PIPELINE_DEPTH-1){
					ACC_TIME_FUNC(wait_time,
						status |= mppa_async_event_wait(&async_events_put[next_ibuf]);
					);
					CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to wait for previous put. "
							"ibuf = %d\n", cid, pid, ibuf);
				}

				// then enqueues next async_get of the remote 'icube+ASYNC_PIPELINE_DEPTH' (if exists) to the local next_ibuf
				deinterleave3((icube+ASYNC_PIPELINE_DEPTH), &icubex, &icubey, &icubez);
				ACC_TIME_FUNC(get_time,
					status |= opal_async_get_cube(&async_buffers_src[next_ibuf][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES, icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, &async_events_get[next_ibuf]);
				);
				CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to get next cube. "
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
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to wait for last puts[%d].\n", cid, pid, i);
		}
	}

	#if NB_CUBES_TRAILING > 0
	// works on cid, compute trailing cube by blocking call
	if(cid < NB_CUBES_TRAILING){
		int trailing_ibuf = (ibuf+1) % ASYNC_PIPELINE_DEPTH;
		deinterleave3(cid + (NB_CLUSTERS*NB_CUBES_PER_CLUSTER), &icubex, &icubey, &icubez);
		if(pid == 0){
			ACC_TIME_FUNC(get_time,
				status |= opal_async_get_cube(&async_buffers_src[trailing_ibuf][0][0][0][0], arg_opal->Src, NODE_SIZE_BYTES, icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_SRC, NULL);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to get trailing cube. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, trailing_ibuf, icubex, icubey, icubez);
		}

		local_barrier()
		collide_cube(pid, icubex, icubey, icubez, trailing_ibuf, arg_opal);
		local_barrier()

		if(pid == 0){
			ACC_TIME_FUNC(put_time,
				status |= opal_async_put_cube(arg_opal->Dst, &async_buffers_src[trailing_ibuf][0][0][0][0], NODE_SIZE_BYTES, icubex, icubey, icubez, OPAL_ASYNC_SEGMENT_DST, NULL);
			);
			CHECK_ERROR(status, "Cluster %d - PE %d : task_routine_opal_compute failed to put trailing cube to global memory. "
					"ibuf = %d - {icubex,icubey,icubez} = {%d, %d, %d}\n", cid, pid, trailing_ibuf, icubex, icubey, icubez);
		}
	}
	#endif // NB_CUBES_TRAILING > 0

	return NULL;
}

/*
 *  compute_async: async compute kernel function
 */
void compute_async(config* Config, off64_t Src, off64_t Dst, int step)
{
	struct utask_arg_opal_compute arg_opal = {
		.Config = Config,
		.Src = Src,
		.Dst = Dst,
		.step = step
	};

	utask_t utask[NB_CLIENTS];
	for (int i = 0; i < NB_CLIENTS; i++) {
		utask_create(&utask[i], NULL, task_routine_opal_compute, (void*)&arg_opal);
	}

	/* start routine on self. */
	task_routine_opal_compute((void*)&arg_opal);

	/* End of a timestep, join other threads */
	for (int i = 0; i < NB_CLIENTS; i++) {
		utask_join(utask[i], NULL);
	}

	mppa_rpc_barrier_all();
}
