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
 *  init.cl: initialisation kernel.
 *
 */
#ifdef USE_CLSTENCIL

/*
 * Init memory using clStencil. 
 */
kernel void init(global float* Source, global float* Velocity,
		  global uchar* Bitmap)
{
#if 0
	// Retrieve coordinates
	int x = getCoordinate('x');
	int y = getCoordinate('y');
	int z = getCoordinate('z');

	// Register regions :
	clst_region_t region_src = {0};
	clst_region_t region_vel = {0};
	clst_region_t region_bmp = {0};

	region_src = clst_register_memory_region((__global void*)Source, CLST_GENTYPE_FLOAT, 
	                  Lx, Ly, Lz, CLST_MODEL_D3Q19, Q);
	region_vel = clst_register_memory_region((__global void*)Velocity, CLST_GENTYPE_FLOAT, 
	                  Lx, Ly, Lz, CLST_MODEL_D3Q19, D);

	// Set Source 
	clst_set_region_to_zero(&region_src, x, y, z);

	// Set Velovity
	clst_set_region_to_zero(&region_vel, x, y, z);

	clst_disregister_memory_region(&region_src);
	clst_disregister_memory_region(&region_vel);
	
	// Set Bitmap
	if (Bitmap && y == (Ly>>1)){
		writePixel(Bitmap, x, z, 0);
	}
#endif
}

// #elif defined(MPPA)

// /*
//  * Init memory using blocking. 
//  */
// kernel void init(global float* Source, global float* Velocity,
// 		  global uchar* Bitmap)
// {

// 	global float* Src;
// 	global float* Vel;
// 	global uchar* Bmp;

// 	// Get the best 3D blocking size to fit page size
// 	int blk_x = get_local_size(0);
// 	int blk_y = get_local_size(1);
// 	int blk_z = (CL_MPPA_PAGE_SIZE_KALRAY/sizeof(float)) / (blk_x*blk_y);

// 	// Init Source 4D : Q x Lx x Ly x Lz
// 	#pragma unroll 
// 	for(int q = 0; q < Q; q++){
// 		// jump to q-th space of Lx*Ly*Lz
// 		Src = Source + q*Lx*Ly*Lz;

// 		// then each WG jumps to it workspace : blk_x*blk_y*blk_z
// 		Src +=  get_group_id(0)*blk_x*blk_y*blk_z;

// 		// then each workitem jumps to its workspace following x-dim : blk_y*blk_z
// 		Src += get_local_id(0)*blk_y*blk_z;

// 		// do block blk_y * blk_z
// 		for(int y = 0; y < blk_y; y++){
// 			for(int z = 0; z < blk_z; z++){ // can replace by memset
// 				 Src[y*blk_y + z] = 0;
// 			}
// 		}
// 	}

// 	// Init Velocity 4D : D x Lx x Ly x Lz
// 	#pragma unroll 
// 	for(int d = 0; d < D; d++){
// 		// jump to d-th space of Lx*Ly*Lz
// 		Vel = Velocity + d*Lx*Ly*Lz;

// 		// then each WG jumps to it workspace : blk_x*blk_y*blk_z
// 		Vel +=  get_group_id(0)*blk_x*blk_y*blk_z;

// 		// then each workitem jumps to its workspace following x-dim : blk_y*blk_z
// 		Vel += get_local_id(0)*blk_y*blk_z;

// 		// do block blk_y * blk_z
// 		for(int y = 0; y < blk_y; y++){
// 			for(int z = 0; z < blk_z; z++){ // can replace by memset
// 				 Vel[y*blk_y + z] = 0;
// 			}
// 		}
// 	}

// 	// Init Bitmap 3D : D x Lx x Lz
// 	if (Bitmap){
// 		// since Bitmap is global uchar*, and of size D*Lx*Lz, recalculate blk_z
// 		blk_z = (CL_MPPA_PAGE_SIZE_KALRAY/sizeof(uchar)) / (blk_x);
// 		#pragma unroll 
// 		for(int d = 0; d < D; d++){
// 			// jump to d-th space of Lx*Ly*Lz
// 			Bmp = Bitmap + d*Lx*Lz;

// 			// then each WG jumps to it workspace : blk_x*blk_z
// 			Bmp +=  get_group_id(0)*blk_x*blk_z;

// 			// then each workitem jumps to its workspace following x-dim : blk_z
// 			Bmp += get_local_id(0)*blk_z;

// 			// do blk_z
// 			for(int z = 0; z < blk_z; z++){ // can replace by memset
// 				 Bmp[z] = 0;
// 			}
// 		}
// 	}
// }

#else // not CLSTENCIL 

kernel void init(global float* Source, global float* Velocity,
		  global uchar* Bitmap)
{
#if 1
	 // Retrieve coordinates
	 int x = getCoordinate('x');
	 int y = getCoordinate('y');
	 int z = getCoordinate('z');

	 // Set current node
	 global float* Src = Source + stride(x, y, z);
	 global float* Vel = Velocity + 3*offset(x, y, z);

	 // Set density distribution to zero
	 for (int a = 0; a < Q; a++) src(a, 0) = 0;

	 // Set velocity field to zero
	 Vel[0] = 0;
	 Vel[1] = 0;
	 Vel[2] = 0;

	 // Write initial bitmap
	 if (Bitmap && y == (Ly>>1)){
		  writePixel(Bitmap, x, z, 0);
	 }
#endif	 
}

#endif // CLSTENCIL
