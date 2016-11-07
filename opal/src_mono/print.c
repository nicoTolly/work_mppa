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
 *  print.c: log printing functions.
 *
 */

#include "opal.h"

/*
 *  Print log header.
 */

void printBanner(config* Config, results* Results)
{
    FILE* log = Results->LogFile;

    fprintf(log, "OpenCL Processor Array LBM solver");
#ifdef OPAL_VERSION
    fprintf(log, " version " OPAL_VERSION);
#endif
    fprintf(log, "\n\n");

    fprintf(log, "Domain: %dx%dx%d\n", Config->Lx, Config->Ly, Config->Lz);
    fprintf(log, "Workgroups: %dx%dx%d\n", Config->Nx, Config->Ny, Config->Nz);
    fprintf(log, "Time steps: %d\n", Config->Duration);
    fprintf(log, "Sampling period: %d\n", Config->Period);
    fprintf(log, "Reynolds number: %g\n", Config->Re);
    fprintf(log, "Imposed velocity: %g\n", Config->U0);
    if (Config->epsilon > 0)
	fprintf(log, "Convergence threshold: %g\n", Config->epsilon);

    fprintf(log, "\n");
}

/*
 *  Print OpenCL kernels build log to standard error output stream.
 */

// void printBuildLog(cl_program Program, cl_device_id Device)
// {
//     size_t Size;
//     char* BuildLog;
    
//     fprintf(stderr, "Failed to build OpenCL program (see log below).\n");
//     clGetProgramBuildInfo(Program, Device, CL_PROGRAM_BUILD_LOG, 0, NULL, &Size);
//     BuildLog = malloc(Size*sizeof(char));
//     if (BuildLog == NULL)
//         eprintf("Failed to allocate memory for log file buffer.\n");
//     clGetProgramBuildInfo(Program, Device, CL_PROGRAM_BUILD_LOG, Size, BuildLog,
//             NULL);
//     fprintf(stderr, "----------------------------------------"
//                     "----------------------------------------\n");
//     fprintf(stderr, BuildLog);
//     fprintf(stderr, "----------------------------------------"
//                     "----------------------------------------\n");
//     fprintf(stderr, "Exiting...\n");
//     exit(EXIT_FAILURE);
// }

/*
 *  Print monitoring results.
 */

void printResults(results* Results, int Time)
{
    fprintf(Results->LogFile, "%- 7d    %-.7f\n", Time, Results->L2);
    if (Results->Log) fflush(Results->LogFile);
}

/*
 *  Print log footer.
 */

void printSummary(results* Results, int Time, timespec* Start, timespec* Stop)
{
    FILE* log = Results->LogFile;
    double t = elapsedTime(Start, Stop);

    fputc('\n', log);
    fprintf(log, "Runtime: %g s\n", t);
    if (Time)
        fprintf(log, "Performance: %g MLUPS\n", (double)(Results->DomainSize)*Time*1E-6/t);
}
