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

data* createData(config* Config)
{
    data* Data;
    size_t DomainSize = Config->Lx*Config->Ly*Config->Lz;
    size_t BitmapSize = 3*Config->Lx*Config->Lz;

    Data = malloc(sizeof(data));
    if (Data == NULL)
        eprintf("Failed to allocate memory for 'Data' structure.\n");
    
    Data->DomainSize = DomainSize;  // Number of lattice nodes
    Data->BitmapSize = BitmapSize;  // Number of bytes for monitoring bitmap

    // // Source lattice array for even time steps
    // Data->LatticeEven = (float*)malloc(Q*DomainSize*sizeof(float));    
    // if(Data->LatticeEven == NULL) printf("Could not allocate 'LatticeEven' array.\n");

    // // Source lattice array for odd time steps
    // Data->LatticeOdd = (float*)malloc(Q*DomainSize*sizeof(float));    
    // if(Data->LatticeOdd == NULL) printf("Could not allocate 'LatticeOdd' array.\n");

    // // Fluid density
    // Data->Density = (float*)malloc(DomainSize*sizeof(float));
    // if(Data->Density == NULL) printf("Could not allocate 'Density' array.\n");

    // // Fluid velocity
    // Data->Velocity = (float*)malloc(D*DomainSize*sizeof(float));
    // if(Data->Velocity == NULL) printf("Could not allocate 'Velocity' array.\n");

    // // Partial sum of squared velocity variations within one period
    // Data->Delta = (float*)malloc(DomainSize*sizeof(float));
    // if(Data->Delta == NULL) printf("Could not allocate 'Delta' array.\n");

    // // Monitoring bitmap array
    // if (Config->Images) {
    //     Data->Bitmap = (unsigned char*)malloc(BitmapSize*sizeof(unsigned char));
    //     if(Data->Bitmap == NULL) printf("Could not allocate 'Bitmap' array.\n");
    // } else Data->Bitmap = NULL;

    return Data;
}

/*
 *  clearData: clear Data container.
 */

void clearData(data* Data)
{
    // free(Data->LatticeEven);
    // free(Data->LatticeOdd);
    // free(Data->Density);
    // free(Data->Velocity);
    // free(Data->Delta);
    // if (Data->Bitmap) free(Data->Bitmap);
    free(Data);
}

