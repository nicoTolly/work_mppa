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
 *  frame.c: frame structure creation and destruction functions.
 *
 */

#include "opal.h"

/*
 *  createFrame: create OpenCL working frame.
 */

frame* createFrame(config* Config)
{
    frame* Frame;

    cl_platform_id Platform[2];
    char platform_vendor[100];
    char device_name[256];
    cl_uint nb_platforms;
    cl_int Status;
    cl_uint i;
    cl_device_id tmp_device[2];
    cl_uint nb_devices;

    Frame = malloc(sizeof(frame));
    if (Frame == NULL)
        eprintf("Failed to allocate memory for 'Frame' structure.\n");

    // OpenCL platform
    Status = clGetPlatformIDs(2, Platform, &nb_platforms);
    exitOnFail(Status, "Could not get OpenCL platform.\n");

    // Print info about platforms
    for(i = 0; i < nb_platforms; i++){

        Status = clGetPlatformInfo(Platform[i], CL_PLATFORM_VENDOR, 100, platform_vendor, NULL);
        exitOnFail(Status, "Could not get OpenCL platform info vendor.\n");
        printf("# Platform vendor : %s \n", platform_vendor);

        Status = clGetDeviceIDs(Platform[i], CL_DEVICE_TYPE_ALL, 2, tmp_device, &nb_devices);
        if (Status != CL_SUCCESS) printf("Could not get OpenCL all devices of platform %s.\n", platform_vendor);

        cl_uint j;
        for(j = 0; j < nb_devices; j++){
            Status = clGetDeviceInfo(tmp_device[j], CL_DEVICE_NAME, 256, device_name, NULL);
            if (Status != CL_SUCCESS) printf("Could not get OpenCL device %d name of platform %s.\n", j, platform_vendor);
            printf("#\t - Device name %s\n", device_name);
        }
    }

    for(i = 0; i < nb_platforms; i++){
        // OpenCL device
        Status = clGetDeviceIDs(Platform[i], CL_DEVICE, 1, tmp_device, NULL);
        // If device type is not found on this platform, jump to next platform
        if (Status != CL_SUCCESS) continue;

        // Found device for this platform, get information
        Frame->Device = tmp_device[0];
        Status = clGetDeviceInfo(Frame->Device, CL_DEVICE_NAME, 256, device_name, NULL);
        exitOnFail(Status, "Could not get OpenCL device name.\n");
        printf("# Choosing device %s \n", device_name);
        break;
    }
    
    exitOnFail(Status, "Could not get OpenCL Device.\n");

    // OpenCL context
    Frame->Context = clCreateContext(0, 1, &(Frame->Device), NULL, NULL,
            &Status);
    exitOnFail(Status, "Could not create OpenCL Context.\n");

    // Associated OpenCL commmand queue
    Frame->Queue = clCreateCommandQueue(Frame->Context, Frame->Device, 0,
            &Status);
    exitOnFail(Status, "Could not create OpenCL CommandQueue.\n");

    return Frame;
}

/*
 *  clearFrame: clear Frame container.
 */

void clearFrame(frame* Frame)
{
    clReleaseCommandQueue(Frame->Queue);
    clReleaseContext(Frame->Context);
    free(Frame);
}
