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
 *  compute.cl: main computation kernel.
 *
 */
#ifdef USE_CLSTENCIL

kernel void compute(global float* Source, global float* Destination,
	global float* Density, global float* Velocity,
	global float* Delta, global uchar* Bitmap)
{
	// Retrieve coordinates
	int x = getCoordinate('x');
	int y = getCoordinate('y');
	int z = getCoordinate('z');

	// Get local geometry
	int g = getGeometry(x, y, z);

	// Moments
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
	    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;

	// Auxiliary variables
	float t0, t1, t2, t3;

	// Local density distribution
	float f[19] = {0};
	unsigned int directions[19] = 
	   {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};

		// Register regions :
	clst_region_t region_src = {0};
	clst_region_t region_dst = {0};

	region_src = clst_register_memory_region((__global void*)Source, CLST_GENTYPE_FLOAT, 
	                  Lx, Ly, Lz, CLST_MODEL_D3Q19, 19);
	region_dst = clst_register_memory_region((__global void*)Destination, CLST_GENTYPE_FLOAT, 
	                  Lx, Ly, Lz, CLST_MODEL_D3Q19, 19);

	// Fetch source density distribution
	f[ 0] = clst_float_read_node(&region_src, directions[ 0], x, y, z); // f00 = src(0, 0);
	if (g & G_BND) {
		#if 1
		// Simple bounce-back boundary condition
		if (g & G_E02) {f[ 1] = clst_float_read_node(&region_src, directions[ 2], x,y,z);     // f01 = src( 2, 0); 
		} else {        f[ 1] = clst_float_read_node(&region_src, directions[ 1], e01  ); }   // f01 = src( 1, -e01);
		if (g & G_E01) {f[ 2] = clst_float_read_node(&region_src, directions[ 1], x,y,z);     // f02 = src( 1, 0); 
		} else {        f[ 2] = clst_float_read_node(&region_src, directions[ 2], e02  ); }   // f02 = src( 2, -e02);
		if (g & G_E04) {f[ 3] = clst_float_read_node(&region_src, directions[ 4], x,y,z);     // f03 = src( 4, 0); 
		} else {        f[ 3] = clst_float_read_node(&region_src, directions[ 3], e03  ); }   // f03 = src( 3, -e03);
		if (g & G_E03) {f[ 4] = clst_float_read_node(&region_src, directions[ 3], x,y,z);     // f04 = src( 3, 0); 
		} else {        f[ 4] = clst_float_read_node(&region_src, directions[ 4], e04  ); }   // f04 = src( 4, -e04);
		if (g & G_E06) {f[ 5] = clst_float_read_node(&region_src, directions[ 6], x,y,z);     // f05 = src( 6, 0); 
		} else {        f[ 5] = clst_float_read_node(&region_src, directions[ 5], e05  ); }   // f05 = src( 5, -e05);
		if (g & G_E05) {f[ 6] = clst_float_read_node(&region_src, directions[ 5], x,y,z);     // f06 = src( 5, 0); 
		} else {        f[ 6] = clst_float_read_node(&region_src, directions[ 6], e06  ); }   // f06 = src( 6, -e06);
		if (g & G_E10) {f[ 7] = clst_float_read_node(&region_src, directions[10], x,y,z);     // f07 = src(10, 0); 
		} else {        f[ 7] = clst_float_read_node(&region_src, directions[ 7], e07  ); }   // f07 = src( 7, -e07);
		if (g & G_E09) {f[ 8] = clst_float_read_node(&region_src, directions[ 9], x,y,z);     // f08 = src( 9, 0); 
		} else {        f[ 8] = clst_float_read_node(&region_src, directions[ 8], e08  ); }   // f08 = src( 8, -e08);
		if (g & G_E08) {f[ 9] = clst_float_read_node(&region_src, directions[ 8], x,y,z);     // f09 = src( 8, 0); 
		} else {        f[ 9] = clst_float_read_node(&region_src, directions[ 9], e09  ); }   // f09 = src( 9, -e09);
		if (g & G_E07) {f[10] = clst_float_read_node(&region_src, directions[ 7], x,y,z);     // f10 = src( 7, 0); 
		} else {        f[10] = clst_float_read_node(&region_src, directions[10], e10  ); }   // f10 = src(10, -e10);
		if (g & G_E14) {f[11] = clst_float_read_node(&region_src, directions[14], x,y,z);     // f11 = src(14, 0); 
		} else {        f[11] = clst_float_read_node(&region_src, directions[11], e11  ); }   // f11 = src(11, -e11);
		if (g & G_E13) {f[12] = clst_float_read_node(&region_src, directions[13], x,y,z);     // f12 = src(13, 0); 
		} else {        f[12] = clst_float_read_node(&region_src, directions[12], e12  ); }   // f12 = src(12, -e12);
		if (g & G_E12) {f[13] = clst_float_read_node(&region_src, directions[12], x,y,z);     // f13 = src(12, 0); 
		} else {        f[13] = clst_float_read_node(&region_src, directions[13], e13  ); }   // f13 = src(13, -e13);
		if (g & G_E11) {f[14] = clst_float_read_node(&region_src, directions[11], x,y,z);     // f14 = src(11, 0); 
		} else {        f[14] = clst_float_read_node(&region_src, directions[14], e14  ); }   // f14 = src(14, -e14);
		if (g & G_E18) {f[15] = clst_float_read_node(&region_src, directions[18], x,y,z);     // f15 = src(18, 0); 
		} else {        f[15] = clst_float_read_node(&region_src, directions[15], e15  ); }   // f15 = src(15, -e15);
		if (g & G_E17) {f[16] = clst_float_read_node(&region_src, directions[17], x,y,z);     // f16 = src(17, 0); 
		} else {        f[16] = clst_float_read_node(&region_src, directions[16], e16  ); }   // f16 = src(16, -e16);
		if (g & G_E16) {f[17] = clst_float_read_node(&region_src, directions[16], x,y,z);     // f17 = src(16, 0); 
		} else {        f[17] = clst_float_read_node(&region_src, directions[17], e17  ); }   // f17 = src(17, -e17);
		if (g & G_E15) {f[18] = clst_float_read_node(&region_src, directions[15], x,y,z);     // f18 = src(15, 0); 
		} else {        f[18] = clst_float_read_node(&region_src, directions[18], e18  ); }   // f18 = src(18, -e18);

		// Imposed lid velocity
		if (z == Lx-1) {
		    f[13] += k0;
		    f[14] -= k0;
		}
		#endif
	} else {
		#if 1
		// Pull propagation
		f[ 1] = clst_float_read_node(&region_src, directions[ 1], e01); // f01 = src( 1, -e01);
		f[ 2] = clst_float_read_node(&region_src, directions[ 2], e02); // f02 = src( 2, -e02);
		f[ 3] = clst_float_read_node(&region_src, directions[ 3], e03); // f03 = src( 3, -e03);
		f[ 4] = clst_float_read_node(&region_src, directions[ 4], e04); // f04 = src( 4, -e04);
		f[ 5] = clst_float_read_node(&region_src, directions[ 5], e05); // f05 = src( 5, -e05);
		f[ 6] = clst_float_read_node(&region_src, directions[ 6], e06); // f06 = src( 6, -e06);
		f[ 7] = clst_float_read_node(&region_src, directions[ 7], e07); // f07 = src( 7, -e07);
		f[ 8] = clst_float_read_node(&region_src, directions[ 8], e08); // f08 = src( 8, -e08);
		f[ 9] = clst_float_read_node(&region_src, directions[ 9], e09); // f09 = src( 9, -e09);
		f[10] = clst_float_read_node(&region_src, directions[10], e10); // f10 = src(10, -e10);
		f[11] = clst_float_read_node(&region_src, directions[11], e11); // f11 = src(11, -e11);
		f[12] = clst_float_read_node(&region_src, directions[12], e12); // f12 = src(12, -e12);
		f[13] = clst_float_read_node(&region_src, directions[13], e13); // f13 = src(13, -e13);
		f[14] = clst_float_read_node(&region_src, directions[14], e14); // f14 = src(14, -e14);
		f[15] = clst_float_read_node(&region_src, directions[15], e15); // f15 = src(15, -e15);
		f[16] = clst_float_read_node(&region_src, directions[16], e16); // f16 = src(16, -e16);
		f[17] = clst_float_read_node(&region_src, directions[17], e17); // f17 = src(17, -e17);
		f[18] = clst_float_read_node(&region_src, directions[18], e18); // f18 = src(18, -e18);
		#endif
	}

	#if 1
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
	#endif

	if (Destination) {
		#if 1
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

		clst_float_write_node_multiple(&region_dst, 19, directions, f, x, y, z);
		#endif		

	} else if (Delta) {
		#if 1
		// Alternate case: monitoring

		// Velocity components
		float ux, uy, uz;

		// Set current node
		global float* Vel = Velocity + 3*offset(x, y, z);
		global float* Del = Delta + offset(x, y, z);

		// Fetch stored velocity
		ux = Vel[0];
		uy = Vel[1];
		uz = Vel[2];

		// Store current velocity
		float vx = jx/(1 + rho);
		float vy = jy/(1 + rho);
		float vz = jz/(1 + rho);
		Vel[0] = vx;
		Vel[1] = vy;
		Vel[2] = vz;

		// Compute and store squared velocity variation
		*Del = (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);

		// Write bitmap pixel
		if (Bitmap && y == (Ly>>1)){
			#if !defined(MPPA) || !defined(LLVM)	
			// sqrt is not defined yet on MPPA LLVM
			writePixel(Bitmap, x, z, 4*sqrt(vx*vx + vy*vy + vz*vz)/U0);
			#endif
		}
		#endif
	} else if (Density) {
		#if 1
		// Alternate case: extraction

		// Set current node
		global float* Den = Density + offset(x, y, z);
		global float* Vel = Velocity + 3*offset(x, y, z);

		// Store current density and velocity
		*Den = 1 + rho;
		Vel[0] = jx/(1 + rho);
		Vel[1] = jy/(1 + rho);
		Vel[2] = jz/(1 + rho);
		#endif		
	}

	// End, clean up
	clst_disregister_memory_region(&region_src);
	clst_disregister_memory_region(&region_dst);
}

