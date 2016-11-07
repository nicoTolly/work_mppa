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
 *  main.c: main program function.
 *
 */
// #include <pencil.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "global.h"

// compute kernel
void compute(
	float Src[128][128][128][19], 
	float Dst[128][128][128][19],
	float f[128][128][128][19]
	);

int main(int argc, char **argv)
{

	// Source and Destination lattice array for time steps : [Lz][Ly][Lx][Q]
	// [128][128][128][19]
	float (*LatticeEven)[128][128][128][19];
	float (*LatticeOdd)[128][128][128][19];
	float (*f)[128][128][128][19];

	// Density[Lz][Ly][Lx], Velocity[Lz][Ly][Lx][D] and Delta[Lz][Ly][Lx]
	float (*Density)[128][128][128];
	float (*Velocity)[128][128][128][3];
	float (*Delta)[128][128][128];

	// Malloc
	LatticeEven = (float(*)[128][128][128][19])
			malloc(2097152*19*sizeof(float));
	if(LatticeEven == NULL) {
		printf("Could not allocate 'LatticeEven' array.\n"); return -1;
	}

	LatticeOdd = (float(*)[128][128][128][19]) 
			malloc(2097152*19*sizeof(float));
	if(LatticeOdd == NULL) {
		printf("Could not allocate 'LatticeOdd' array.\n"); return -1;
	}

	f = (float(*)[128][128][128][19]) 
			malloc(2097152*19*sizeof(float));
	if(f == NULL) {
		printf("Could not allocate 'f' array.\n"); return -1;
	}

	Density = (float(*)[128][128][128])
			malloc(2097152*sizeof(float));
	if(Density == NULL) {
		printf("Could not allocate 'Density' array.\n"); return -1;
	}

	Velocity = (float(*)[128][128][128][3])
			malloc(2097152*3*sizeof(float));
	if(Velocity == NULL) {
		printf("Could not allocate 'Velocity' array.\n"); return -1;
	}

	Delta = (float(*)[128][128][128])
			malloc(2097152*sizeof(float));
	if(Delta == NULL) {
		printf("Could not allocate 'Delta' array.\n"); return -1;
	}

	////////////////////////////////////////////////////////////////////////
	// COMPUTE : Loop on Duration
	////////////////////////////////////////////////////////////////////////	
	float (*Src)[128][128][128][19] = LatticeEven;
	float (*Dst)[128][128][128][19] = LatticeOdd;
	for(unsigned int step = 0; step <= 100; step++)
	{	
		// Swap Src | Dst pointers mapping on LatticeEven | LatticeOdd 
		// based on step's parity
		if(step & 1) {Src = LatticeOdd ; Dst = LatticeEven;}
		else         {Src = LatticeEven; Dst = LatticeOdd ;}

		// Launch compute kernel, only 'compute' use #pragma scop
		compute(*Src, *Dst, *f);

	} // end for step

	////////////////////////////////////////////////////////////////////////
	// Clean up
	////////////////////////////////////////////////////////////////////////	
	free((void*)LatticeEven);
	free((void*)LatticeOdd);
	free((void*)f);
	free((void*)Density);
	free((void*)Velocity);
	free((void*)Delta);

	return 0;
}

