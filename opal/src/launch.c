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
 *  launch.c: kernels launch functions.
 *
 */

#include "opal.h"

/*
 *  launchInitKernel: launch initialisation kernel.
 */

void launchInitKernel(frame* Frame, app* App, data* Data, results* Results)
{
    cl_int Status;

    Status = clEnqueueNDRangeKernel(Frame->Queue, App->Init, D, NULL,
            App->GlobalWorkSize, App->LocalWorkSize, 0, NULL, NULL);
    exitOnFail(Status, "Failed to launch 'init' kernel.\n");
    Results->L2 = 0;
    if (Results->Bitmap) {
        Status = clEnqueueReadBuffer(Frame->Queue, Data->Bitmap, CL_TRUE, 0,
                Data->BitmapSize*sizeof(cl_uchar), Results->Bitmap, 0, NULL,
                NULL);
        exitOnFail(Status, "Failed to copy image bitmap to host memory.\n");
    }
}

/*
 *  launchComputeKernel: launch either ComputeOdd or ComputeEven kernel
 *  depending on parity of current time step.
 */

void launchComputeKernel(frame* Frame, app* App, int Time)
{
    cl_int Status;

    if (Time & 1) 
        Status = clEnqueueNDRangeKernel(Frame->Queue, App->ComputeOdd, D, NULL,
                App->GlobalWorkSize, App->LocalWorkSize, 0, NULL, NULL);
    else
        Status = clEnqueueNDRangeKernel(Frame->Queue, App->ComputeEven, D, NULL,
                App->GlobalWorkSize, App->LocalWorkSize, 0, NULL, NULL);
    exitOnFail(Status, "Failed to launch 'compute' kernel.\n");
}

/*
 *  launchMonitorKernel: launch monitoring kernel and transfer monitoring data
 *  to host memory.
 */

void launchMonitorKernel(frame* Frame, app* App, data* Data, results* Results)
{
    cl_int Status;
    int Size = Results->DomainSize;

    Status = clEnqueueNDRangeKernel(Frame->Queue, App->Monitor, D, NULL,
            App->GlobalWorkSize, App->LocalWorkSize, 0, NULL, NULL);
    exitOnFail(Status, "Failed to launch 'monitor' kernel.\n");
    
    Status = clEnqueueReadBuffer(Frame->Queue, Data->Delta, CL_TRUE, 0,
            Data->DomainSize*sizeof(cl_float), Results->Delta, 0, NULL,
            NULL);
    exitOnFail(Status, "Failed to copy velocity variation array to host"
	    " memory.\n");
    Results->L2 = sqrt(sumArray(Results->Delta, Size)/Size);
    if (Results->L2 < Results->epsilon)
	Results->Converged = true;
    
    if (Results->Bitmap) {
        Status = clEnqueueReadBuffer(Frame->Queue, Data->Bitmap, CL_TRUE, 0,
                Data->BitmapSize*sizeof(cl_uchar), Results->Bitmap, 0, NULL,
                NULL);
        exitOnFail(Status, "Failed to copy image bitmap to host memory.\n");
    }
}

/*
 *  launchExtractKernel: launch results extraction kernel and transfer
 *  simulation data to host memory.
 */

void launchExtractKernel(frame* Frame, app* App, data* Data, results* Results)
{
    cl_int Status;

    Status = clEnqueueNDRangeKernel(Frame->Queue, App->Extract, D, NULL,
            App->GlobalWorkSize, App->LocalWorkSize, 0, NULL, NULL);
    exitOnFail(Status, "Failed to launch 'extract' kernel.\n");
    
    Status = clEnqueueReadBuffer(Frame->Queue, Data->Density, CL_TRUE, 0,
            Data->DomainSize*sizeof(cl_float), Results->Density, 0, NULL,
            NULL);
    exitOnFail(Status, "Failed to copy density array to host memory.\n");
    
    Status = clEnqueueReadBuffer(Frame->Queue, Data->Velocity, CL_TRUE, 0,
            D*Data->DomainSize*sizeof(cl_float), Results->Velocity, 0, NULL,
            NULL);
    exitOnFail(Status, "Failed to copy velocity array to host memory.\n");
}