#else 

#if 0
kernel void compute(global float* Source, global float* Destination,
	global float* Density, global float* Velocity,
	global float* Delta, global uchar* Bitmap)
{

	// Retrieve coordinates
	int x = getCoordinate('x');
	int y = getCoordinate('y');
	int z = getCoordinate('z');

	// Get local geometry
	int g = getGeometry(x, y, z);

	// Set current node
	global float* Src = Source + stride(x, y, z);
	global float* Dst = Destination + stride(x, y, z);

	// Local density distribution
	float f00, f01, f02, f03, f04, f05, f06,
	    f07, f08, f09, f10, f11, f12, f13, f14, f15, f16, f17, f18;
	// Moments
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
	    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;
	// Auxiliary variables
	float t0, t1, t2, t3;

	// Fetch source density distribution
	f00 = src(0, 0);
	if (g & G_BND) {
		// Simple bounce-back boundary condition
		if (g & G_E02) f01 = src( 2, 0); else f01 = src( 1, -e01);
		if (g & G_E01) f02 = src( 1, 0); else f02 = src( 2, -e02);
		if (g & G_E04) f03 = src( 4, 0); else f03 = src( 3, -e03);
		if (g & G_E03) f04 = src( 3, 0); else f04 = src( 4, -e04);
		if (g & G_E06) f05 = src( 6, 0); else f05 = src( 5, -e05);
		if (g & G_E05) f06 = src( 5, 0); else f06 = src( 6, -e06);
		if (g & G_E10) f07 = src(10, 0); else f07 = src( 7, -e07);
		if (g & G_E09) f08 = src( 9, 0); else f08 = src( 8, -e08);
		if (g & G_E08) f09 = src( 8, 0); else f09 = src( 9, -e09);
		if (g & G_E07) f10 = src( 7, 0); else f10 = src(10, -e10);
		if (g & G_E14) f11 = src(14, 0); else f11 = src(11, -e11);
		if (g & G_E13) f12 = src(13, 0); else f12 = src(12, -e12);
		if (g & G_E12) f13 = src(12, 0); else f13 = src(13, -e13);
		if (g & G_E11) f14 = src(11, 0); else f14 = src(14, -e14);
		if (g & G_E18) f15 = src(18, 0); else f15 = src(15, -e15);
		if (g & G_E17) f16 = src(17, 0); else f16 = src(16, -e16);
		if (g & G_E16) f17 = src(16, 0); else f17 = src(17, -e17);
		if (g & G_E15) f18 = src(15, 0); else f18 = src(18, -e18);
		// Imposed lid velocity
		if (z == Lx-1) {
		    f13 += k0;
		    f14 -= k0;
		}
	} else {
		// Pull propagation
		f01 = src( 1, -e01);
		f02 = src( 2, -e02);
		f03 = src( 3, -e03);
		f04 = src( 4, -e04);
		f05 = src( 5, -e05);
		f06 = src( 6, -e06);
		f07 = src( 7, -e07);
		f08 = src( 8, -e08);
		f09 = src( 9, -e09);
		f10 = src(10, -e10);
		f11 = src(11, -e11);
		f12 = src(12, -e12);
		f13 = src(13, -e13);
		f14 = src(14, -e14);
		f15 = src(15, -e15);
		f16 = src(16, -e16);
		f17 = src(17, -e17);
		f18 = src(18, -e18);
	}
    
	// Computation of moments
	t0 = f01 + f02 + f03 + f04 + f05 + f06;
	t1 = f07 + f08 + f09 + f10 + f11 + f12 + f13 + f14
	  + f15 + f16 + f17 + f18;
	rho = f00 + t0 + t1;
	e = -30*f00 - 11*t0 + 8*t1; 
	eps = 12*f00 - 4*t0 + t1;
	t0 = f01 - f02;
	t1 = f07 - f08 + f09 - f10 + f11 - f12 + f13 - f14;
	jx = t0 + t1;
	qx = -4*t0 + t1;
	t0 = f03 - f04;
	t1 = f07 + f08 - f09 - f10 + f15 - f16 + f17 - f18;
	jy = t0 + t1;
	qy = -4*t0 + t1;
	t0 = f05 - f06;
	t1 = f11 + f12 - f13 - f14 + f15 + f16 - f17 - f18;
	jz = t0 + t1;
	qz = -4*t0 + t1;

	if (Destination) {

		// Default case

		t0 = 2*(f01 + f02) - (f03 + f04 + f05 + f06);
		t1 = (f07 + f08 + f09 + f10 + f11 + f12 + f13 + f14)
		 - 2*(f15 + f16 + f17 + f18);
		px = t0 + t1;
		pix = -2*t0 + t1;
		t0 = (f03 + f04) - (f05 + f06);
		t1 = (f07 + f08 + f09 + f10) - (f11 + f12 + f13 + f14);
		pw = t0 + t1;
		piw = -2*t0 + t1;
		pxy = f07 - f08 - f09 + f10;
		pyz = f15 - f16 - f17 + f18;
		pxz = f11 - f12 - f13 + f14;
		mx = f07 - f08 + f09 - f10 - f11 + f12 - f13 + f14;
		my = -f07 - f08 + f09 + f10 + f15 - f16 + f17 - f18;
		mz = f11 + f12 - f13 - f14 - f15 - f16 + f17 + f18;

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
		f00 = c0*rho - c1*e + c4*eps;

		t0 = c0*rho - c2*e - c5*eps;
		t1 = px - pix;
		t2 = pw - piw;
		f01 = t0 + c9*t1 + c7*(jx - qx);
		f02 = t0 + c9*t1 - c7*(jx - qx);
		f03 = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
		f04 = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
		f05 = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
		f06 = t0 - c10*t1 - c12*t2 - c7*(jz - qz);

		t0 = c0*rho + c3*e + c6*eps;
		t1 = c7*jx + c8*qx;
		t2 = c7*jy + c8*qy;
		t3 = c7*jz + c8*qz;
		f07 = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
			+ c15*(mx - my);
		f08 = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
			- c15*(mx + my);
		f09 = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
			+ c15*(mx + my);
		f10 = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
			- c15*(mx - my);
		f11 = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
			- c15*(mx - mz);
		f12 = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
			+ c15*(mx + mz);
		f13 = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
			- c15*(mx + mz);
		f14 = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
			+ c15*(mx - mz);
		f15 = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
		f16 = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
		f17 = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
		f18 = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);

		dst( 0) = f00;
		dst( 1) = f01;
		dst( 2) = f02;
		dst( 3) = f03;
		dst( 4) = f04;
		dst( 5) = f05;
		dst( 6) = f06;
		dst( 7) = f07;
		dst( 8) = f08;
		dst( 9) = f09;
		dst(10) = f10;
		dst(11) = f11;
		dst(12) = f12;
		dst(13) = f13;
		dst(14) = f14;
		dst(15) = f15;
		dst(16) = f16;
		dst(17) = f17;
		dst(18) = f18;

	} else if (Delta) {

		// Alternate case: monitoring

		// Velocity components
		float ux, uy, uz;

		// Set current node
		global float* Vel = Velocity + 3*offset(x, y, z);
		global float* Del = Delta + offset(x, y, z);

		// Fetch stored velocity
		ux = Vel[0];
		uy = Vel[1];
		uz = Vel[2];

		// Store current velocity
		float vx = jx/(1 + rho);
		float vy = jy/(1 + rho);
		float vz = jz/(1 + rho);
		Vel[0] = vx;
		Vel[1] = vy;
		Vel[2] = vz;

		// Compute and store squared velocity variation
		*Del = (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);

		// Write bitmap pixel
		if (Bitmap && y == (Ly>>1)){
			writePixel(Bitmap, x, z, 4*sqrt(vx*vx + vy*vy + vz*vz)/U0);
		}
	} else if (Density) {

		// Alternate case: extraction

		// Set current node
		global float* Den = Density + offset(x, y, z);
		global float* Vel = Velocity + 3*offset(x, y, z);

		// Store current density and velocity
		*Den = 1 + rho;
		Vel[0] = jx/(1 + rho);
		Vel[1] = jy/(1 + rho);
		Vel[2] = jz/(1 + rho);

	}
}
#else

