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

#include "opal.h"
#include "global.h"

// #define COMPUTE_BOUNDARY_ON_HOST

#define TWO_LATTICE_MERGED

// init kernel
void init(config* Config, results* Results);

void compute_merged(
	config* Config, results* Results,
	float Lattices[2][Config->Lz][Config->Ly][Config->Lx][Q], 
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
	);

// compute kernel
void compute(
	config* Config, 
	float Src[128][128][128][Q], 
	float Dst[128][128][128][Q]
#ifdef COMPUTE_BOUNDARY_ON_HOST	
	, float f[128][128][128][Q]
#endif	
	);

// monitor kernel
void monitor(
	config* Config, results* Results, 
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
	);

// extraction kernel
void extraction(
	config* Config, 
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Density[Config->Lz][Config->Ly][Config->Lx],
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D]
	);


int main(int argc, char **argv)
{
	config* Config;
	data* Data;
	results* Results;
	timespec Start, Stop;

	// Start timer
	clock_gettime(CLOCK_REALTIME, &Start);
	
	// Create containers
	Config = getConfig(argc, argv);
	Data = createData(Config);
	Results = createResults(Config);

	// Source and Destination lattice array for time steps : [Lz][Ly][Lx][Q]
#ifdef TWO_LATTICE_MERGED
	float (*Lattices)[2][Config->Lz][Config->Ly][Config->Lx][Q];
#else
	float (*LatticeEven)[Config->Lz][Config->Ly][Config->Lx][Q];
	float (*LatticeOdd)[Config->Lz][Config->Ly][Config->Lx][Q];
#endif

#ifdef COMPUTE_BOUNDARY_ON_HOST
	float (*f)[Config->Lz][Config->Ly][Config->Lx][Q];
#endif

	// Density[Lz][Ly][Lx], Velocity[Lz][Ly][Lx][D] and Delta[Lz][Ly][Lx]
	float (*Density)[Config->Lz][Config->Ly][Config->Lx];
	float (*Velocity)[Config->Lz][Config->Ly][Config->Lx][D];
	float (*Delta)[Config->Lz][Config->Ly][Config->Lx];

	// Malloc
#ifdef TWO_LATTICE_MERGED
	Lattices = (float(*)[2][Config->Lz][Config->Ly][Config->Lx][Q])
			malloc(2*Results->DomainSize*Q*sizeof(float));
	if(Lattices == NULL) {
		printf("Could not allocate 'Lattices' array.\n"); return -1;
	}
#else
	LatticeEven = (float(*)[Config->Lz][Config->Ly][Config->Lx][Q])
			malloc(Results->DomainSize*Q*sizeof(float));
	if(LatticeEven == NULL) {
		printf("Could not allocate 'LatticeEven' array.\n"); return -1;
	}

	LatticeOdd = (float(*)[Config->Lz][Config->Ly][Config->Lx][Q]) 
			malloc(Results->DomainSize*Q*sizeof(float));
	if(LatticeOdd == NULL) {
		printf("Could not allocate 'LatticeOdd' array.\n"); return -1;
	}
#endif

#ifdef COMPUTE_BOUNDARY_ON_HOST
	f = (float(*)[Config->Lz][Config->Ly][Config->Lx][Q]) 
			malloc(Results->DomainSize*Q*sizeof(float));
	if(f == NULL) {
		printf("Could not allocate 'f' array.\n"); return -1;
	}
#endif

	Density = (float(*)[Config->Lz][Config->Ly][Config->Lx])
			malloc(Results->DomainSize*sizeof(float));
	if(Density == NULL) {
		printf("Could not allocate 'Density' array.\n"); return -1;
	}

	Velocity = (float(*)[Config->Lz][Config->Ly][Config->Lx][D])
			malloc(Results->DomainSize*D*sizeof(float));
	if(Velocity == NULL) {
		printf("Could not allocate 'Velocity' array.\n"); return -1;
	}

	Delta = (float(*)[Config->Lz][Config->Ly][Config->Lx])
			malloc(Results->DomainSize*sizeof(float));
	if(Delta == NULL) {
		printf("Could not allocate 'Delta' array.\n"); return -1;
	}

	// Begin log
	printBanner(Config, Results);

	// Print result at step 0 
	printResults(Results, 0);

	////////////////////////////////////////////////////////////////////////
	// INIT
	////////////////////////////////////////////////////////////////////////
	init(Config, Results);

	////////////////////////////////////////////////////////////////////////
	// COMPUTE : Loop on Duration
	////////////////////////////////////////////////////////////////////////	
#ifdef TWO_LATTICE_MERGED
	// Attention : this compute_merged will to 1000 timesteps hardcoded, period=100
	compute_merged(Config, Results, *Lattices, *Velocity, *Delta);
#else /* TWO_LATTICE_MERGED */

	float (*Src)[Config->Lz][Config->Ly][Config->Lx][Q] = LatticeEven;
	float (*Dst)[Config->Lz][Config->Ly][Config->Lx][Q] = LatticeOdd;
	for(unsigned int step = 0; step <= Config->Duration && !(Results->Converged); 
																							step++)
	{	
		// Swap Src | Dst pointers mapping on LatticeEven | LatticeOdd 
		// based on step's parity
		if(step & 1) {Src = LatticeOdd ; Dst = LatticeEven;}
		else         {Src = LatticeEven; Dst = LatticeOdd ;}

		// Launch compute kernel, only 'compute' use #pragma scop
#ifdef COMPUTE_BOUNDARY_ON_HOST
		compute(Config, *Src, *Dst, *f);
#else
		compute(Config, *Src, *Dst);
#endif

		// Monitoring: Do every 'Period' steps
		if (step > 0 && (step % Config->Period == 0)) {
			monitor(Config, Results, *Src, *Velocity, *Delta);

			// print progress and exports image
			printResults(Results, step);
			if(Config->Images) exportImage(Config, Results, step);
		}
	} // end for step
#endif /* end TWO_LATTICE_MERGED */

	// Optional simulation results exportation at the end
	if (Config->Export) {
#ifdef TWO_LATTICE_MERGED
		extraction(Config, *Lattices[0], *Density, *Velocity);
#else
		extraction(Config, *Src, *Density, *Velocity);
#endif // end TWO_LATTICE_MERGED
		exportResults(Results, *Density, *Velocity);
	} 

	////////////////////////////////////////////////////////////////////////
	// Clean up
	////////////////////////////////////////////////////////////////////////	
#ifdef TWO_LATTICE_MERGED
	free((void*)Lattices);
#else
	free((void*)LatticeEven);
	free((void*)LatticeOdd);
#endif // end TWO_LATTICE_MERGED

#ifdef COMPUTE_BOUNDARY_ON_HOST	
	free((void*)f);
#endif	
	free((void*)Density);
	free((void*)Velocity);
	free((void*)Delta);
	clearData(Data);
	free(Config);

	// Stop timer
	clock_gettime(CLOCK_REALTIME, &Stop);

	// Finalise log
	printSummary(Results, Config->Duration, &Start, &Stop);
	clearResults(Results);

	return EXIT_SUCCESS;
}

