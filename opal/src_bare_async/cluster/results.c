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
 *  results.c: Results structure creation and destruction functions.
 *
 */

#include "opal.h"

/*
 *  createResults: create simulation results arrays in host memory.
 */

results createResults(config* Config)
{
    results Results;
    size_t DomainSize = Config->Lx*Config->Ly*Config->Lz;
    size_t BitmapSize = 3*Config->Lx*Config->Lz;
    // char* FileName = NULL;

    // Results = malloc(sizeof(results));
    // if (Results == NULL)
    //     eprintf("Failed to allocate memory for 'Results' structure.\n");

    Results.Lx = Config->Lx;
    Results.Ly = Config->Ly;
    Results.Lz = Config->Lz;
    Results.L2 = 0;
    Results.DomainSize = DomainSize;
    Results.BitmapSize = BitmapSize;

    // // Fluid density
    // Results->Density = malloc(DomainSize*sizeof(float));
    // if (Results->Density == NULL)
    //     eprintf("Could not allocate 'Density' array.\n");

    // // Fluid velocity
    // Results->Velocity = malloc(D*DomainSize*sizeof(float));
    // if (Results->Velocity == NULL)
    //     eprintf("Could not allocate 'Velocity' array.\n");

    // // Squared velocity variations 
    // Results->Delta = malloc(DomainSize*sizeof(float));
    // if (Results->Delta == NULL)
    //     eprintf("Could not allocate 'Delta' array.\n");

    // Convergence threshold and flag
    Results.epsilon = Config->epsilon;
    Results.Converged = 0;

    // Optional monitoring bitmap
    if (Config->Images) {
	// Bitmap array
        // Results->Bitmap = malloc(BitmapSize*sizeof(unsigned char));
        // if (Results->Bitmap == NULL)
        //     eprintf("Could not allocate 'Bitmap' array.\n");
	// BMP header
        Results.Header[0]  = BitmapSize + 54;
        Results.Header[1]  = 0;
        Results.Header[2]  = 54;
        Results.Header[3]  = 40;
        Results.Header[4]  = Config->Lx;
        Results.Header[5]  = Config->Lz;
        Results.Header[6]  = 0x00180001;
        Results.Header[7]  = 0;
        Results.Header[8]  = BitmapSize;
        Results.Header[9]  = 0x00000B12;
        Results.Header[10] = 0x00000B12;
        Results.Header[11] = 0;
        Results.Header[12] = 0;
    // } else Results->Bitmap = NULL;
    } 

    // // Log file
    // Results->Log = Config->Log;
    // if (Config->Log) {
    //     asprintf(&FileName, "%s/%s.log", Config->Path, Config->Prefix);
    //     Results->LogFile = fopen(FileName, "w");
    //     if (Results->LogFile == NULL)
    //         eprintf("Failed to open log file '%s'.\n", FileName);
    //     free(FileName);
    // } else
    //     Results->LogFile = stdout;

    // // Optional results exportation file
    // Results->Export = Config->Export;
    // if (Config->Export) {
    //     asprintf(&FileName, "%s/%s.vti", Config->Path, Config->Prefix);
    //     Results->ExportFile = fopen(FileName, "w");
    //     if (Results->ExportFile == NULL)
    //         eprintf("Failed to open export file '%s'.\n", FileName);
    //     free(FileName);
    // }

    return Results;
}

/*
 *  clearResults: clear Results container.
 */

void clearResults(results* Results)
{
    // free(Results->Density);
    // free(Results->Velocity);
    // free(Results->Delta);
    // if (Results->Bitmap) free(Results->Bitmap);
    // if (Results->Export) fclose(Results->ExportFile);
    // if (Results->Log) fclose(Results->LogFile);
    // free(Results);
}
