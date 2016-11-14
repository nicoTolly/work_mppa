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
 *  app.c: App structure creation and destruction functions.
 *
 */

#include "opal.h"

/*
 *  createApp: create OpenCL kernels.
 */

app* createApp(config* Config, frame* Frame, data* Data)
{
    app* App;
    cl_int Status;
    char* Buffer;
    size_t Length;

    App = malloc(sizeof(app));
    if (App == NULL)
        eprintf("Failed to allocate memory for 'App' structure.\n");

    // Dimensions of computation domain
    App->GlobalWorkSize[0] = Config->Lx;
    App->GlobalWorkSize[1] = Config->Ly;
    App->GlobalWorkSize[2] = Config->Lz;

    // Dimension of the workgroups
    App->LocalWorkSize[0] = Config->Nx;
    App->LocalWorkSize[1] = Config->Ny;
    App->LocalWorkSize[2] = Config->Nz;

    // Initialise source buffer with macro definitions for runtime parameters
    // This takes advantage of kernel runtime compilation, thus lowering
    // constant memory consumption.
    Length = asprintf(&Buffer,
#ifdef MPPA
        "#define MPPA\n"
    #ifdef LLVM
        "#define LLVM\n"        
    #else
	    "#include <math.h>\n"
	    "#define sqrt sqrtf\n"
    #endif        
#endif
#ifdef USE_CLSTENCIL
        "#define USE_CLSTENCIL\n"
#endif
            "#define Re %fF\n"
            "#define U0 %fF\n"
            "#define Lx %d\n"
            "#define Ly %d\n"
            "#define Lz %d\n"
            "#define sX %d\n"
            "#define sY %d\n"
            "#define sZ %d\n"
            "#define sA %d\n"
            "#define rX %d\n"
            "#define rY %d\n"
            "#define rZ %d\n",
            Config->Re,                         // Re
            Config->U0,                         // U0
            Config->Lx,                         // Lx
            Config->Ly,                         // Ly
            Config->Lz,                         // Lz
#ifdef CPU
            Q,                                  // sX    /-(CPU)
            Q*Config->Lx,                       // sY    |
            Q*Config->Lx*Config->Ly,            // sZ    |
            1,                                  // sA    |
#elif MIC                                       //       |
            1,                                  // sX    --(MIC)
            Q*Config->Lx,                       // sY    |
            Q*Config->Lx*Config->Ly,            // sZ    |
            Config->Lx,                         // sA    |
#elif MPPA                                      //       |
        #if 1
            Q,                                  // sX    /-(CPU)
            Q*Config->Lx,                       // sY    |
            Q*Config->Lx*Config->Ly,            // sZ    |
            1,                                  // sA    |
        #else
        #if 1
            1,                                  // sX    --(MIC)
            Q*Config->Lx,                       // sY    |
            Q*Config->Lx*Config->Ly,            // sZ    |
            Config->Lx,                         // sA    |
        #else
            1,                                  // sX    \-(GPU)
            Config->Lx,                         // sY
            Config->Lx*Config->Ly,              // sZ
            Config->Lx*Config->Ly*Config->Lz,   // sA
        #endif
        #endif 
#else                                           //       |
            1,                                  // sX    \-(Other)
            Config->Lx,                         // sY
            Config->Lx*Config->Ly,              // sZ
            Config->Lx*Config->Ly*Config->Lz,   // sA
#endif
            1,                                  // rX
            Config->Lx,                         // rY
            Config->Lx*Config->Ly);             // rZ

    // Read constants header and OpenCL sources
    readFile(SRC_DIR "const.h", &Buffer, &Length);
#ifdef USE_CLSTENCIL
    readFile(SRC_DIR "../clstencil/clstencil.h", &Buffer, &Length);
    readFile(SRC_DIR "../clstencil/clstencil.cl", &Buffer, &Length);    
#endif    
    readFile(SRC_DIR "global.cl", &Buffer, &Length);
    readFile(SRC_DIR "geometry.cl", &Buffer, &Length);
    readFile(SRC_DIR "init.cl", &Buffer, &Length);
    readFile(SRC_DIR "compute.cl", &Buffer, &Length);
    App->Program = clCreateProgramWithSource(Frame->Context, 1,
            (const char **)&Buffer, &Length, &Status);
    exitOnFail(Status, "Could not create OpenCL program.\n");

    // Build program
    Status = clBuildProgram(App->Program, 0, NULL,
#ifdef CLSTENCIL_NO_HEADER
        "-DCLSTENCIL_NO_HEADER "
#endif // CLSTENCIL_NO_HEADER

#ifdef CPU
        "-DCPU "
#elif defined(MIC)
        "-DMIC "
#elif defined(GPU) || defined(CUDA)
        "-DGPU "
#endif
        
#ifdef MPPA
	    "",
#else
            "-cl-fast-relaxed-math",
#endif
	    NULL, NULL);
    // Print OpenCL build log in case of compilation errors
    if (Status != CL_SUCCESS) 
        printBuildLog(App->Program, Frame->Device);

    // Create initialisation kernel
    App->Init = clCreateKernel(App->Program, "init", &Status);
    exitOnFail(Status, "Could not create 'Init' kernel.\n");
    Status = clSetKernelArg(App->Init, 0, sizeof(cl_mem),
            &(Data->LatticeEven));
    Status |= clSetKernelArg(App->Init, 1, sizeof(cl_mem),
            &(Data->Velocity));
    Status |= clSetKernelArg(App->Init, 2, sizeof(cl_mem),
	    &(Data->Bitmap));
    exitOnFail(Status, "Failed to set 'Init' kernel arguments.\n");

    // Create main computation kernel for even time steps
    App->ComputeEven = clCreateKernel(App->Program, "compute", &Status);
    exitOnFail(Status, "Could not create 'ComputeEven' kernel.\n");
    Status = clSetKernelArg(App->ComputeEven, 0, sizeof(cl_mem),
            &(Data->LatticeEven));
    Status |= clSetKernelArg(App->ComputeEven, 1, sizeof(cl_mem),
            &(Data->LatticeOdd));
    Status |= clSetKernelArg(App->ComputeEven, 2, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeEven, 3, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeEven, 4, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeEven, 5, sizeof(cl_mem),
            NULL);
    exitOnFail(Status, "Failed to set 'ComputeEven' kernel arguments.\n");

    // Create main computation kernel for odd time steps
    App->ComputeOdd = clCreateKernel(App->Program, "compute", &Status);
    exitOnFail(Status, "Could not create 'compute' kernel.\n");
    Status = clSetKernelArg(App->ComputeOdd, 0, sizeof(cl_mem),
            &(Data->LatticeOdd));
    Status |= clSetKernelArg(App->ComputeOdd, 1, sizeof(cl_mem),
            &(Data->LatticeEven));
    Status |= clSetKernelArg(App->ComputeOdd, 2, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeOdd, 3, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeOdd, 4, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->ComputeOdd, 5, sizeof(cl_mem),
            NULL);
    exitOnFail(Status, "Failed to set 'ComputeOdd' kernel arguments.\n");

    // Create monitoring kernel
    App->Monitor = clCreateKernel(App->Program, "compute", &Status);
    exitOnFail(Status, "Could not create 'Monitor' kernel.\n");
    Status = clSetKernelArg(App->Monitor, 0, sizeof(cl_mem),
            &(Data->LatticeEven));
    Status |= clSetKernelArg(App->Monitor, 1, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->Monitor, 2, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->Monitor, 3, sizeof(cl_mem),
            &(Data->Velocity));
    Status |= clSetKernelArg(App->Monitor, 4, sizeof(cl_mem),
	    &(Data->Delta));
    Status |= clSetKernelArg(App->Monitor, 5, sizeof(cl_mem),
	    &(Data->Bitmap));
    exitOnFail(Status, "Failed to set 'Monitor' kernel arguments.\n");

    // Create results extraction kernel
    App->Extract = clCreateKernel(App->Program, "compute", &Status);
    exitOnFail(Status, "Could not create 'Extract' kernel.\n");
    Status = clSetKernelArg(App->Extract, 0, sizeof(cl_mem),
            &(Data->LatticeEven));
    Status |= clSetKernelArg(App->Extract, 1, sizeof(cl_mem),
            NULL);
    Status |= clSetKernelArg(App->Extract, 2, sizeof(cl_mem),
            &(Data->Density));
    Status |= clSetKernelArg(App->Extract, 3, sizeof(cl_mem),
            &(Data->Velocity));
    Status |= clSetKernelArg(App->Extract, 4, sizeof(cl_mem),
	    NULL);
    Status |= clSetKernelArg(App->Extract, 5, sizeof(cl_mem),
	    NULL);
    exitOnFail(Status, "Failed to set 'Extract' kernel arguments.\n");

    return App;
}    

/*
 *  clearApp: clear OpenCL kernels container.
 */

void clearApp(app* App)
{
    clReleaseKernel(App->Init);
    clReleaseKernel(App->ComputeEven);
    clReleaseKernel(App->ComputeOdd);
    clReleaseKernel(App->Monitor);
    clReleaseKernel(App->Extract);
    clReleaseProgram(App->Program);
    free(App);
}
