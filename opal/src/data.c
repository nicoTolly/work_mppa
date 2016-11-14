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
 *  data.c: Data structure creation and destruction functions.
 *
 */

#include "opal.h"

/*
 *  createData: create simulation data arrays in device memory.
 */

data* createData(config* Config, frame* Frame)
{
    data* Data;
    size_t DomainSize = Config->Lx*Config->Ly*Config->Lz;
    size_t BitmapSize = 3*Config->Lx*Config->Lz;
    cl_int Status;

    Data = malloc(sizeof(data));
    if (Data == NULL)
        eprintf("Failed to allocate memory for 'Data' structure.\n");
    
    Data->DomainSize = DomainSize;  // Number of lattice nodes
    Data->BitmapSize = BitmapSize;  // Number of bytes for monitoring bitmap

    // Source lattice array for even time steps
    Data->LatticeEven = clCreateBuffer(Frame->Context, CL_MEM_READ_WRITE,
            Q*DomainSize*sizeof(cl_float), NULL, &Status);
    exitOnFail(Status, "Could not allocate 'LatticeEven' array.\n");

    // Source lattice array for odd time steps
    Data->LatticeOdd = clCreateBuffer(Frame->Context, CL_MEM_READ_WRITE,
            Q*DomainSize*sizeof(cl_float), NULL, &Status);
    exitOnFail(Status, "Could not allocate 'LatticeOdd' array.\n");

    // Fluid density
    Data->Density = clCreateBuffer(Frame->Context, CL_MEM_WRITE_ONLY,
            DomainSize*sizeof(cl_float), NULL, &Status);
    exitOnFail(Status, "Could not allocate 'Density' array.\n");

    // Fluid velocity
    Data->Velocity = clCreateBuffer(Frame->Context, CL_MEM_WRITE_ONLY,
            D*DomainSize*sizeof(cl_float), NULL, &Status);
    exitOnFail(Status, "Could not allocate 'Velocity' array.\n");

    // Partial sum of squared velocity variations within one period
    Data->Delta = clCreateBuffer(Frame->Context, CL_MEM_WRITE_ONLY,
            DomainSize*sizeof(cl_float), NULL, &Status);
    exitOnFail(Status, "Could not allocate 'Delta' array.\n");

    // Monitoring bitmap array
    if (Config->Images) {
        Data->Bitmap = clCreateBuffer(Frame->Context, CL_MEM_WRITE_ONLY,
                BitmapSize*sizeof(cl_uchar), NULL, &Status);
        exitOnFail(Status, "Could not allocate 'Bitmap' array.\n");
    } else Data->Bitmap = NULL;

    return Data;
}

/*
 *  clearData: clear Data container.
 */

void clearData(data* Data)
{
    clReleaseMemObject(Data->LatticeEven);
    clReleaseMemObject(Data->LatticeOdd);
    clReleaseMemObject(Data->Density);
    clReleaseMemObject(Data->Velocity);
    clReleaseMemObject(Data->Delta);
    if (Data->Bitmap) clReleaseMemObject(Data->Bitmap);
    free(Data);
}