void next_step(
	float *f0, float *f1, float *f2, float *f3, float *f4, 
	float *f5, float *f6, float *f7, float *f8, float *f9, 
	float *f10, float *f11, float *f12, float *f13, float *f14, 
	float *f15, float *f16, float *f17, float *f18)
{
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
				    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;
	float t0, t1, t2, t3;

	#define f_p(i) f##i[0]
	#define c0 (1.F/19)

	// [1] Computation of moments
	t0 = f_p(1) + f_p(2) + f_p(3) + f_p(4) + f_p(5) + f_p(6);
	t1 = f_p(7) + f_p(8) + f_p(9) + f_p(10) + f_p(11) + f_p(12) + f_p(13) + f_p(14)
	  + f_p(15) + f_p(16) + f_p(17) + f_p(18);
	rho = f_p(0) + t0 + t1;
	e = -30*f_p(0) - 11*t0 + 8*t1; 
	eps = 12*f_p(0) - 4*t0 + t1;
	t0 = f_p(1) - f_p(2);
	t1 = f_p(7) - f_p(8) + f_p(9) - f_p(10) + f_p(11) - f_p(12) + f_p(13) - f_p(14);
	jx = t0 + t1;
	qx = -4*t0 + t1;
	t0 = f_p(3) - f_p(4);
	t1 = f_p(7) + f_p(8) - f_p(9) - f_p(10) + f_p(15) - f_p(16) + f_p(17) - f_p(18);
	jy = t0 + t1;
	qy = -4*t0 + t1;
	t0 = f_p(5) - f_p(6);
	t1 = f_p(11) + f_p(12) - f_p(13) - f_p(14) + f_p(15) + f_p(16) - f_p(17) - f_p(18);
	jz = t0 + t1;
	qz = -4*t0 + t1;

	// [2] Default case, compute new state
	t0 = 2*(f_p(1) + f_p(2)) - (f_p(3) + f_p(4) + f_p(5) + f_p(6));
	t1 = (f_p(7) + f_p(8) + f_p(9) + f_p(10) + f_p(11) + f_p(12) + f_p(13) + f_p(14))
	 - 2*(f_p(15) + f_p(16) + f_p(17) + f_p(18));
	px = t0 + t1;
	pix = -2*t0 + t1;
	t0 = (f_p(3) + f_p(4)) - (f_p(5) + f_p(6));
	t1 = (f_p(7) + f_p(8) + f_p(9) + f_p(10)) - (f_p(11) + f_p(12) + f_p(13) + f_p(14));
	pw = t0 + t1;
	piw = -2*t0 + t1;
	pxy = f_p(7) - f_p(8) - f_p(9) + f_p(10);
	pyz = f_p(15) - f_p(16) - f_p(17) + f_p(18);
	pxz = f_p(11) - f_p(12) - f_p(13) + f_p(14);
	mx = f_p(7) - f_p(8) + f_p(9) - f_p(10) - f_p(11) + f_p(12) - f_p(13) + f_p(14);
	my = -f_p(7) - f_p(8) + f_p(9) + f_p(10) + f_p(15) - f_p(16) + f_p(17) - f_p(18);
	mz = f_p(11) + f_p(12) - f_p(13) - f_p(14) - f_p(15) - f_p(16) + f_p(17) + f_p(18);

	// [3] Collision
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

	// [4] Computation of updated density distribution
	f_p(0) = c0*rho - c1*e + c4*eps;
	t0 = c0*rho - c2*e - c5*eps;
	t1 = px - pix;
	t2 = pw - piw;
	f_p(1) = t0 + c9*t1 + c7*(jx - qx);
	f_p(2) = t0 + c9*t1 - c7*(jx - qx);
	f_p(3) = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
	f_p(4) = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
	f_p(5) = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
	f_p(6) = t0 - c10*t1 - c12*t2 - c7*(jz - qz);
	t0 = c0*rho + c3*e + c6*eps;
	t1 = c7*jx + c8*qx;
	t2 = c7*jy + c8*qy;
	t3 = c7*jz + c8*qz;
	f_p(7) = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
		+ c15*(mx - my);
	f_p(8) = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
		- c15*(mx + my);
	f_p(9) = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
		+ c15*(mx + my);
	f_p(10) = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
		- c15*(mx - my);
	f_p(11) = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
		- c15*(mx - mz);
	f_p(12) = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
		+ c15*(mx + mz);
	f_p(13) = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
		- c15*(mx + mz);
	f_p(14) = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
		+ c15*(mx - mz);
	f_p(15) = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
	f_p(16) = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
	f_p(17) = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
	f_p(18) = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);
	#undef f_p
	#undef c0
}