/*
 *  getGeometry: encode local geometry informations in a bitfield.
 *  NOTE: In case of more complex geometries, the bitfields might be created
 *  during initialisation and stored in device memory.
 */

static inline int getGeometry(config* Config, int x, int y, int z)
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

static inline void writePixel(results* Results, config* Config, int x, int z, float u)
{
	int h;
	unsigned char s, r, g, b;

	unsigned char* Bitmap = (unsigned char*)Results->Bitmap; 
	Bitmap += 3*(x + z*Config->Lx);
	h = (int)u % 6;
	s = (unsigned char)(255.F*(u - h));
	#if 0
	switch(h) {
	  case 0: r = 255; g = s; b = 0; break;
	  case 1: r = 255 - s; g = 255; b = 0; break;
	  case 2: r = 0; g = 255; b = s; break;
	  case 3: r = 0; g = 255 - s; b = 255; break;
	  case 4: r = s; g = 0; b = 255; break;
	  case 5: r = 255; g = 0; b = 255 - s; break;
	  default: r = 0; g = 0; b = 0;
	}
	#else
	if (h == 0) { r = 255; g = s; b = 0; }
	else if (h == 1) { r = 255 - s; g = 255; b = 0; }
	else if (h == 2) { r = 0; g = 255; b = s; }
	else if (h == 3) { r = 0; g = 255 - s; b = 255; }
	else if (h == 4) { r = s; g = 0; b = 255; }
	else if (h == 5) { r = 255; g = 0; b = 255 - s; }
	else { r = 0; g = 0; b = 0; }
	#endif
	Bitmap[0] = r;
	Bitmap[1] = g;
	Bitmap[2] = b;
}

/*
 *  Init: init kernel 
 */
void init(config* Config, results* Results){
	// Write bitmap at step 0
	if (Results->Bitmap) {
		for(unsigned int z = 0; z < Config->Lz; z++){
			for(unsigned int x = 0; x < Config->Lx; x++){
				writePixel(Results, Config, x, z, 0); // Set Bitmap
			}
		}
	}
	// Export image at step 0
	if(Config->Images) exportImage(Config, Results, 0);
}

