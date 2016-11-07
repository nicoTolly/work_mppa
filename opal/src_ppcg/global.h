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
 *  global.h: OpenCL and literal constants, macro definitions, and auxiliary
 *  functions
 *
 */

// Velocity parameter
#define k0 (0.4F/6)

// Relaxation coefficients
#define s1 (1.19F)
#define s2 (1.40F)
#define s4 (1.20F)
#define s9 (2*1E3/(6*0.4F*128 + 1E3))
#define s10 (1.40F)
#define s16 (1.98F)

// Collision coefficients
// #define c0 (1.F/19)
// #define c1 (5.F/399)
// #define c2 (11.F/2394)
// #define c3 (4.F/1197)
// #define c4 (1.F/21)
// #define c5 (1.F/63)
// #define c6 (1.F/252)
// #define c7 (1.F/10)
// #define c8 (1.F/40)
// #define c9 (1.F/18)
// #define c10 (1.F/36)
// #define c11 (1.F/72)
// #define c12 (1.F/12)
// #define c13 (1.F/24)
// #define c14 (1.F/4)
// #define c15 (1.F/8)

// Solid boundaries encoding
#define G_E01   0x000001
#define G_E02   0x000002
#define G_E03   0x000004
#define G_E04   0x000008
#define G_E05   0x000010
#define G_E06   0x000020
#define G_E07   0x000040
#define G_E08   0x000080
#define G_E09   0x000100
#define G_E10   0x000200
#define G_E11   0x000400
#define G_E12   0x000800
#define G_E13   0x001000
#define G_E14   0x002000
#define G_E15   0x004000
#define G_E16   0x008000
#define G_E17   0x010000
#define G_E18   0x020000

// Walls
#define G_W1    0x001541
#define G_W2    0x002A82
#define G_W3    0x0140C4
#define G_W4    0x028308
#define G_W5    0x00CC10
#define G_W6    0x033020

// Boundary nodes
#define G_BND   0x03FFFF

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
}