#define WG_SIZE 16

// kernel compute with reorganized boundary and __local mem
kernel void compute(global float* Source, global float* Destination,
	global float* Density, global float* Velocity,
	global float* Delta, global uchar* Bitmap)
{

	// Retrieve coordinates
	int x = getCoordinate('x');
	int y = getCoordinate('y');
	int z = getCoordinate('z');

	// Get local geometry
	int g = getGeometry(x, y, z);

	// Set current node
	global float* Src = Source + stride(x, y, z);
	global float* Dst = Destination + stride(x, y, z);

	// Local density distribution
	int lid = get_local_id(0);
	__local float localf[WG_SIZE][19];

	// Moments
	float rho, e, eps, jx, qx, jy, qy, jz, qz,
	    px, pix, pw, piw, pxy, pyz, pxz, mx, my, mz;
	// Auxiliary variables
	float t0, t1, t2, t3;
	int i;

	// Fetch source density distribution
	if(lid == 0){
		for (i = 0; i < WG_SIZE; i++)
		{
			// printf("lx %d ly %d lz %d\n", lx, ly, lz);
			g = getGeometry(x+i, y, z);
			if (g & G_BND) {
				if (g & G_E09) localf[i][ 8] = src( 9, 0); else localf[i][ 8] = src( 8, -e08);
				if (g & G_E04) localf[i][ 3] = src( 4, 0); else localf[i][ 3] = src( 3, -e03);
				if (g & G_E10) localf[i][ 7] = src(10, 0); else localf[i][ 7] = src( 7, -e07);
			} else {
				localf[i][ 8] = src( 8, -e08);
				localf[i][ 3] = src( 3, -e03);
				localf[i][ 7] = src( 7, -e07);
			}
		}

		// for (int i = 0; i < WG_SIZE; ++i)
		// {
		// 	g = getGeometry(x+i, y, z);
		// 	localf[i][0] = src(0, 0);
		// 	if (g & G_BND) {
		// 		if (g & G_E02) localf[i][ 1] = src( 2, 0); else localf[i][ 1] = src( 1, -e01);
		// 		if (g & G_E01) localf[i][ 2] = src( 1, 0); else localf[i][ 2] = src( 2, -e02);
		// 	} else {
		// 		localf[i][ 1] = src( 1, -e01);
		// 		localf[i][ 2] = src( 2, -e02);
		// 	}
		// }

		// for (int i = 0; i < WG_SIZE; ++i)
		// {
		// 	g = getGeometry(x+i, y, z);
		// 	if (g & G_BND) {
		// 		if (g & G_E07) localf[i][10] = src( 7, 0); else localf[i][10] = src(10, -e10);
		// 		if (g & G_E03) localf[i][ 4] = src( 3, 0); else localf[i][ 4] = src( 4, -e04);
		// 		if (g & G_E08) localf[i][ 9] = src( 8, 0); else localf[i][ 9] = src( 9, -e09);
		// 	} else {
		// 		localf[i][10] = src(10, -e10);
		// 		localf[i][ 4] = src( 4, -e04);
		// 		localf[i][ 9] = src( 9, -e09);
		// 	}
		// }

		// for (int i = 0; i < WG_SIZE; ++i)
		// {
		// 	g = getGeometry(x+i, y, z);
		// 	if (g & G_BND) {
		// 		if (g & G_E18) localf[i][15] = src(18, 0); else localf[i][15] = src(15, -e15);
		// 		if (g & G_E13) localf[i][12] = src(13, 0); else localf[i][12] = src(12, -e12);
		// 		if (g & G_E06) localf[i][ 5] = src( 6, 0); else localf[i][ 5] = src( 5, -e05);
		// 		if (g & G_E14) localf[i][11] = src(14, 0); else localf[i][11] = src(11, -e11);
		// 		if (g & G_E17) localf[i][16] = src(17, 0); else localf[i][16] = src(16, -e16);
		// 	} else {
		// 		localf[i][15] = src(15, -e15);
		// 		localf[i][12] = src(12, -e12);
		// 		localf[i][ 5] = src( 5, -e05);
		// 		localf[i][11] = src(11, -e11);
		// 		localf[i][16] = src(16, -e16);
		// 	}
		// }

		// for (int i = 0; i < WG_SIZE; ++i)
		// {
		// 	g = getGeometry(x+i, y, z);
		// 	if (g & G_BND) {
		// 		if (g & G_E16) localf[i][17] = src(16, 0); else localf[i][17] = src(17, -e17);
		// 		if (g & G_E11) localf[i][14] = src(11, 0); else localf[i][14] = src(14, -e14);
		// 		if (g & G_E05) localf[i][ 6] = src( 5, 0); else localf[i][ 6] = src( 6, -e06);
		// 		if (g & G_E12) localf[i][13] = src(12, 0); else localf[i][13] = src(13, -e13);
		// 		if (g & G_E15) localf[i][18] = src(15, 0); else localf[i][18] = src(18, -e18);
		// 		// Imposed lid velocity
		// 		if (z == Lx-1) {
		// 			localf[i][13] += k0;
		// 			localf[i][14] -= k0;
		// 		}
		// 	} else {
		// 		localf[i][17] = src(17, -e17);
		// 		localf[i][14] = src(14, -e14);
		// 		localf[i][ 6] = src( 6, -e06);
		// 		localf[i][13] = src(13, -e13);
		// 		localf[i][18] = src(18, -e18);
		// 	}
		// }
	}
	barrier(CLK_LOCAL_MEM_FENCE);

	// Computation of moments
	t0 = localf[lid][1] + localf[lid][2] + localf[lid][3] + localf[lid][4] + localf[lid][5] + localf[lid][6];
	t1 = localf[lid][7] + localf[lid][8] + localf[lid][9] + localf[lid][10] + localf[lid][11] + localf[lid][12] + localf[lid][13] + localf[lid][14]
	  + localf[lid][15] + localf[lid][16] + localf[lid][17] + localf[lid][18];
	rho = localf[lid][0] + t0 + t1;
	e = -30*localf[lid][0] - 11*t0 + 8*t1; 
	eps = 12*localf[lid][0] - 4*t0 + t1;
	t0 = localf[lid][1] - localf[lid][2];
	t1 = localf[lid][7] - localf[lid][8] + localf[lid][9] - localf[lid][10] + localf[lid][11] - localf[lid][12] + localf[lid][13] - localf[lid][14];
	jx = t0 + t1;
	qx = -4*t0 + t1;
	t0 = localf[lid][3] - localf[lid][4];
	t1 = localf[lid][7] + localf[lid][8] - localf[lid][9] - localf[lid][10] + localf[lid][15] - localf[lid][16] + localf[lid][17] - localf[lid][18];
	jy = t0 + t1;
	qy = -4*t0 + t1;
	t0 = localf[lid][5] - localf[lid][6];
	t1 = localf[lid][11] + localf[lid][12] - localf[lid][13] - localf[lid][14] + localf[lid][15] + localf[lid][16] - localf[lid][17] - localf[lid][18];
	jz = t0 + t1;
	qz = -4*t0 + t1;

	if (Destination) {

		// Default case

		t0 = 2*(localf[lid][1] + localf[lid][2]) - (localf[lid][3] + localf[lid][4] + localf[lid][5] + localf[lid][6]);
		t1 = (localf[lid][7] + localf[lid][8] + localf[lid][9] + localf[lid][10] + localf[lid][11] + localf[lid][12] + localf[lid][13] + localf[lid][14])
		 - 2*(localf[lid][15] + localf[lid][16] + localf[lid][17] + localf[lid][18]);
		px = t0 + t1;
		pix = -2*t0 + t1;
		t0 = (localf[lid][3] + localf[lid][4]) - (localf[lid][5] + localf[lid][6]);
		t1 = (localf[lid][7] + localf[lid][8] + localf[lid][9] + localf[lid][10]) - (localf[lid][11] + localf[lid][12] + localf[lid][13] + localf[lid][14]);
		pw = t0 + t1;
		piw = -2*t0 + t1;
		pxy = localf[lid][7] - localf[lid][8] - localf[lid][9] + localf[lid][10];
		pyz = localf[lid][15] - localf[lid][16] - localf[lid][17] + localf[lid][18];
		pxz = localf[lid][11] - localf[lid][12] - localf[lid][13] + localf[lid][14];
		mx = localf[lid][7] - localf[lid][8] + localf[lid][9] - localf[lid][10] - localf[lid][11] + localf[lid][12] - localf[lid][13] + localf[lid][14];
		my = -localf[lid][7] - localf[lid][8] + localf[lid][9] + localf[lid][10] + localf[lid][15] - localf[lid][16] + localf[lid][17] - localf[lid][18];
		mz = localf[lid][11] + localf[lid][12] - localf[lid][13] - localf[lid][14] - localf[lid][15] - localf[lid][16] + localf[lid][17] + localf[lid][18];

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
		localf[lid][0] = c0*rho - c1*e + c4*eps;

		t0 = c0*rho - c2*e - c5*eps;
		t1 = px - pix;
		t2 = pw - piw;
		localf[lid][1] = t0 + c9*t1 + c7*(jx - qx);
		localf[lid][2] = t0 + c9*t1 - c7*(jx - qx);
		localf[lid][3] = t0 - c10*t1 + c12*t2 + c7*(jy - qy);
		localf[lid][4] = t0 - c10*t1 + c12*t2 - c7*(jy - qy);
		localf[lid][5] = t0 - c10*t1 - c12*t2 + c7*(jz - qz);
		localf[lid][6] = t0 - c10*t1 - c12*t2 - c7*(jz - qz);

		t0 = c0*rho + c3*e + c6*eps;
		t1 = c7*jx + c8*qx;
		t2 = c7*jy + c8*qy;
		t3 = c7*jz + c8*qz;
		localf[lid][7] = t0 + t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
			+ c15*(mx - my);
		localf[lid][8] = t0 - t1 + t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
			- c15*(mx + my);
		localf[lid][9] = t0 + t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw - c14*pxy
			+ c15*(mx + my);
		localf[lid][10] = t0 - t1 - t2 + c10*px + c11*pix + c12*pw + c13*piw + c14*pxy
			- c15*(mx - my);
		localf[lid][11] = t0 + t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
			- c15*(mx - mz);
		localf[lid][12] = t0 - t1 + t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
			+ c15*(mx + mz);
		localf[lid][13] = t0 + t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw - c14*pxz
			- c15*(mx + mz);
		localf[lid][14] = t0 - t1 - t3 + c10*px + c11*pix - c12*pw - c13*piw + c14*pxz
			+ c15*(mx - mz);
		localf[lid][15] = t0 + t2 + t3 - c9*px - c10*pix + c14*pyz + c15*(my - mz);
		localf[lid][16] = t0 - t2 + t3 - c9*px - c10*pix - c14*pyz - c15*(my + mz);
		localf[lid][17] = t0 + t2 - t3 - c9*px - c10*pix - c14*pyz + c15*(my + mz);
		localf[lid][18] = t0 - t2 - t3 - c9*px - c10*pix + c14*pyz - c15*(my - mz);

		dst( 0) = localf[lid][0];
		dst( 1) = localf[lid][1];
		dst( 2) = localf[lid][2];
		dst( 3) = localf[lid][3];
		dst( 4) = localf[lid][4];
		dst( 5) = localf[lid][5];
		dst( 6) = localf[lid][6];
		dst( 7) = localf[lid][7];
		dst( 8) = localf[lid][8];
		dst( 9) = localf[lid][9];
		dst(10) = localf[lid][10];
		dst(11) = localf[lid][11];
		dst(12) = localf[lid][12];
		dst(13) = localf[lid][13];
		dst(14) = localf[lid][14];
		dst(15) = localf[lid][15];
		dst(16) = localf[lid][16];
		dst(17) = localf[lid][17];
		dst(18) = localf[lid][18];

	} else if (Delta) {

		// Alternate case: monitoring

		// Velocity components
		float ux, uy, uz;

		// Set current node
		global float* Vel = Velocity + 3*offset(x, y, z);
		global float* Del = Delta + offset(x, y, z);

		// Fetch stored velocity
		ux = Vel[0];
		uy = Vel[1];
		uz = Vel[2];

		// Store current velocity
		float vx = jx/(1 + rho);
		float vy = jy/(1 + rho);
		float vz = jz/(1 + rho);
		Vel[0] = vx;
		Vel[1] = vy;
		Vel[2] = vz;

		// Compute and store squared velocity variation
		*Del = (ux-vx)*(ux-vx) + (uy-vy)*(uy-vy) + (uz-vz)*(uz-vz);

		// Write bitmap pixel
		if (Bitmap && y == (Ly>>1)){
			writePixel(Bitmap, x, z, 4*sqrt(vx*vx + vy*vy + vz*vz)/U0);
		}
	} else if (Density) {

		// Alternate case: extraction

		// Set current node
		global float* Den = Density + offset(x, y, z);
		global float* Vel = Velocity + 3*offset(x, y, z);

		// Store current density and velocity
		*Den = 1 + rho;
		Vel[0] = jx/(1 + rho);
		Vel[1] = jy/(1 + rho);
		Vel[2] = jz/(1 + rho);

	}
}
#endif

#endif // USE_CLSTENCIL