void compute_merged(
	config* Config, results* Results,
	float Lattices[2][128][128][128][Q], 
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
)
{
	// for(unsigned int step = 0; step <= Config->Duration && !(Results->Converged); step += Config->Period)
	for(unsigned int step = 0; step < 1000; step += 100)
	{
		// Monitor
		if(step > 0){
			monitor(Config, Results, Lattices[0], Velocity, Delta);
			// print progress and exports image
			printResults(Results, step);
			if(Config->Images) exportImage(Config, Results, step);
		}

		#pragma scop
		#define MAX_SMALL_STEPS 100
		// run ''Period step then do a monitor
		// for(unsigned int smallstep = step; smallstep < step+Config->Period; smallstep++)
		for(unsigned int smallstep = 0; smallstep <= MAX_SMALL_STEPS-1; smallstep++)
		{
			for(unsigned int z = 0; z < 128; z++){
				for(unsigned int y = 0; y < 128; y++){
					for(unsigned int x = 0; x < 128; x++){

						//////////// LOCAL VARIABLES //////////////////////////////////
						// Moments
						float rho, e, eps, jx, qx, jy, qy, jz, qz,
						    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

						// Auxiliary variables
						float t0, t1, t2, t3;

						float f0, f1, f2, f3, f4, f5, f6,
							f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18;
						int g = 0;
						#define f(i) f##i
						///////////////////////////////////////////////////////////////

						if (x == 128-1) g = g | G_W1;
						if (x == 0)    g = g | G_W2;
						if (y == 128-1) g = g | G_W3;
						if (y == 0)    g = g | G_W4;
						if (z == 128-1) g = g | G_W5;
						if (z == 0)    g = g | G_W6;

						// Fetch source density distribution
						f(0) = Lattices[smallstep%2][z][y][x][0];
						if (g & G_BND) {
							// Simple bounce-back boundary condition
							if (g & G_E02) f(1)  = Lattices[smallstep%2][z][y][x][ 2]; else f(1)  = Lattices[smallstep%2][z  ][y  ][x-1][ 1];
							if (g & G_E01) f(2)  = Lattices[smallstep%2][z][y][x][ 1]; else f(2)  = Lattices[smallstep%2][z  ][y  ][x+1][ 2];
							if (g & G_E04) f(3)  = Lattices[smallstep%2][z][y][x][ 4]; else f(3)  = Lattices[smallstep%2][z  ][y-1][x  ][ 3];
							if (g & G_E03) f(4)  = Lattices[smallstep%2][z][y][x][ 3]; else f(4)  = Lattices[smallstep%2][z  ][y+1][x  ][ 4];
							if (g & G_E06) f(5)  = Lattices[smallstep%2][z][y][x][ 6]; else f(5)  = Lattices[smallstep%2][z-1][y  ][x  ][ 5];
							if (g & G_E05) f(6)  = Lattices[smallstep%2][z][y][x][ 5]; else f(6)  = Lattices[smallstep%2][z+1][y  ][x  ][ 6];
							if (g & G_E10) f(7)  = Lattices[smallstep%2][z][y][x][10]; else f(7)  = Lattices[smallstep%2][z  ][y-1][x-1][ 7];
							if (g & G_E09) f(8)  = Lattices[smallstep%2][z][y][x][ 9]; else f(8)  = Lattices[smallstep%2][z  ][y-1][x+1][ 8];
							if (g & G_E08) f(9)  = Lattices[smallstep%2][z][y][x][ 8]; else f(9)  = Lattices[smallstep%2][z  ][y+1][x-1][ 9];
							if (g & G_E07) f(10) = Lattices[smallstep%2][z][y][x][ 7]; else f(10) = Lattices[smallstep%2][z  ][y+1][x+1][10];
							if (g & G_E14) f(11) = Lattices[smallstep%2][z][y][x][14]; else f(11) = Lattices[smallstep%2][z-1][y  ][x-1][11];
							if (g & G_E13) f(12) = Lattices[smallstep%2][z][y][x][13]; else f(12) = Lattices[smallstep%2][z-1][y  ][x+1][12];
							if (g & G_E12) f(13) = Lattices[smallstep%2][z][y][x][12]; else f(13) = Lattices[smallstep%2][z+1][y  ][x-1][13];
							if (g & G_E11) f(14) = Lattices[smallstep%2][z][y][x][11]; else f(14) = Lattices[smallstep%2][z+1][y  ][x+1][14];
							if (g & G_E18) f(15) = Lattices[smallstep%2][z][y][x][18]; else f(15) = Lattices[smallstep%2][z-1][y-1][x  ][15];
							if (g & G_E17) f(16) = Lattices[smallstep%2][z][y][x][17]; else f(16) = Lattices[smallstep%2][z-1][y+1][x  ][16];
							if (g & G_E16) f(17) = Lattices[smallstep%2][z][y][x][16]; else f(17) = Lattices[smallstep%2][z+1][y-1][x  ][17];
							if (g & G_E15) f(18) = Lattices[smallstep%2][z][y][x][15]; else f(18) = Lattices[smallstep%2][z+1][y+1][x  ][18];

							// Imposed lid velocity
							if (z == 128-1) {
								f(13) += k0;
								f(14) -= k0;
							}
						} else {
							// Pull propagation
							f(1)  = Lattices[smallstep%2][z  ][y  ][x-1][ 1];
							f(2)  = Lattices[smallstep%2][z  ][y  ][x+1][ 2];
							f(3)  = Lattices[smallstep%2][z  ][y-1][x  ][ 3];
							f(4)  = Lattices[smallstep%2][z  ][y+1][x  ][ 4];
							f(5)  = Lattices[smallstep%2][z-1][y  ][x  ][ 5];
							f(6)  = Lattices[smallstep%2][z+1][y  ][x  ][ 6];
							f(7)  = Lattices[smallstep%2][z  ][y-1][x-1][ 7];
							f(8)  = Lattices[smallstep%2][z  ][y-1][x+1][ 8];
							f(9)  = Lattices[smallstep%2][z  ][y+1][x-1][ 9];
							f(10) = Lattices[smallstep%2][z  ][y+1][x+1][10];
							f(11) = Lattices[smallstep%2][z-1][y  ][x-1][11];
							f(12) = Lattices[smallstep%2][z-1][y  ][x+1][12];
							f(13) = Lattices[smallstep%2][z+1][y  ][x-1][13];
							f(14) = Lattices[smallstep%2][z+1][y  ][x+1][14];
							f(15) = Lattices[smallstep%2][z-1][y-1][x  ][15];
							f(16) = Lattices[smallstep%2][z-1][y+1][x  ][16];
							f(17) = Lattices[smallstep%2][z+1][y-1][x  ][17];
							f(18) = Lattices[smallstep%2][z+1][y+1][x  ][18];
						}					

						#define c0 (1.F/19)
						#define c1 (5.F/399)
						#define c2 (11.F/2394)
						#define c3 (4.F/1197)
						#define c4 (1.F/21)
						#define c5 (1.F/63)
						#define c6 (1.F/252)
						#define c7 (1.F/10)
						#define c8 (1.F/40)
						#define c9 (1.F/18)
						#define c10 (1.F/36)
						#define c11 (1.F/72)
						#define c12 (1.F/12)
						#define c13 (1.F/24)
						#define c14 (1.F/4)
						#define c15 (1.F/8)

						// [1] Computation of moments
						t0 = f(1) + f(2) + f(3) + f(4) + f(5) + f(6);
						t1 = f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14)
						  + f(15) + f(16) + f(17) + f(18);
						rho = f(0) + t0 + t1;
						e = -30*f(0) - 11*t0 + 8*t1; 
						eps = 12*f(0) - 4*t0 + t1;
						t0 = f(1) - f(2);
						t1 = f(7) - f(8) + f(9) - f(10) + f(11) - f(12) + f(13) - f(14);
						jx = t0 + t1;
						qx = -4*t0 + t1;
						t0 = f(3) - f(4);
						t1 = f(7) + f(8) - f(9) - f(10) + f(15) - f(16) + f(17) - f(18);
						jy = t0 + t1;
						qy = -4*t0 + t1;
						t0 = f(5) - f(6);
						t1 = f(11) + f(12) - f(13) - f(14) + f(15) + f(16) - f(17) - f(18);
						jz = t0 + t1;
						qz = -4*t0 + t1;

						// [2] Default case, compute new state
						t0 = 2*(f(1) + f(2)) - (f(3) + f(4) + f(5) + f(6));
						t1 = (f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14))
						 - 2*(f(15) + f(16) + f(17) + f(18));
						px = t0 + t1;
						pix = -2*t0 + t1;
						t0 = (f(3) + f(4)) - (f(5) + f(6));
						t1 = (f(7) + f(8) + f(9) + f(10)) - (f(11) + f(12) + f(13) + f(14));
						pw = t0 + t1;
						piw = -2*t0 + t1;
						pxy = f(7) - f(8) - f(9) + f(10);
						pyz = f(15) - f(16) - f(17) + f(18);
						pxz = f(11) - f(12) - f(13) + f(14);
						mx = f(7) - f(8) + f(9) - f(10) - f(11) + f(12) - f(13) + f(14);
						my = -f(7) - f(8) + f(9) + f(10) + f(15) - f(16) + f(17) - f(18);
						mz = f(11) + f(12) - f(13) - f(14) - f(15) - f(16) + f(17) + f(18);

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
						f(0) = c0*rho - c1*e + c4*eps;
						t0 = c0*rho - c2*e - c5*eps;
						t1 = px - pix;
						t2 = pw - piw;
						f(1) = t0 + c9*t1 + c7*(jx - qx);
						f(2) = t0 + c9*t1 - c7*(jx - qx);
						f(3) = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
						f(4) = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
						f(5) = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
						f(6) = t0 - c10*t1 - c12*t2 - c7*(jz - qz);
						t0 = c0*rho + c3*e + c6*eps;
						t1 = c7*jx + c8*qx;
						t2 = c7*jy + c8*qy;
						t3 = c7*jz + c8*qz;
						f(7) = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
							+ c15*(mx - my);
						f(8) = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
							- c15*(mx + my);
						f(9) = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
							+ c15*(mx + my);
						f(10) = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
							- c15*(mx - my);
						f(11) = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
							- c15*(mx - mz);
						f(12) = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
							+ c15*(mx + mz);
						f(13) = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
							- c15*(mx + mz);
						f(14) = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
							+ c15*(mx - mz);
						f(15) = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
						f(16) = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
						f(17) = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
						f(18) = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);

						// Store
						Lattices[(smallstep+1)%2][z][y][x][ 0] = f(0);
						Lattices[(smallstep+1)%2][z][y][x][ 1] = f(1);
						Lattices[(smallstep+1)%2][z][y][x][ 2] = f(2);
						Lattices[(smallstep+1)%2][z][y][x][ 3] = f(3);
						Lattices[(smallstep+1)%2][z][y][x][ 4] = f(4);
						Lattices[(smallstep+1)%2][z][y][x][ 5] = f(5);
						Lattices[(smallstep+1)%2][z][y][x][ 6] = f(6);
						Lattices[(smallstep+1)%2][z][y][x][ 7] = f(7);
						Lattices[(smallstep+1)%2][z][y][x][ 8] = f(8);
						Lattices[(smallstep+1)%2][z][y][x][ 9] = f(9);
						Lattices[(smallstep+1)%2][z][y][x][10] = f(10);
						Lattices[(smallstep+1)%2][z][y][x][11] = f(11);
						Lattices[(smallstep+1)%2][z][y][x][12] = f(12);
						Lattices[(smallstep+1)%2][z][y][x][13] = f(13);
						Lattices[(smallstep+1)%2][z][y][x][14] = f(14);
						Lattices[(smallstep+1)%2][z][y][x][15] = f(15);
						Lattices[(smallstep+1)%2][z][y][x][16] = f(16);
						Lattices[(smallstep+1)%2][z][y][x][17] = f(17);
						Lattices[(smallstep+1)%2][z][y][x][18] = f(18);

						#undef c0
						#undef c1
						#undef c2
						#undef c3
						#undef c4
						#undef c5
						#undef c6
						#undef c7
						#undef c8
						#undef c9
						#undef c10
						#undef c11
						#undef c12
						#undef c13
						#undef c14
						#undef c15
						#undef f
					} // end for x
				} // end for y
			} // end for z
		} // end for smallstep
		#undef MAX_SMALL_STEPS
		#pragma endscop
	} // end for step
	monitor(Config, Results, Lattices[0], Velocity, Delta);
	// print progress and exports image
	printResults(Results, 1000);
	if(Config->Images) exportImage(Config, Results, 1000);
}

