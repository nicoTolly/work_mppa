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
 *  opal.h: global functions header.
 *
 */

#define OPAL_VERSION "0.2"
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

// #if defined CPU
// #define CL_DEVICE CL_DEVICE_TYPE_CPU
// #elif defined CUDA
// #define CL_DEVICE CL_DEVICE_TYPE_GPU
// #elif defined MIC
// #define CL_DEVICE CL_DEVICE_TYPE_ACCELERATOR
// #elif defined MPPA
// #define CL_DEVICE CL_DEVICE_TYPE_ACCELERATOR
// #include <CL/cl_kalray.h>
// #endif

#include "const.h"
#include "types.h"

void eprintf(const char* fmt, ...);

// void exitOnFail(cl_int Status, const char* Message);

double elapsedTime(timespec* Start, timespec* Stop);

void readFile(const char* Name, char** Contents, size_t* Length);

float sumArray(float* Array, size_t Size);

config* getConfig(int argc, char** argv);

// frame* createFrame(config* Config);

// void clearFrame(frame* Frame);

data* createData(config* Config);

void clearData(data* Data);

// app* createApp(config* Config, frame* Frame, data* Data);

// void clearApp(app* App);

results* createResults(config* Config);

void clearResults(results* Results);

// void launchInitKernel(frame* Frame, app* App, data* Data, results* Results);

// void launchComputeKernel(frame* Frame, app* App, int Time);

// void launchMonitorKernel(frame* Frame, app* App, data* Data, results* Results);

// void launchExtractKernel(frame* Frame, app* App, data* Data, results* Results);

void printBanner(config* Config, results* Results);

// void printBuildLog(cl_program Program, cl_device_id Device);

void printResults(results* Results, int Time);

void printSummary(results* Results, int Time, timespec* Start, timespec* Stop);

void exportImage(config* Config, results* Results, int Time);

void exportResults(results* Results, 
    float Density[Results->Lz][Results->Ly][Results->Lx],
    float Velocity[Results->Lz][Results->Ly][Results->Lx][D]);
