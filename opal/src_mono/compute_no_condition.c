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

#undef __BORDER_CHECK__ // Do we want the border check ?
/*
 *  getGeometry: encode local geometry informations in a bitfield.
 *  NOTE: In case of more complex geometries, the bitfields might be created
 *  during initialisation and stored in device memory.
 */

static inline int getGeometry(config* Config, int x, int y, int z)
{
    int g = 0;

    if (x == Config->Lx-1) g = g | G_W1;
    if (x == 0)    g = g | G_W2;
    if (y == Config->Ly-1) g = g | G_W3;
    if (y == 0)    g = g | G_W4;
    if (z == Config->Lz-1) g = g | G_W5;
    if (z == 0)    g = g | G_W6;

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

/*
 *  compute: compute kernel function
 */
void compute(
	config* Config, results* Results, unsigned int step,
	float Src[Config->Lz][Config->Ly][Config->Lx][Q], 
	float Dst[Config->Lz][Config->Ly][Config->Lx][Q],
	float Density[Config->Lz][Config->Ly][Config->Lx],
	float Velocity[Config->Lz][Config->Ly][Config->Lx][D],
	float Delta[Config->Lz][Config->Ly][Config->Lx]
	)
{
	// Moments
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
	    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

	// Auxiliary variables
	float t0, t1, t2, t3;

	// Local density distribution
	float f[19] = {0};

	// Compute all nodes for the current step
	#pragma scop
	for(unsigned int z = 0; z < Config->Lz; z++){
		for(unsigned int y = 0; y < Config->Ly; y++){
			for(unsigned int x = 0; x < Config->Lx; x++){

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

					Dst[z][y][x][ 0] = f[ 0]; /*dst( 0) = f[ 0];*/
					Dst[z][y][x][ 1] = f[ 1]; /*dst( 1) = f[ 1];*/
					Dst[z][y][x][ 2] = f[ 2]; /*dst( 2) = f[ 2];*/
					Dst[z][y][x][ 3] = f[ 3]; /*dst( 3) = f[ 3];*/
					Dst[z][y][x][ 4] = f[ 4]; /*dst( 4) = f[ 4];*/
					Dst[z][y][x][ 5] = f[ 5]; /*dst( 5) = f[ 5];*/
					Dst[z][y][x][ 6] = f[ 6]; /*dst( 6) = f[ 6];*/
					Dst[z][y][x][ 7] = f[ 7]; /*dst( 7) = f[ 7];*/
					Dst[z][y][x][ 8] = f[ 8]; /*dst( 8) = f[ 8];*/
					Dst[z][y][x][ 9] = f[ 9]; /*dst( 9) = f[ 9];*/
					Dst[z][y][x][10] = f[10]; /*dst(10) = f[10];*/
					Dst[z][y][x][11] = f[11]; /*dst(11) = f[11];*/
					Dst[z][y][x][12] = f[12]; /*dst(12) = f[12];*/
					Dst[z][y][x][13] = f[13]; /*dst(13) = f[13];*/
					Dst[z][y][x][14] = f[14]; /*dst(14) = f[14];*/
					Dst[z][y][x][15] = f[15]; /*dst(15) = f[15];*/
					Dst[z][y][x][16] = f[16]; /*dst(16) = f[16];*/
					Dst[z][y][x][17] = f[17]; /*dst(17) = f[17];*/
					Dst[z][y][x][18] = f[18]; /*dst(18) = f[18];*/
			// } 

				// Monitoring: Do every 'Period' steps
			} // end for x
		} // end for y
	} // end for z
	#pragma endscop

	// If monitoring, print progress and exports image
	if(step > 0 && (step % Config->Period == 0))
	{
		Results->L2 = sqrt(sumArray((float*)Delta, 
			Results->DomainSize)/Results->DomainSize);
	   if (Results->L2 < Results->epsilon) Results->Converged = true;

		printResults(Results, step);
		if(Config->Images) exportImage(Config, Results, step);
	}
}