/*
 *  compute: compute kernel function
 */
#ifndef COMPUTE_BOUNDARY_ON_HOST
// compute with boundary condition on device
void compute(
	config* Config, 
	// float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	// float Dst[Config->Lz][Config->Ly][Config->Lx][Q]
	float Src[128][128][128][Q], 
	float Dst[128][128][128][Q])
{
	// Fetch
	// #pragma scop
	for(unsigned int z = 0; z < 128; z++){
		for(unsigned int y = 0; y < 128; y++){
			for(unsigned int x = 0; x < 128; x++){

				//////////// LOCAL VARIABLES //////////////////////////////////
				// Moments
				float rho, e, eps, jx, qx, jy, qy, jz, qz,
				    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

				// Auxiliary variables
				float t0, t1, t2, t3;

				float f0, f1, f2, f3, f4, f5, f6,
					f7, f8, f9, f10, f11, f12, f13, f14, f15, f16, f17, f18;
				int g = 0;
				#define f(i) f##i
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

				#define c0 (1.F/19)
				#define c1 (5.F/399)
				#define c2 (11.F/2394)
				#define c3 (4.F/1197)
				#define c4 (1.F/21)
				#define c5 (1.F/63)
				#define c6 (1.F/252)
				#define c7 (1.F/10)
				#define c8 (1.F/40)
				#define c9 (1.F/18)
				#define c10 (1.F/36)
				#define c11 (1.F/72)
				#define c12 (1.F/12)
				#define c13 (1.F/24)
				#define c14 (1.F/4)
				#define c15 (1.F/8)

				// [1] Computation of moments
				t0 = f(1) + f(2) + f(3) + f(4) + f(5) + f(6);
				t1 = f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14)
				  + f(15) + f(16) + f(17) + f(18);
				rho = f(0) + t0 + t1;
				e = -30*f(0) - 11*t0 + 8*t1; 
				eps = 12*f(0) - 4*t0 + t1;
				t0 = f(1) - f(2);
				t1 = f(7) - f(8) + f(9) - f(10) + f(11) - f(12) + f(13) - f(14);
				jx = t0 + t1;
				qx = -4*t0 + t1;
				t0 = f(3) - f(4);
				t1 = f(7) + f(8) - f(9) - f(10) + f(15) - f(16) + f(17) - f(18);
				jy = t0 + t1;
				qy = -4*t0 + t1;
				t0 = f(5) - f(6);
				t1 = f(11) + f(12) - f(13) - f(14) + f(15) + f(16) - f(17) - f(18);
				jz = t0 + t1;
				qz = -4*t0 + t1;

				// [2] Default case, compute new state
				t0 = 2*(f(1) + f(2)) - (f(3) + f(4) + f(5) + f(6));
				t1 = (f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14))
				 - 2*(f(15) + f(16) + f(17) + f(18));
				px = t0 + t1;
				pix = -2*t0 + t1;
				t0 = (f(3) + f(4)) - (f(5) + f(6));
				t1 = (f(7) + f(8) + f(9) + f(10)) - (f(11) + f(12) + f(13) + f(14));
				pw = t0 + t1;
				piw = -2*t0 + t1;
				pxy = f(7) - f(8) - f(9) + f(10);
				pyz = f(15) - f(16) - f(17) + f(18);
				pxz = f(11) - f(12) - f(13) + f(14);
				mx = f(7) - f(8) + f(9) - f(10) - f(11) + f(12) - f(13) + f(14);
				my = -f(7) - f(8) + f(9) + f(10) + f(15) - f(16) + f(17) - f(18);
				mz = f(11) + f(12) - f(13) - f(14) - f(15) - f(16) + f(17) + f(18);

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
				f(0) = c0*rho - c1*e + c4*eps;
				t0 = c0*rho - c2*e - c5*eps;
				t1 = px - pix;
				t2 = pw - piw;
				f(1) = t0 + c9*t1 + c7*(jx - qx);
				f(2) = t0 + c9*t1 - c7*(jx - qx);
				f(3) = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
				f(4) = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
				f(5) = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
				f(6) = t0 - c10*t1 - c12*t2 - c7*(jz - qz);
				t0 = c0*rho + c3*e + c6*eps;
				t1 = c7*jx + c8*qx;
				t2 = c7*jy + c8*qy;
				t3 = c7*jz + c8*qz;
				f(7) = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
					+ c15*(mx - my);
				f(8) = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
					- c15*(mx + my);
				f(9) = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
					+ c15*(mx + my);
				f(10) = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
					- c15*(mx - my);
				f(11) = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
					- c15*(mx - mz);
				f(12) = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
					+ c15*(mx + mz);
				f(13) = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
					- c15*(mx + mz);
				f(14) = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
					+ c15*(mx - mz);
				f(15) = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
				f(16) = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
				f(17) = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
				f(18) = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);
				#undef c0
				#undef c1
				#undef c2
				#undef c3
				#undef c4
				#undef c5
				#undef c6
				#undef c7
				#undef c8
				#undef c9
				#undef c10
				#undef c11
				#undef c12
				#undef c13
				#undef c14
				#undef c15

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
	// #pragma endscop
}