/*
 *  compute: compute kernel function
 */
void compute(
	float Src[128][128][128][19], 
	float Dst[128][128][128][19],
	float f[128][128][128][19]
	)
{
	// Fetch
	// #pragma scop
	for(unsigned int z = 0; z < 128; z++){
		for(unsigned int y = 0; y < 128; y++){
			for(unsigned int x = 0; x < 128; x++){

				//////////// LOCAL VARIABLES //////////////////////////////////
				int g = 0;
				#define f(i) f[z][y][x][i] 
				///////////////////////////////////////////////////////////////

				if (x == 128-1) g = g | G_W1;
				if (x == 0)    g = g | G_W2;
				if (y == 128-1) g = g | G_W3;
				if (y == 0)    g = g | G_W4;
				if (z == 128-1) g = g | G_W5;
				if (z == 0)    g = g | G_W6;

				// Fetch source density distribution
				f(0) = Src[z][y][x][0];
				if (g & G_BND) {
					// Simple bounce-back boundary condition
					if (g & G_E02) f(1)  = Src[z][y][x][ 2]; else f(1)  = Src[z  ][y  ][x-1][ 1];
					if (g & G_E01) f(2)  = Src[z][y][x][ 1]; else f(2)  = Src[z  ][y  ][x+1][ 2];
					if (g & G_E04) f(3)  = Src[z][y][x][ 4]; else f(3)  = Src[z  ][y-1][x  ][ 3];
					if (g & G_E03) f(4)  = Src[z][y][x][ 3]; else f(4)  = Src[z  ][y+1][x  ][ 4];
					if (g & G_E06) f(5)  = Src[z][y][x][ 6]; else f(5)  = Src[z-1][y  ][x  ][ 5];
					if (g & G_E05) f(6)  = Src[z][y][x][ 5]; else f(6)  = Src[z+1][y  ][x  ][ 6];
					if (g & G_E10) f(7)  = Src[z][y][x][10]; else f(7)  = Src[z  ][y-1][x-1][ 7];
					if (g & G_E09) f(8)  = Src[z][y][x][ 9]; else f(8)  = Src[z  ][y-1][x+1][ 8];
					if (g & G_E08) f(9)  = Src[z][y][x][ 8]; else f(9)  = Src[z  ][y+1][x-1][ 9];
					if (g & G_E07) f(10) = Src[z][y][x][ 7]; else f(10) = Src[z  ][y+1][x+1][10];
					if (g & G_E14) f(11) = Src[z][y][x][14]; else f(11) = Src[z-1][y  ][x-1][11];
					if (g & G_E13) f(12) = Src[z][y][x][13]; else f(12) = Src[z-1][y  ][x+1][12];
					if (g & G_E12) f(13) = Src[z][y][x][12]; else f(13) = Src[z+1][y  ][x-1][13];
					if (g & G_E11) f(14) = Src[z][y][x][11]; else f(14) = Src[z+1][y  ][x+1][14];
					if (g & G_E18) f(15) = Src[z][y][x][18]; else f(15) = Src[z-1][y-1][x  ][15];
					if (g & G_E17) f(16) = Src[z][y][x][17]; else f(16) = Src[z-1][y+1][x  ][16];
					if (g & G_E16) f(17) = Src[z][y][x][16]; else f(17) = Src[z+1][y-1][x  ][17];
					if (g & G_E15) f(18) = Src[z][y][x][15]; else f(18) = Src[z+1][y+1][x  ][18];

					// Imposed lid velocity
					if (z == 128-1) {
						f(13) += k0;
						f(14) -= k0;
					}
				} else {
					// Pull propagation
					f(1)  = Src[z  ][y  ][x-1][ 1];
					f(2)  = Src[z  ][y  ][x+1][ 2];
					f(3)  = Src[z  ][y-1][x  ][ 3];
					f(4)  = Src[z  ][y+1][x  ][ 4];
					f(5)  = Src[z-1][y  ][x  ][ 5];
					f(6)  = Src[z+1][y  ][x  ][ 6];
					f(7)  = Src[z  ][y-1][x-1][ 7];
					f(8)  = Src[z  ][y-1][x+1][ 8];
					f(9)  = Src[z  ][y+1][x-1][ 9];
					f(10) = Src[z  ][y+1][x+1][10];
					f(11) = Src[z-1][y  ][x-1][11];
					f(12) = Src[z-1][y  ][x+1][12];
					f(13) = Src[z+1][y  ][x-1][13];
					f(14) = Src[z+1][y  ][x+1][14];
					f(15) = Src[z-1][y-1][x  ][15];
					f(16) = Src[z-1][y+1][x  ][16];
					f(17) = Src[z+1][y-1][x  ][17];
					f(18) = Src[z+1][y+1][x  ][18];
				}

				#undef f
			} // end for x
		} // end for y
	} // end for z
	// #pragma endscop

	// Compute all nodes for the current step
	#pragma scop
	for(unsigned int z = 0; z < 128; z++){
		for(unsigned int y = 0; y < 128; y++){
			for(unsigned int x = 0; x < 128; x++){

				//////////// LOCAL VARIABLES //////////////////////////////////
				// float rho, e, eps, jx, qx, jy, qy, jz, qz,
				//     px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;
				// float t0, t1, t2, t3;
				float f0, f1, f2, f3, f4, f5, f6,
    				f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18;
				#define f(i) f##i 
				///////////////////////////////////////////////////////////////

				// Fetch source density distribution (already done before)
				f(0)  = f[z][y][x][ 0];
				f(1)  = f[z][y][x][ 1];
				f(2)  = f[z][y][x][ 2];
				f(3)  = f[z][y][x][ 3];
				f(4)  = f[z][y][x][ 4];
				f(5)  = f[z][y][x][ 5];
				f(6)  = f[z][y][x][ 6];
				f(7)  = f[z][y][x][ 7];
				f(8)  = f[z][y][x][ 8];
				f(9)  = f[z][y][x][ 9];
				f(10) = f[z][y][x][10];
				f(11) = f[z][y][x][11];
				f(12) = f[z][y][x][12];
				f(13) = f[z][y][x][13];
				f(14) = f[z][y][x][14];
				f(15) = f[z][y][x][15];
				f(16) = f[z][y][x][16];
				f(17) = f[z][y][x][17];
				f(18) = f[z][y][x][18];

				next_step(&f(0), &f(1), &f(2), &f(3), &f(4), &f(5), &f(6), &f(7), 
					&f(8), &f(9), &f(10), &f(11), &f(12), &f(13), &f(14), &f(15), 
					&f(16), &f(17), &f(18));

				// Store
				Dst[z][y][x][ 0] = f(0);
				Dst[z][y][x][ 1] = f(1);
				Dst[z][y][x][ 2] = f(2);
				Dst[z][y][x][ 3] = f(3);
				Dst[z][y][x][ 4] = f(4);
				Dst[z][y][x][ 5] = f(5);
				Dst[z][y][x][ 6] = f(6);
				Dst[z][y][x][ 7] = f(7);
				Dst[z][y][x][ 8] = f(8);
				Dst[z][y][x][ 9] = f(9);
				Dst[z][y][x][10] = f(10);
				Dst[z][y][x][11] = f(11);
				Dst[z][y][x][12] = f(12);
				Dst[z][y][x][13] = f(13);
				Dst[z][y][x][14] = f(14);
				Dst[z][y][x][15] = f(15);
				Dst[z][y][x][16] = f(16);
				Dst[z][y][x][17] = f(17);
				Dst[z][y][x][18] = f(18);

				#undef f
			} // end for x
		} // end for y
	} // end for z
	#pragma endscop
}