#else // compute with boundary condition on Host side

void compute(
	config* Config, 
	// float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	// float Dst[Config->Lz][Config->Ly][Config->Lx][Q]
	float Src[128][128][128][Q], 
	float Dst[128][128][128][Q], 
	float f[128][128][128][Q])
{
	// Fetch
	// FIXME : ppcg will generate too big allocation of dev_Src
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
	// #pragma scop
	for(unsigned int z = 0; z < 128; z++){
		for(unsigned int y = 0; y < 128; y++){
			for(unsigned int x = 0; x < 128; x++){

				//////////// LOCAL VARIABLES //////////////////////////////////
				// Moments
				float rho, e, eps, jx, qx, jy, qy, jz, qz,
				    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

				// Auxiliary variables
				float t0, t1, t2, t3;

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

				#if 0
				next_step(&f(0), &f(1), &f(2), &f(3), &f(4), &f(5), &f(6), &f(7), 
					&f(8), &f(9), &f(10), &f(11), &f(12), &f(13), &f(14), &f(15), 
					&f(16), &f(17), &f(18));
				#else
				#define c0 (1.F/19)
				#define c1 (5.F/399)
				#define c2 (11.F/2394)
				#define c3 (4.F/1197)
				#define c4 (1.F/21)
				#define c5 (1.F/63)
				#define c6 (1.F/252)
				#define c7 (1.F/10)
				#define c8 (1.F/40)
				#define c9 (1.F/18)
				#define c10 (1.F/36)
				#define c11 (1.F/72)
				#define c12 (1.F/12)
				#define c13 (1.F/24)
				#define c14 (1.F/4)
				#define c15 (1.F/8)

				// [1] Computation of moments
				t0 = f(1) + f(2) + f(3) + f(4) + f(5) + f(6);
				t1 = f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14)
				  + f(15) + f(16) + f(17) + f(18);
				rho = f(0) + t0 + t1;
				e = -30*f(0) - 11*t0 + 8*t1; 
				eps = 12*f(0) - 4*t0 + t1;
				t0 = f(1) - f(2);
				t1 = f(7) - f(8) + f(9) - f(10) + f(11) - f(12) + f(13) - f(14);
				jx = t0 + t1;
				qx = -4*t0 + t1;
				t0 = f(3) - f(4);
				t1 = f(7) + f(8) - f(9) - f(10) + f(15) - f(16) + f(17) - f(18);
				jy = t0 + t1;
				qy = -4*t0 + t1;
				t0 = f(5) - f(6);
				t1 = f(11) + f(12) - f(13) - f(14) + f(15) + f(16) - f(17) - f(18);
				jz = t0 + t1;
				qz = -4*t0 + t1;

				// [2] Default case, compute new state
				t0 = 2*(f(1) + f(2)) - (f(3) + f(4) + f(5) + f(6));
				t1 = (f(7) + f(8) + f(9) + f(10) + f(11) + f(12) + f(13) + f(14))
				 - 2*(f(15) + f(16) + f(17) + f(18));
				px = t0 + t1;
				pix = -2*t0 + t1;
				t0 = (f(3) + f(4)) - (f(5) + f(6));
				t1 = (f(7) + f(8) + f(9) + f(10)) - (f(11) + f(12) + f(13) + f(14));
				pw = t0 + t1;
				piw = -2*t0 + t1;
				pxy = f(7) - f(8) - f(9) + f(10);
				pyz = f(15) - f(16) - f(17) + f(18);
				pxz = f(11) - f(12) - f(13) + f(14);
				mx = f(7) - f(8) + f(9) - f(10) - f(11) + f(12) - f(13) + f(14);
				my = -f(7) - f(8) + f(9) + f(10) + f(15) - f(16) + f(17) - f(18);
				mz = f(11) + f(12) - f(13) - f(14) - f(15) - f(16) + f(17) + f(18);

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
				f(0) = c0*rho - c1*e + c4*eps;
				t0 = c0*rho - c2*e - c5*eps;
				t1 = px - pix;
				t2 = pw - piw;
				f(1) = t0 + c9*t1 + c7*(jx - qx);
				f(2) = t0 + c9*t1 - c7*(jx - qx);
				f(3) = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
				f(4) = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
				f(5) = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
				f(6) = t0 - c10*t1 - c12*t2 - c7*(jz - qz);
				t0 = c0*rho + c3*e + c6*eps;
				t1 = c7*jx + c8*qx;
				t2 = c7*jy + c8*qy;
				t3 = c7*jz + c8*qz;
				f(7) = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
					+ c15*(mx - my);
				f(8) = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
					- c15*(mx + my);
				f(9) = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
					+ c15*(mx + my);
				f(10) = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
					- c15*(mx - my);
				f(11) = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
					- c15*(mx - mz);
				f(12) = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
					+ c15*(mx + mz);
				f(13) = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
					- c15*(mx + mz);
				f(14) = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
					+ c15*(mx - mz);
				f(15) = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
				f(16) = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
				f(17) = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
				f(18) = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);
				#undef c0
				#undef c1
				#undef c2
				#undef c3
				#undef c4
				#undef c5
				#undef c6
				#undef c7
				#undef c8
				#undef c9
				#undef c10
				#undef c11
				#undef c12
				#undef c13
				#undef c14
				#undef c15
				#endif

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
	// #pragma endscop
}
#endif // COMPUTE_BOUNDARY_ON_HOST

/*
 *  monitor: monitor kernel function
 */
void monitor(
	config* Config, results* Results, 
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
	)
{
	// Moments
	float rho, jx, jy, jz;

	// Auxiliary variables
	float t0, t1;

	// Local density distribution
	float f[19] = {0};

	// Compute all nodes for the current step
	// #pragma scop
	for(unsigned int z = 0; z < Config->Lz; z++){
		for(unsigned int y = 0; y < Config->Ly; y++){
			for(unsigned int x = 0; x < Config->Lx; x++){

				// Get local geometry
				// int g = getGeometry(Config, x, y, z);
				int g = 0;
				if (x == Config->Lx-1) g = g | G_W1;
				if (x == 0)    g = g | G_W2;
				if (y == Config->Ly-1) g = g | G_W3;
				if (y == 0)    g = g | G_W4;
				if (z == Config->Lz-1) g = g | G_W5;
				if (z == 0)    g = g | G_W6;

				// Fetch source density distribution
				f[ 0] = Src[z][y][x][0]; // src(0, 0);
				if (g & G_BND) {
					// Simple bounce-back boundary condition
					if (g & G_E02) f[ 1] = Src[z][y][x][ 2]; /*src( 2, 0);*/ else f[ 1] = Src[z  ][y  ][x-1][ 1]; /*src( 1, -e01);*/
					if (g & G_E01) f[ 2] = Src[z][y][x][ 1]; /*src( 1, 0);*/ else f[ 2] = Src[z  ][y  ][x+1][ 2]; /*src( 2, -e02);*/
					if (g & G_E04) f[ 3] = Src[z][y][x][ 4]; /*src( 4, 0);*/ else f[ 3] = Src[z  ][y-1][x  ][ 3]; /*src( 3, -e03);*/
					if (g & G_E03) f[ 4] = Src[z][y][x][ 3]; /*src( 3, 0);*/ else f[ 4] = Src[z  ][y+1][x  ][ 4]; /*src( 4, -e04);*/
					if (g & G_E06) f[ 5] = Src[z][y][x][ 6]; /*src( 6, 0);*/ else f[ 5] = Src[z-1][y  ][x  ][ 5]; /*src( 5, -e05);*/
					if (g & G_E05) f[ 6] = Src[z][y][x][ 5]; /*src( 5, 0);*/ else f[ 6] = Src[z+1][y  ][x  ][ 6]; /*src( 6, -e06);*/
					if (g & G_E10) f[ 7] = Src[z][y][x][10]; /*src(10, 0);*/ else f[ 7] = Src[z  ][y-1][x-1][ 7]; /*src( 7, -e07);*/
					if (g & G_E09) f[ 8] = Src[z][y][x][ 9]; /*src( 9, 0);*/ else f[ 8] = Src[z  ][y-1][x+1][ 8]; /*src( 8, -e08);*/
					if (g & G_E08) f[ 9] = Src[z][y][x][ 8]; /*src( 8, 0);*/ else f[ 9] = Src[z  ][y+1][x-1][ 9]; /*src( 9, -e09);*/
					if (g & G_E07) f[10] = Src[z][y][x][ 7]; /*src( 7, 0);*/ else f[10] = Src[z  ][y+1][x+1][10]; /*src(10, -e10);*/
					if (g & G_E14) f[11] = Src[z][y][x][14]; /*src(14, 0);*/ else f[11] = Src[z-1][y  ][x-1][11]; /*src(11, -e11);*/
					if (g & G_E13) f[12] = Src[z][y][x][13]; /*src(13, 0);*/ else f[12] = Src[z-1][y  ][x+1][12]; /*src(12, -e12);*/
					if (g & G_E12) f[13] = Src[z][y][x][12]; /*src(12, 0);*/ else f[13] = Src[z+1][y  ][x-1][13]; /*src(13, -e13);*/
					if (g & G_E11) f[14] = Src[z][y][x][11]; /*src(11, 0);*/ else f[14] = Src[z+1][y  ][x+1][14]; /*src(14, -e14);*/
					if (g & G_E18) f[15] = Src[z][y][x][18]; /*src(18, 0);*/ else f[15] = Src[z-1][y-1][x  ][15]; /*src(15, -e15);*/
					if (g & G_E17) f[16] = Src[z][y][x][17]; /*src(17, 0);*/ else f[16] = Src[z-1][y+1][x  ][16]; /*src(16, -e16);*/
					if (g & G_E16) f[17] = Src[z][y][x][16]; /*src(16, 0);*/ else f[17] = Src[z+1][y-1][x  ][17]; /*src(17, -e17);*/
					if (g & G_E15) f[18] = Src[z][y][x][15]; /*src(15, 0);*/ else f[18] = Src[z+1][y+1][x  ][18]; /*src(18, -e18);*/

					// Imposed lid velocity
					if (z == Config->Lx-1) {
						f[13] += k0;
						f[14] -= k0;
					}
				} else {
					// Pull propagation
					f[ 1] = Src[z  ][y  ][x-1][ 1]; /*src( 1, -e01);*/
					f[ 2] = Src[z  ][y  ][x+1][ 2]; /*src( 2, -e02);*/
					f[ 3] = Src[z  ][y-1][x  ][ 3]; /*src( 3, -e03);*/
					f[ 4] = Src[z  ][y+1][x  ][ 4]; /*src( 4, -e04);*/
					f[ 5] = Src[z-1][y  ][x  ][ 5]; /*src( 5, -e05);*/
					f[ 6] = Src[z+1][y  ][x  ][ 6]; /*src( 6, -e06);*/
					f[ 7] = Src[z  ][y-1][x-1][ 7]; /*src( 7, -e07);*/
					f[ 8] = Src[z  ][y-1][x+1][ 8]; /*src( 8, -e08);*/
					f[ 9] = Src[z  ][y+1][x-1][ 9]; /*src( 9, -e09);*/
					f[10] = Src[z  ][y+1][x+1][10]; /*src(10, -e10);*/
					f[11] = Src[z-1][y  ][x-1][11]; /*src(11, -e11);*/
					f[12] = Src[z-1][y  ][x+1][12]; /*src(12, -e12);*/
					f[13] = Src[z+1][y  ][x-1][13]; /*src(13, -e13);*/
					f[14] = Src[z+1][y  ][x+1][14]; /*src(14, -e14);*/
					f[15] = Src[z-1][y-1][x  ][15]; /*src(15, -e15);*/
					f[16] = Src[z-1][y+1][x  ][16]; /*src(16, -e16);*/
					f[17] = Src[z+1][y-1][x  ][17]; /*src(17, -e17);*/
					f[18] = Src[z+1][y+1][x  ][18]; /*src(18, -e18);*/
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

				// Velocity components
				float ux, uy, uz;

				// Fetch stored velocity
				ux = Velocity[z][y][x][0];
				uy = Velocity[z][y][x][1];
				uz = Velocity[z][y][x][2];

				// Store current velocity
				float vx = jx/(1 + rho);
				float vy = jy/(1 + rho);
				float vz = jz/(1 + rho);
				Velocity[z][y][x][0] = vx;
				Velocity[z][y][x][1] = vy;
				Velocity[z][y][x][2] = vz;

				// Compute and store squared velocity variation
				Delta[z][y][x] = (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);
				
				// Write bitmap pixel
				if (Results->Bitmap && y == (Config->Ly>>1)){
					writePixel(Results, Config, x, z, 4*sqrt(vx*vx + vy*vy + vz*vz)/Config->U0);
				}
			} // end for x
		} // end for y
	} // end for z
	// #pragma endscop

	// Monitoring, print progress and exports image
	Results->L2 = sqrt(sumArray((float*)Delta, 
		Results->DomainSize)/Results->DomainSize);
   if (Results->L2 < Results->epsilon) Results->Converged = true;
}

/*
 *  extraction: extraction kernel function
 */
void extraction(
	config* Config, 
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Density[Config->Lz][Config->Ly][Config->Lx],
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D]
	)
{
	// Moments
	float rho, jx, jy, jz;

	// Auxiliary variables
	float t0, t1;

	// Local density distribution
	float f[19] = {0};

	// Compute all nodes for the current step
	// #pragma scop
	for(unsigned int z = 0; z < Config->Lz; z++){
		for(unsigned int y = 0; y < Config->Ly; y++){
			for(unsigned int x = 0; x < Config->Lx; x++){

				// Get local geometry
				// int g = getGeometry(Config, x, y, z);
				int g = 0;
				if (x == Config->Lx-1) g = g | G_W1;
				if (x == 0)    g = g | G_W2;
				if (y == Config->Ly-1) g = g | G_W3;
				if (y == 0)    g = g | G_W4;
				if (z == Config->Lz-1) g = g | G_W5;
				if (z == 0)    g = g | G_W6;

				// Fetch source density distribution
				f[ 0] = Src[z][y][x][0]; // src(0, 0);
				if (g & G_BND) {
					// Simple bounce-back boundary condition
					if (g & G_E02) f[ 1] = Src[z][y][x][ 2]; /*src( 2, 0);*/ else f[ 1] = Src[z  ][y  ][x-1][ 1]; /*src( 1, -e01);*/
					if (g & G_E01) f[ 2] = Src[z][y][x][ 1]; /*src( 1, 0);*/ else f[ 2] = Src[z  ][y  ][x+1][ 2]; /*src( 2, -e02);*/
					if (g & G_E04) f[ 3] = Src[z][y][x][ 4]; /*src( 4, 0);*/ else f[ 3] = Src[z  ][y-1][x  ][ 3]; /*src( 3, -e03);*/
					if (g & G_E03) f[ 4] = Src[z][y][x][ 3]; /*src( 3, 0);*/ else f[ 4] = Src[z  ][y+1][x  ][ 4]; /*src( 4, -e04);*/
					if (g & G_E06) f[ 5] = Src[z][y][x][ 6]; /*src( 6, 0);*/ else f[ 5] = Src[z-1][y  ][x  ][ 5]; /*src( 5, -e05);*/
					if (g & G_E05) f[ 6] = Src[z][y][x][ 5]; /*src( 5, 0);*/ else f[ 6] = Src[z+1][y  ][x  ][ 6]; /*src( 6, -e06);*/
					if (g & G_E10) f[ 7] = Src[z][y][x][10]; /*src(10, 0);*/ else f[ 7] = Src[z  ][y-1][x-1][ 7]; /*src( 7, -e07);*/
					if (g & G_E09) f[ 8] = Src[z][y][x][ 9]; /*src( 9, 0);*/ else f[ 8] = Src[z  ][y-1][x+1][ 8]; /*src( 8, -e08);*/
					if (g & G_E08) f[ 9] = Src[z][y][x][ 8]; /*src( 8, 0);*/ else f[ 9] = Src[z  ][y+1][x-1][ 9]; /*src( 9, -e09);*/
					if (g & G_E07) f[10] = Src[z][y][x][ 7]; /*src( 7, 0);*/ else f[10] = Src[z  ][y+1][x+1][10]; /*src(10, -e10);*/
					if (g & G_E14) f[11] = Src[z][y][x][14]; /*src(14, 0);*/ else f[11] = Src[z-1][y  ][x-1][11]; /*src(11, -e11);*/
					if (g & G_E13) f[12] = Src[z][y][x][13]; /*src(13, 0);*/ else f[12] = Src[z-1][y  ][x+1][12]; /*src(12, -e12);*/
					if (g & G_E12) f[13] = Src[z][y][x][12]; /*src(12, 0);*/ else f[13] = Src[z+1][y  ][x-1][13]; /*src(13, -e13);*/
					if (g & G_E11) f[14] = Src[z][y][x][11]; /*src(11, 0);*/ else f[14] = Src[z+1][y  ][x+1][14]; /*src(14, -e14);*/
					if (g & G_E18) f[15] = Src[z][y][x][18]; /*src(18, 0);*/ else f[15] = Src[z-1][y-1][x  ][15]; /*src(15, -e15);*/
					if (g & G_E17) f[16] = Src[z][y][x][17]; /*src(17, 0);*/ else f[16] = Src[z-1][y+1][x  ][16]; /*src(16, -e16);*/
					if (g & G_E16) f[17] = Src[z][y][x][16]; /*src(16, 0);*/ else f[17] = Src[z+1][y-1][x  ][17]; /*src(17, -e17);*/
					if (g & G_E15) f[18] = Src[z][y][x][15]; /*src(15, 0);*/ else f[18] = Src[z+1][y+1][x  ][18]; /*src(18, -e18);*/

					// Imposed lid velocity
					if (z == Config->Lx-1) {
						f[13] += k0;
						f[14] -= k0;
					}
				} else {
					// Pull propagation
					f[ 1] = Src[z  ][y  ][x-1][ 1]; /*src( 1, -e01);*/
					f[ 2] = Src[z  ][y  ][x+1][ 2]; /*src( 2, -e02);*/
					f[ 3] = Src[z  ][y-1][x  ][ 3]; /*src( 3, -e03);*/
					f[ 4] = Src[z  ][y+1][x  ][ 4]; /*src( 4, -e04);*/
					f[ 5] = Src[z-1][y  ][x  ][ 5]; /*src( 5, -e05);*/
					f[ 6] = Src[z+1][y  ][x  ][ 6]; /*src( 6, -e06);*/
					f[ 7] = Src[z  ][y-1][x-1][ 7]; /*src( 7, -e07);*/
					f[ 8] = Src[z  ][y-1][x+1][ 8]; /*src( 8, -e08);*/
					f[ 9] = Src[z  ][y+1][x-1][ 9]; /*src( 9, -e09);*/
					f[10] = Src[z  ][y+1][x+1][10]; /*src(10, -e10);*/
					f[11] = Src[z-1][y  ][x-1][11]; /*src(11, -e11);*/
					f[12] = Src[z-1][y  ][x+1][12]; /*src(12, -e12);*/
					f[13] = Src[z+1][y  ][x-1][13]; /*src(13, -e13);*/
					f[14] = Src[z+1][y  ][x+1][14]; /*src(14, -e14);*/
					f[15] = Src[z-1][y-1][x  ][15]; /*src(15, -e15);*/
					f[16] = Src[z-1][y+1][x  ][16]; /*src(16, -e16);*/
					f[17] = Src[z+1][y-1][x  ][17]; /*src(17, -e17);*/
					f[18] = Src[z+1][y+1][x  ][18]; /*src(18, -e18);*/
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

				// Store current density and velocity
				Density[z][y][x] = 1 + rho;
				Velocity[z][y][x][0] = jx/(1 + rho);
				Velocity[z][y][x][1] = jy/(1 + rho);
				Velocity[z][y][x][2] = jz/(1 + rho);
			} // end for x
		} // end for y
	} // end for z
	// #pragma endscop
}
